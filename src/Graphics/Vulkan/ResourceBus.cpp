/*==================================================================*\
  ResourceBus.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/ResourceBus.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/ImageSource.hpp>
#include <Graphics/MeshSource.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	namespace {

		enum : VkDeviceSize { TightPack = 0u };

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ETPureFunctionHint VkSubmitInfo AsSubmitInfo(const ResourceBus::TransferPhase& phase, void* extensions = nullptr) ETNoexceptHint {
			return VkSubmitInfo {
				VK_STRUCTURE_TYPE_SUBMIT_INFO,
				/*pNext =*/extensions,
				uint32(phase.waits.GetSize()),
				phase.waits.GetData<VkSemaphore>(),
				phase.waits.GetData<VkPipelineStageFlags>(),
				ETCountOf(phase.commands),
				phase.commands,
				uint32(phase.signals.GetSize()),
				phase.signals.GetData()
			};
		}

	} // anonymous namespace

	ResourceBus::BindPhase::BindPhase(BindPhase&& phase) :
		BindPhase() {
		Swap(*this, phase);
	}

	// ---------------------------------------------------

	ResourceBus::BindPhase::BindPhase() :
		complete(nullptr),
		waits(MallocAllocator("Bind Phase Waits List Allocator")),
		signals(MallocAllocator("Bind Phase Signals List Allocator")),
		imageBinds(MallocAllocator("Bind Phase Image Sparse Binds List Allocator")),
		imageOpaqueBinds(MallocAllocator("Bind Phase Image Opaque Binds List Allocator")),
		bufferBinds(MallocAllocator("Bind Phase Buffer Binds List Allocator")) {}

	// ---------------------------------------------------

	ResourceBus::BindPhase::~BindPhase() {
		ET_ASSERT(complete == nullptr, "Leaking Vulkan semaphore object!");
	}

	// ---------------------------------------------------

	ResourceBus::TransferPhase::TransferPhase(TransferPhase&& phase) :
		TransferPhase() {
		Swap(*this, phase);
	}

	// ---------------------------------------------------

	ResourceBus::TransferPhase::TransferPhase() :
		waits(MallocAllocator("Frame I/O Phase Wait List Allocator")),
		signals(MallocAllocator("Frame I/O Phase Signals List Allocator")),
		commands {} {}

	// ---------------------------------------------------

	ResourceBus::TransferPhase::~TransferPhase() {
		ET_ASSERT(commands == nullptr, "Leaking Vulkan command buffer!");
	}

	// ---------------------------------------------------

	ResourceBus::Frame::Frame() :
		_bindsConsumed(nullptr),
		_transfersConsumed(nullptr),
		_commandPool(nullptr) {}

	// ---------------------------------------------------

	bool ResourceBus::Frame::GetBindsConsumed(Gpu& gpu) const ETNoexceptHint {
		return _bindsConsumed == VK_NULL_HANDLE || vkGetFenceStatus(gpu, _bindsConsumed) != VK_NOT_READY;
	}

	// ---------------------------------------------------

	bool ResourceBus::Frame::GetTransfersConsumed(Gpu& gpu) const ETNoexceptHint {
		return _transfersConsumed == VK_NULL_HANDLE || vkGetFenceStatus(gpu, _transfersConsumed) != VK_NOT_READY;
	}

	// ---------------------------------------------------

	void ResourceBus::Frame::PushToGpu(VkImage target, VkOffset3D offset, VkExtent3D extent, VkImageSubresourceLayers subresource) {
		const VkDeviceSize size(extent.width * extent.height * extent.depth * subresource.layerCount);
		VkBufferImageCopy  copies[] = { {
            /*bufferOffset =*/0u, // Offset in staging buffer will be filled in below.
            TightPack,
            TightPack,
            subresource,
            offset,
            extent,
        } };

		vkCmdCopyBufferToImage(_phases[StandardUpload].commands[0], _stagingBuffer, target, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, ETCountOf(copies), copies);
	}

	// ---------------------------------------------------

	void ResourceBus::Frame::PushToGpu(VkBuffer target, VkDeviceSize offset, VkDeviceSize extent) {
		VkBufferCopy copies[] = { { /*srcOffset =*/0u, // Offset in staging buffer will be filled in below.
									offset,
									extent } };

		vkCmdCopyBuffer(_phases[StandardUpload].commands[0], _stagingBuffer, target, ETCountOf(copies), copies);
	}

	// ---------------------------------------------------

	void ResourceBus::Frame::PullToHost(VkImage source, VkOffset3D offset, VkExtent3D extent, VkImageSubresourceLayers subresource) {
		const VkDeviceSize size(extent.width * extent.height * extent.depth * subresource.layerCount);
		VkBufferImageCopy  copies[] = { {
            /*bufferOffset =*/0u, // Offset in staging buffer will be filled in below.
            TightPack,
            TightPack,
            subresource,
            offset,
            extent,
        } };

		vkCmdCopyImageToBuffer(_phases[StandardDownload].commands[0], source, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, _stagingBuffer, ETCountOf(copies), copies);
	}

	// ---------------------------------------------------

	void ResourceBus::Frame::PullToHost(VkBuffer source, VkDeviceSize offset, VkDeviceSize extent) {
		VkBufferCopy copies[] = { { offset,
									/*dstOffset =*/0u, // Offset in staging buffer will be filled in below.
									extent } };

		vkCmdCopyBuffer(_phases[StandardDownload].commands[0], source, _stagingBuffer, ETCountOf(copies), copies);
	}

	// ---------------------------------------------------

	VkResult ResourceBus::Frame::SubmitIo(Gpu& gpu) {
		ET_ABORT_UNLESS(vkResetCommandPool(gpu, _commandPool, /*flags =*/0u));
		for (TransferPhase& phase : _phases) {
			const VkCommandBufferBeginInfo beginInfo {
				VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
				/*pNext =*/nullptr, // No extension structures.
				VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
				/*pInheritanceInfo =*/nullptr
			};
			ET_ABORT_UNLESS(vkBeginCommandBuffer(phase.commands[0], ETAddressOf(beginInfo)));
		}

		// Transition all resources from transfer optimal state into shader optimal state.
		static ETConstexpr VkMemoryBarrier entryBarriers[] = {
			VkMemoryBarrier {
				VK_STRUCTURE_TYPE_MEMORY_BARRIER,
				/*pNext =*/nullptr,
				/*srcAccessMask =*/VK_ACCESS_MEMORY_WRITE_BIT,
				/*dstAccessMask =*/VK_ACCESS_TRANSFER_READ_BIT }
		};
		static ETConstexpr VkMemoryBarrier exitBarriers[] = {
			VkMemoryBarrier {
				VK_STRUCTURE_TYPE_MEMORY_BARRIER,
				/*pNext =*/nullptr,
				/*srcAccessMask =*/VK_ACCESS_TRANSFER_WRITE_BIT,
				/*dstAccessMask =*/VK_ACCESS_MEMORY_READ_BIT }
		};

		vkCmdPipelineBarrier(_phases[StandardDownload].commands[0], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, /*dependencyFlags =*/0u, ETCountOf(entryBarriers), entryBarriers, 0u, nullptr, 0u, nullptr);
		vkCmdPipelineBarrier(_phases[StandardUpload].commands[0], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, /*dependencyFlags =*/0u, ETCountOf(exitBarriers), exitBarriers, 0u, nullptr, 0u, nullptr);
		vkCmdPipelineBarrier(_phases[SparseUpload].commands[0], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, /*dependencyFlags =*/0u, ETCountOf(exitBarriers), exitBarriers, 0u, nullptr, 0u, nullptr);

		for (TransferPhase& phase : _phases) {
			ET_ABORT_UNLESS(vkEndCommandBuffer(phase.commands[0]));
		}

		ReadLock   _(_bindsMutex);
		const auto bufferBinds(ETStackAlloc(VkSparseBufferMemoryBindInfo, _sparseBinds.bufferBinds.GetSize()));
		const auto imageOpaqueBinds(ETStackAlloc(VkSparseImageOpaqueMemoryBindInfo, _sparseBinds.imageOpaqueBinds.GetSize()));
		const auto imageBinds(ETStackAlloc(VkSparseImageMemoryBindInfo, _sparseBinds.imageBinds.GetSize()));
		ET_AT_SCOPE_EXIT(_sparseBinds.bufferBinds.Clear());
		ET_AT_SCOPE_EXIT(_sparseBinds.imageOpaqueBinds.Clear());
		ET_AT_SCOPE_EXIT(_sparseBinds.imageBinds.Clear());

		Transform(_sparseBinds.bufferBinds.ConstBegin(), _sparseBinds.bufferBinds.ConstEnd(), bufferBinds, [](const Pair<const VkBuffer, ArrayList<VkSparseMemoryBind>>& bind) ETNoexceptHint {
			return VkSparseBufferMemoryBindInfo {
				/*buffer =*/bind.first,
				/*bindCount =*/uint32(bind.second.GetSize()),
				/*pBinds =*/bind.second.GetData()
			};
		});
		Transform(_sparseBinds.imageOpaqueBinds.ConstBegin(), _sparseBinds.imageOpaqueBinds.ConstEnd(), imageOpaqueBinds, [](const Pair<const VkImage, ArrayList<VkSparseMemoryBind>>& bind) ETNoexceptHint {
			return VkSparseImageOpaqueMemoryBindInfo {
				/*image =*/bind.first,
				/*bindCount =*/uint32(bind.second.GetSize()),
				/*pBinds =*/bind.second.GetData()
			};
		});
		Transform(_sparseBinds.imageBinds.ConstBegin(), _sparseBinds.imageBinds.ConstEnd(), imageBinds, [](const Pair<const VkImage, ArrayList<VkSparseImageMemoryBind>>& bind) ETNoexceptHint {
			return VkSparseImageMemoryBindInfo {
				/*image =*/bind.first,
				/*bindCount =*/uint32(bind.second.GetSize()),
				/*pBinds =*/bind.second.GetData()
			};
		});

		const VkFence allFences[] = { _bindsConsumed, _transfersConsumed };
		ET_ABORT_UNLESS(vkResetFences(gpu, uint32(ETCountOf(allFences)), allFences));
		ET_ABORT_UNLESS(gpu.BindAsync(_bindsConsumed, { // clang-format off
			VkBindSparseInfo {
				VK_STRUCTURE_TYPE_BIND_SPARSE_INFO,
				/*pNext =*/nullptr,
				uint32(_sparseBinds.waits.GetSize()), _sparseBinds.waits.GetData(),
				uint32(_sparseBinds.bufferBinds.GetSize()), bufferBinds,
				uint32(_sparseBinds.imageOpaqueBinds.GetSize()), imageOpaqueBinds,
				uint32(_sparseBinds.imageBinds.GetSize()), imageBinds,
				uint32(_sparseBinds.signals.GetSize()), _sparseBinds.signals.GetData() } })); // clang-format on
		ET_ABORT_UNLESS(gpu.SubmitAsync(Transfer, _transfersConsumed, { AsSubmitInfo(_phases[SparseUpload]), AsSubmitInfo(_phases[StandardDownload]), AsSubmitInfo(_phases[StandardUpload]) }));

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult ResourceBus::Frame::MakeResident(Mesh& target, const MeshSource<SkinnedVertex>& source) {
		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult ResourceBus::Frame::MakeResident(ShaderImage& target, const ImageSource& source) {
		const auto image(source.GetDescription());

		for (uint32 mip(0u); mip < image.mips; ++mip) {
			for (uint32 slice(0u); slice < image.slices; ++slice) {
				const VkExtent3D dimensions { Max(image.texelWidth >> mip, 1u), Max(image.texelHeight >> mip, 1u), Max(image.texelDepth >> mip, 1u) };
				VkExtent3D       chunkExtent {};
				VkOffset3D       offset { 0u, 0u, 0u };

				PushToGpu(target, offset, chunkExtent, // clang-format off
						  VkImageSubresourceLayers {
							  VK_IMAGE_ASPECT_COLOR_BIT,
							  mip,
							  slice,
							  /*layerCount =*/1u }); // clang-format on
			}
		}

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult ResourceBus::Frame::BindResources(Gpu& gpu) {
		using ::Eldritch2::Swap;

		VkFence     bindsConsumed, transfersConsumed;
		ETConstexpr VkFenceCreateInfo consumedInfo {
			VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			/*pNext =*/nullptr,
			VK_FENCE_CREATE_SIGNALED_BIT
		};
		ET_ABORT_UNLESS(vkCreateFence(gpu, ETAddressOf(consumedInfo), gpu.GetAllocationCallbacks(), ETAddressOf(bindsConsumed)));
		ET_AT_SCOPE_EXIT(vkDestroyFence(gpu, bindsConsumed, gpu.GetAllocationCallbacks()));
		ET_ABORT_UNLESS(vkCreateFence(gpu, ETAddressOf(consumedInfo), gpu.GetAllocationCallbacks(), ETAddressOf(transfersConsumed)));
		ET_AT_SCOPE_EXIT(vkDestroyFence(gpu, transfersConsumed, gpu.GetAllocationCallbacks()));

		VkCommandPool                 commandPool;
		const VkCommandPoolCreateInfo commandPoolInfo {
			VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			/*pNext =*/nullptr,
			VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
			gpu.GetQueueFamilyByConcept(Transfer)
		};
		ET_ABORT_UNLESS(vkCreateCommandPool(gpu, ETAddressOf(commandPoolInfo), gpu.GetAllocationCallbacks(), ETAddressOf(commandPool)));
		ET_AT_SCOPE_EXIT(vkDestroyCommandPool(gpu, commandPool, gpu.GetAllocationCallbacks()));

		VkCommandBuffer                   commands[ETCountOf(_phases)];
		const VkCommandBufferAllocateInfo commandsInfo {
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			/*pNext =*/nullptr,
			commandPool,
			VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			ETCountOf(commands)
		};
		ET_ABORT_UNLESS(vkAllocateCommandBuffers(gpu, ETAddressOf(commandsInfo), commands));
		ET_AT_SCOPE_EXIT(if (commandPool) vkFreeCommandBuffers(gpu, commandPool, ETCountOf(commands), commands));

		BindPhase   sparseBinds;
		ETConstexpr VkSemaphoreCreateInfo bindsCompleteInfo {
			VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u
		};
		ET_ABORT_UNLESS(vkCreateSemaphore(gpu, ETAddressOf(bindsCompleteInfo), gpu.GetAllocationCallbacks(), ETAddressOf(sparseBinds.complete)));

		TransferPhase phases[ETCountOf(_phases)];

		if (gpu.RequiresSemaphore(SparseBinding, Transfer)) {
			sparseBinds.signals.Append(sparseBinds.complete);
			phases[SparseUpload].waits.Append(sparseBinds.complete, VK_PIPELINE_STAGE_TRANSFER_BIT);
		}

		Swap(_bindsConsumed, bindsConsumed);
		Swap(_transfersConsumed, transfersConsumed);
		Swap(_commandPool, commandPool);
		Swap(_sparseBinds, sparseBinds);
		Swap(_phases, phases);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void ResourceBus::Frame::FreeResources(Gpu& gpu) {
		for (TransferPhase& phase : _phases) {
			phase.signals.Clear();
			phase.waits.Clear();
			if (_commandPool) {
				vkFreeCommandBuffers(gpu, _commandPool, ETCountOf(phase.commands), phase.commands);
			}
		}

		if (VkCommandPool pool = eastl::exchange(_commandPool, nullptr)) {
			vkDestroyCommandPool(gpu, pool, gpu.GetAllocationCallbacks());
		}

		vkDestroyFence(gpu, eastl::exchange(_transfersConsumed, nullptr), gpu.GetAllocationCallbacks());
		vkDestroyFence(gpu, eastl::exchange(_bindsConsumed, nullptr), gpu.GetAllocationCallbacks());
	}

	// ---------------------------------------------------

	ResourceBus::ResourceBus(ResourceBus&& bus) :
		ResourceBus() {
		Swap(*this, bus);
	}

	// ---------------------------------------------------

	ResourceBus::ResourceBus() :
		_meshesBySource(MallocAllocator("Geometry By Source Collection Allocator")),
		_imagesBySource(MallocAllocator("Image By Source Collection Allocator")) {
	}

	// ---------------------------------------------------

	VkResult ResourceBus::BindResources(Gpu& gpu, VkDeviceSize frameUploadByteSize, VkDeviceSize vertexCacheByteSize, VkDeviceSize indexCacheByteSize) {
		using Eldritch2::Swap;

		VertexCache vertexCache;
		ET_ABORT_UNLESS(vertexCache.BindResources(gpu, vertexCacheByteSize, indexCacheByteSize));
		ET_AT_SCOPE_EXIT(vertexCache.FreeResources(gpu));

		UploadBuffer uploadBuffer;
		ET_ABORT_UNLESS(uploadBuffer.BindResources(gpu, frameUploadByteSize * ETCountOf(_frames)));
		ET_AT_SCOPE_EXIT(uploadBuffer.FreeResources(gpu));

		Frame frames[ETCountOf(_frames)];
		ET_AT_SCOPE_EXIT(for (Frame& frame
							  : frames) frame.FreeResources(gpu));
		for (Frame& frame : frames) {
			ET_ABORT_UNLESS(frame.BindResources(gpu));
		}

		Swap(_vertexCache, vertexCache);
		Swap(_uploadBuffer, uploadBuffer);
		Swap(_frames, frames);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void ResourceBus::FreeResources(Gpu& gpu) {
		for (ResidentSet<ImageSource, ShaderImage>::ValueType& image : _imagesBySource) {
			image.second.FreeResources(gpu);
		}

		for (ResidentSet<MeshSource<SkinnedVertex>, Mesh>::ValueType& mesh : _meshesBySource) {
			mesh.second.FreeResources(_vertexCache);
		}

		_imagesBySource.Clear();
		_meshesBySource.Clear();

		_uploadBuffer.FreeResources(gpu);
		_vertexCache.FreeResources(gpu);
	}

	// ---------------------------------------------------

	VkResult ResourceBus::Insert(Gpu& /*gpu*/, const MeshSource<SkinnedVertex>& source, bool andMakeResident) {
		const MeshSurface surface(source.GetSurface(0u));
		Mesh              mesh;

		ET_ABORT_UNLESS(mesh.BindResources(_vertexCache, surface.primitiveCount, uint32(surface.type) * surface.primitiveCount));
		ET_AT_SCOPE_EXIT(mesh.FreeResources(_vertexCache));

		if (andMakeResident) {
			ET_ABORT_UNLESS(_frames[0].MakeResident(mesh, source));
		}

		_meshesBySource.Emplace(ETAddressOf(source), eastl::move(mesh));
		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult ResourceBus::Insert(Gpu& gpu, const ImageSource& source, bool andMakeResident) {
		const auto  description(source.GetDescription());
		ShaderImage image;

		ET_ABORT_UNLESS(image.BindResources(gpu, TextureFormats[size_t(description.format)].deviceFormat, { description.texelWidth, description.texelHeight, description.texelDepth }, description.mips, description.slices));
		ET_AT_SCOPE_EXIT(image.FreeResources(gpu));

		if (andMakeResident) {
			ET_ABORT_UNLESS(_frames[0].MakeResident(image, source));
		}

		_imagesBySource.Emplace(ETAddressOf(source), eastl::move(image));
		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void ResourceBus::Erase(Gpu& /*gpu*/, const MeshSource<SkinnedVertex>& source) {
		const auto candidate(_meshesBySource.Find(ETAddressOf(source)));
		if (ET_UNLIKELY(candidate == _meshesBySource.End())) {
			return;
		}

		candidate->second.FreeResources(_vertexCache);
		_meshesBySource.Erase(candidate);
	}

	// ---------------------------------------------------

	void ResourceBus::Erase(Gpu& gpu, const ImageSource& source) {
		const auto candidate(_imagesBySource.Find(ETAddressOf(source)));
		if (ET_UNLIKELY(candidate == _imagesBySource.End())) {
			return;
		}

		candidate->second.FreeResources(gpu);
		_imagesBySource.Erase(candidate);
	}

	// ---------------------------------------------------

	void Swap(ResourceBus::BindPhase& lhs, ResourceBus::BindPhase& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs.waits, rhs.waits);
		Swap(lhs.signals, rhs.signals);
		Swap(lhs.imageBinds, rhs.imageBinds);
		Swap(lhs.imageOpaqueBinds, rhs.imageOpaqueBinds);
		Swap(lhs.bufferBinds, rhs.bufferBinds);
		Swap(lhs.complete, rhs.complete);
	}

	// ---------------------------------------------------

	void Swap(ResourceBus::TransferPhase& lhs, ResourceBus::TransferPhase& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs.waits, rhs.waits);
		Swap(lhs.signals, rhs.signals);
		Swap(lhs.commands, rhs.commands);
	}

	// ---------------------------------------------------

	void Swap(ResourceBus::Frame& lhs, ResourceBus::Frame& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._bindsConsumed, rhs._bindsConsumed);
		Swap(rhs._transfersConsumed, rhs._transfersConsumed);
		Swap(lhs._commandPool, rhs._commandPool);
		Swap(lhs._sparseBinds, rhs._sparseBinds);
		Swap(lhs._phases, rhs._phases);
	}

	// ---------------------------------------------------

	void Swap(ResourceBus& lhs, ResourceBus& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._vertexCache, rhs._vertexCache);
		Swap(lhs._uploadBuffer, rhs._uploadBuffer);
		Swap(lhs._frames, rhs._frames);
		Swap(lhs._meshesBySource, rhs._meshesBySource);
		Swap(lhs._imagesBySource, rhs._imagesBySource);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
