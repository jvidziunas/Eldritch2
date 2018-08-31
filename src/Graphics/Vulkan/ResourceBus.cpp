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
#include <Scheduling/JobExecutor.hpp>
#include <Graphics/ImageSource.hpp>
#include <Graphics/MeshSource.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Scheduling;

	namespace {

		ETInlineHint ETForceInlineHint ETPureFunctionHint VkSubmitInfo AsSubmitInfo(const ResourceBus::Phase& phase, void* extensions = nullptr) ETNoexceptHint {
			return VkSubmitInfo {
				VK_STRUCTURE_TYPE_SUBMIT_INFO,
				/*pNext =*/extensions,
				uint32(phase.waits.GetSize()),
				phase.waits.Get<VkSemaphore>(),
				phase.waits.Get<VkPipelineStageFlags>(),
				/*commandBufferCount =*/1u,
				ETAddressOf(phase.commands),
				uint32(phase.signals.GetSize()),
				phase.signals.GetData()
			};
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ETPureFunctionHint VkBindSparseInfo AsBindSparseInfo(const ResourceBus::BindPhase& phase, void* extensions = nullptr) ETNoexceptHint {
			return VkBindSparseInfo {
				VK_STRUCTURE_TYPE_BIND_SPARSE_INFO,
				/*pNext =*/extensions,
				uint32(phase.waits.GetSize()), phase.waits.GetData(),
				0u, nullptr, // No buffer binds.
				uint32(phase.imageOpaqueBinds.GetSize()), phase.imageOpaqueBinds.GetData(),
				uint32(phase.imageBinds.GetSize()), phase.imageBinds.GetData(),
				uint32(phase.signals.GetSize()), phase.signals.GetData()
			};
		}

	} // anonymous namespace

	ResourceBus::ResourceBus(ResourceBus&& bus) :
		ResourceBus() {
		Swap(*this, bus);
	}

	// ---------------------------------------------------

	ResourceBus::ResourceBus() :
		_bindsConsumed(nullptr),
		_transfersConsumed(nullptr),
		_commandPool(nullptr),
		_phases {
			{ MallocAllocator("Standard Download Waits List Allocator"), MallocAllocator("Standard Download Signals List Allocator"), VK_NULL_HANDLE },
			{ MallocAllocator("Standard Upload Waits List Allocator"), MallocAllocator("Standard Upload Signals List Allocator"), VK_NULL_HANDLE },
			{ MallocAllocator("Sparse Upload Waits List Allocator"), MallocAllocator("Sparse Upload Waits List Allocator"), VK_NULL_HANDLE },
		},
		_meshesBySource(MallocAllocator("Geometry By Source Collection Allocator")),
		_imagesBySource(MallocAllocator("Image By Source Collection Allocator")) {
	}

	// ---------------------------------------------------

	bool ResourceBus::GetBindsConsumed(Gpu& gpu) const ETNoexceptHint {
		return vkGetFenceStatus(gpu, _bindsConsumed) != VK_NOT_READY;
	}

	// ---------------------------------------------------

	bool ResourceBus::GetTransfersConsumed(Gpu& gpu) const ETNoexceptHint {
		return vkGetFenceStatus(gpu, _transfersConsumed) != VK_NOT_READY;
	}

	// ---------------------------------------------------

	VkResult ResourceBus::SubmitFrameIo(Gpu& gpu) {
		ET_ABORT_UNLESS(vkResetCommandPool(gpu, _commandPool, /*flags =*/0u));
		for (Phase& phase : _phases) {
			const VkCommandBufferBeginInfo beginInfo {
				VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
				/*pNext =*/nullptr, // No extension structures.
				VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
				/*pInheritanceInfo =*/nullptr
			};
			ET_ABORT_UNLESS(vkBeginCommandBuffer(phase.commands, ETAddressOf(beginInfo)));
		}

		// Transition all resources from transfer optimal state into shader optimal state.
		VkMemoryBarrier barrier {
			VK_STRUCTURE_TYPE_MEMORY_BARRIER,
			/*pNext =*/nullptr,
			/*srcAccessMask =*/VK_ACCESS_MEMORY_WRITE_BIT,
			/*dstAccessMask =*/VK_ACCESS_TRANSFER_READ_BIT
		};

		vkCmdPipelineBarrier(_phases[StandardDownload].commands, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0u, 1u, &barrier, 0u, nullptr, 0u, nullptr);

		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;

		vkCmdPipelineBarrier(_phases[StandardUpload].commands, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0u, 1u, &barrier, 0u, nullptr, 0u, nullptr);
		vkCmdPipelineBarrier(_phases[SparseUpload].commands, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0u, 1u, &barrier, 0u, nullptr, 0u, nullptr);

		for (Phase& phase : _phases) {
			ET_ABORT_UNLESS(vkEndCommandBuffer(phase.commands));
		}

		const VkFence fences[] = { _bindsConsumed, _transfersConsumed };
		ET_ABORT_UNLESS(vkResetFences(gpu, uint32(ETCountOf(fences)), fences));
		ET_ABORT_UNLESS(gpu.BindAsync(SparseBinding, _bindsConsumed, { AsBindSparseInfo(_sparseBinds) }));
		ET_ABORT_UNLESS(gpu.SubmitAsync(Transfer, _transfersConsumed, { AsSubmitInfo(_phases[SparseUpload]), AsSubmitInfo(_phases[StandardDownload]), AsSubmitInfo(_phases[StandardUpload]) }));
	}

	// ---------------------------------------------------

	void ResourceBus::PushToGpu(VkImage target, VkOffset3D offset, VkExtent3D extent, VkImageSubresourceLayers subresource) {
		const VkDeviceSize size(extent.width * extent.height * extent.depth * subresource.layerCount);
		VkBufferImageCopy  copy {
            /*bufferOffset =*/0u, // Offset in staging buffer will be filled in below.
            TightPack,
            TightPack,
            subresource,
            offset,
            extent,
		};

		if (!LockRegion<VkDeviceSize>(_readOffset, size, copy.bufferOffset)) {
			return;
		}

		vkCmdCopyBufferToImage(_phases[StandardUpload].commands, _stagingBuffer, target, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, /*regionCount =*/1u, ETAddressOf(copy));
	}

	// ---------------------------------------------------

	void ResourceBus::PushToGpu(VkBuffer target, VkDeviceSize offset, VkDeviceSize extent) {
		VkBufferCopy copy {
			/*srcOffset =*/0u, // Offset in staging buffer will be filled in below.
			offset,
			extent
		};

		if (!LockRegion<VkDeviceSize>(_readOffset, extent, copy.srcOffset)) {
			return;
		}

		vkCmdCopyBuffer(_phases[StandardUpload].commands, _stagingBuffer, target, /*regionCount =*/1u, ETAddressOf(copy));
	}

	// ---------------------------------------------------

	void ResourceBus::PullToHost(VkImage source, VkOffset3D offset, VkExtent3D extent, VkImageSubresourceLayers subresource) {
		const VkDeviceSize size(extent.width * extent.height * extent.depth * subresource.layerCount);
		VkBufferImageCopy  copy {
            /*bufferOffset =*/0u, // Offset in staging buffer will be filled in below.
            TightPack,
            TightPack,
            subresource,
            offset,
            extent,
		};

		if (!LockRegion<VkDeviceSize>(_readOffset, size, copy.bufferOffset)) {
			return;
		}

		vkCmdCopyImageToBuffer(_phases[StandardDownload].commands, source, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, _stagingBuffer, /*regionCount =*/1u, ETAddressOf(copy));
	}

	// ---------------------------------------------------

	void ResourceBus::PullToHost(VkBuffer source, VkDeviceSize offset, VkDeviceSize extent) {
		VkBufferCopy copy {
			offset,
			/*dstOffset =*/0u, // Offset in staging buffer will be filled in below.
			extent
		};

		if (!LockRegion<VkDeviceSize>(_readOffset, extent, copy.dstOffset)) {
			return;
		}

		vkCmdCopyBuffer(_phases[StandardDownload].commands, source, _stagingBuffer, /*regionCount =*/1u, ETAddressOf(copy));
	}

	// ---------------------------------------------------

	VkResult ResourceBus::BindResources(Gpu& gpu, VkDeviceSize transferBufferSize, VkDeviceSize vertexCacheSize, VkDeviceSize indexCacheSize) {
		using Eldritch2::Swap;

		TransferBuffer stagingBuffer;
		ET_ABORT_UNLESS(stagingBuffer.BindResources(gpu, transferBufferSize));
		ET_AT_SCOPE_EXIT(stagingBuffer.FreeResources(gpu));

		VertexCache vertexCache;
		ET_ABORT_UNLESS(vertexCache.BindResources(gpu, vertexCacheSize, indexCacheSize));
		ET_AT_SCOPE_EXIT(vertexCache.FreeResources(gpu));

		VkFence                 commandsConsumed;
		const VkFenceCreateInfo commandsConsumedInfo {
			VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			/*pNext =*/nullptr,
			VK_FENCE_CREATE_SIGNALED_BIT
		};
		ET_ABORT_UNLESS(vkCreateFence(gpu, ETAddressOf(commandsConsumedInfo), gpu.GetAllocationCallbacks(), &commandsConsumed));
		ET_AT_SCOPE_EXIT(vkDestroyFence(gpu, commandsConsumed, gpu.GetAllocationCallbacks()));

		VkCommandPool                 commandPool;
		const VkCommandPoolCreateInfo commandPoolInfo {
			VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			/*pNext =*/nullptr,
			VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
			gpu.GetQueueFamilyByConcept(Transfer)
		};
		ET_ABORT_UNLESS(vkCreateCommandPool(gpu, ETAddressOf(commandPoolInfo), gpu.GetAllocationCallbacks(), &commandPool));
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

		Phase phases[ETCountOf(_phases)] = {
			{ _phases[StandardDownload].waits.GetAllocator(), _phases[StandardDownload].signals.GetAllocator(), commands[StandardDownload] },
			{ _phases[StandardUpload].waits.GetAllocator(), _phases[StandardUpload].signals.GetAllocator(), commands[StandardUpload] },
			{ _phases[SparseUpload].waits.GetAllocator(), _phases[SparseUpload].signals.GetAllocator(), commands[SparseUpload] }
		};

		//	Swap created resources into the object.
		Swap(_stagingBuffer, stagingBuffer);
		Swap(_vertexCache, vertexCache);
		Swap(_transfersConsumed, commandsConsumed);
		Swap(_commandPool, commandPool);
		Swap(_phases, phases);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void ResourceBus::FreeResources(Gpu& gpu) {
		for (ResidentSet<ImageSource, ShaderImage>::ValueType& image : _imagesBySource) {
			image.second.FreeResources(gpu);
		}

		for (ResidentSet<MeshSource, Mesh>::ValueType& mesh : _meshesBySource) {
			mesh.second.FreeResources(_vertexCache);
		}

		_imagesBySource.Clear();
		_meshesBySource.Clear();

		for (VkSemaphore& semaphore : _phaseCompleted) {
			vkDestroySemaphore(gpu, eastl::exchange(semaphore, nullptr), gpu.GetAllocationCallbacks());
		}

		if (VkCommandPool pool = eastl::exchange(_commandPool, nullptr)) {
			vkFreeCommandBuffers(gpu, pool, ETCountOf(_commands), _commands);
			vkDestroyCommandPool(gpu, pool, gpu.GetAllocationCallbacks());
		}

		vkDestroyFence(gpu, eastl::exchange(_transfersConsumed, nullptr), gpu.GetAllocationCallbacks());
		vkDestroyFence(gpu, eastl::exchange(_bindsConsumed, nullptr), gpu.GetAllocationCallbacks());

		_vertexCache.FreeResources(gpu);
		_stagingBuffer.FreeResources(gpu);
	}

	// ---------------------------------------------------

	VkResult ResourceBus::Insert(Gpu& gpu, const MeshSource& source, bool andMakeResident) {
		const MeshSource::SurfaceDescription surface(source.GetSurface(0u));
		Mesh                                 mesh;

		ET_ABORT_UNLESS(mesh.BindResources(_vertexCache, surface.primitiveCount, surface.type * surface.primitiveCount));
		ET_AT_SCOPE_EXIT(mesh.FreeResources(_vertexCache));

		if (andMakeResident) {
			ET_ABORT_UNLESS(MakeResident(mesh, source));
		}

		_meshesBySource.Emplace(ETAddressOf(source), eastl::move(mesh));
		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult ResourceBus::Insert(Gpu& gpu, const ImageSource& source, bool andMakeResident) {
		const auto  description(source.GetDescription());
		ShaderImage image;

		ET_ABORT_UNLESS(image.BindResources(gpu, DescribeFormat(description.format).format, VkExtent3D { description.widthInTexels, description.heightInTexels, description.depthInTexels }, description.mips, description.slices));
		ET_AT_SCOPE_EXIT(image.FreeResources(gpu));

		if (andMakeResident) {
			ET_ABORT_UNLESS(MakeResident(image, source));
		}

		_imagesBySource.Emplace(ETAddressOf(source), eastl::move(image));
		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void ResourceBus::Erase(Gpu& gpu, const MeshSource& source) {
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

	VkResult ResourceBus::MakeResident(Mesh& target, const MeshSource& source) {
		VkDeviceSize remainingBytes(target.GetVerticesSize());

		for (const VkDeviceSize end(target.GetVertexOffset() + remainingBytes); remainingBytes;) {
			VkDeviceSize chunkSize(Min<VkDeviceSize>(remainingBytes, 16384u));

			PushToGpu(target.GetVertices(), end - remainingBytes, chunkSize);
			remainingBytes -= chunkSize;
		}

		remainingBytes = target.GetIndicesSize();
		for (const VkDeviceSize end(target.GetIndexOffset() + remainingBytes); remainingBytes;) {
			VkDeviceSize chunkSize(Min<VkDeviceSize>(remainingBytes, 16384u));

			PushToGpu(target.GetIndices(), end - remainingBytes, chunkSize);
			remainingBytes -= chunkSize;
		}

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult ResourceBus::MakeResident(ShaderImage& target, const ImageSource& source) {
		const auto image(source.GetDescription());

		for (uint32 mip(0u); mip < image.mips; ++mip) {
			for (uint32 slice(0u); slice < image.slices; ++slice) {
				const auto       subImage(source.GetDescription(GetSubimageIndex(slice, mip, image.mips)));
				const VkExtent3D dimensions { subImage.widthInTexels, subImage.heightInTexels, subImage.depthInTexels };
				VkOffset3D       offset { 0u, 0u, 0u };
				VkExtent3D       chunkExtent {};

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

	void Swap(ResourceBus::Frame& lhs, ResourceBus::Frame& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs.bindsConsumed, rhs.bindsConsumed);
		Swap(rhs.transfersConsumed, rhs.transfersConsumed);
		Swap(lhs.commandPool, rhs.commandPool);
		Swap(lhs.sparseBinds, rhs.sparseBinds);
		Swap(lhs.phases, rhs.phases);
	}

	// ---------------------------------------------------

	void Swap(ResourceBus& lhs, ResourceBus& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._stagingBuffer, rhs._stagingBuffer);
		Swap(lhs._vertexCache, rhs._vertexCache);
		Swap(lhs._frames, rhs._frames);
		Swap(lhs._meshesBySource, rhs._meshesBySource);
		Swap(lhs._imagesBySource, rhs._imagesBySource);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
