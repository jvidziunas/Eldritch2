/*==================================================================*\
  GpuResourceBus.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GpuResourceBus.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {
	namespace {

		enum : uint32_t {
			TightPack = 0u
		};

		// ---------------------------------------------------

		template <typename Offset>
		ETInlineHint ETPureFunctionHint bool LockRegion(Atomic<Offset>& tracker, Offset extent, Offset& offset) {
			VkDeviceSize transferOffset(tracker.load(std::memory_order_consume));

			do {
				if (transferOffset < extent) {
					return false;
				}
			} while (!tracker.compare_exchange_weak(transferOffset, transferOffset - extent, std::memory_order_release));

			offset = transferOffset;
			return true;
		}

		// ---------------------------------------------------

		template <typename BindInfo, typename Allocator, typename Bind, typename BindAllocator>
		ETInlineHint ArrayList<BindInfo, Allocator> CollectBinds(const Allocator& allocator, ArrayList<Bind, BindAllocator>& binds) {
			using VulkanBind = typename Bind::BindType;

			ArrayList<BindInfo, Allocator> bindInfos(allocator);

			// Group binds by target.
			Sort(binds.Begin(), binds.End(), [](const Bind& lhs, const Bind& rhs) { return lhs.target < rhs.target; });
			for (const Bind& bind : binds) {
				bindInfos.Append(BindInfo{});
			}

			VulkanBind* vulkanBinds(reinterpret_cast<VulkanBind*>(binds.GetData()));
			Bind* const source(binds.GetData());

			for (size_t i(0u); i < binds.GetSize(); ++i) {
				MoveMemory(vulkanBinds + i, static_cast<VulkanBind*>(source + i), sizeof(*vulkanBinds));
			}

			// Patch bind pointers.
			for (BindInfo& bindInfo : bindInfos) {
				if (bindInfo.bindCount == 0u) {
					continue;
				}

				bindInfo.pBinds = vulkanBinds;
				vulkanBinds += bindInfo.bindCount;
			}

			return eastl::move(bindInfos);
		}

	} // anonymous namespace

	GpuResourceBus::GpuResourceBus() :
		_pendingImageBinds(MallocAllocator("Resource Bus Image Bind List Allocator")),
		_pendingImageOpaqueBinds(MallocAllocator("Resource Bus Image Opaque Bind List Allocator")),
		_signals(MallocAllocator("Resource Bus Signal Semaphore List Allocator")),
		_readOffset(0u),
		_writeOffset(0u),
		_phaseCompleted{ nullptr },
		_commandsConsumed(nullptr),
		_commandPool(nullptr),
		_commands{ nullptr } {
	}

	// ---------------------------------------------------

	GpuResourceBus::GpuResourceBus(GpuResourceBus&& bus) :
		GpuResourceBus() {
		Swap(*this, bus);
	}

	// ---------------------------------------------------

	void GpuResourceBus::TransferToGpu(VkImage target, VkImageSubresourceLayers subresource, VkOffset3D targetOffset, VkExtent3D targetExtent) {
		VkBufferImageCopy copy{
			/*bufferOffset =*/0u, // Offset in staging buffer will be filled in below.
			TightPack,
			TightPack,
			subresource,
			targetOffset,
			targetExtent,
		};

		const VkDeviceSize extent(targetExtent.width * targetExtent.height * targetExtent.depth * subresource.layerCount);
		if (!LockRegion<VkDeviceSize>(_readOffset, extent, copy.bufferOffset)) {
			return;
		}

		vkCmdCopyBufferToImage(_commands[StandardUpload], _stagingBuffer, target, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1u, &copy);
	}

	// ---------------------------------------------------

	void GpuResourceBus::TransferToGpu(VkBuffer target, VkDeviceSize targetOffset, VkDeviceSize targetExtent) {
		VkBufferCopy copy{
			/*srcOffset =*/0u, // Offset in staging buffer will be filled in below.
			targetOffset,
			targetExtent
		};

		if (!LockRegion<VkDeviceSize>(_readOffset, targetExtent, copy.srcOffset)) {
			return;
		}

		vkCmdCopyBuffer(_commands[StandardUpload], _stagingBuffer, target, 1u, &copy);
	}

	// ---------------------------------------------------

	void GpuResourceBus::TransferToHost(VkImage source, VkImageSubresourceLayers subresource, VkOffset3D sourceOffset, VkExtent3D sourceExtent) {
		VkBufferImageCopy copy{
			/*bufferOffset =*/0u, // Offset in staging buffer will be filled in below.
			TightPack,
			TightPack,
			subresource,
			sourceOffset,
			sourceExtent,
		};

		const VkDeviceSize extent(sourceExtent.width * sourceExtent.height * sourceExtent.depth * subresource.layerCount);
		if (!LockRegion<VkDeviceSize>(_readOffset, extent, copy.bufferOffset)) {
			return;
		}

		vkCmdCopyImageToBuffer(_commands[StandardDownload], source, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, _stagingBuffer, 1u, &copy);
	}

	// ---------------------------------------------------

	void GpuResourceBus::TransferToHost(VkBuffer source, VkDeviceSize sourceOffset, VkDeviceSize sourceExtent) {
		VkBufferCopy copy{
			sourceOffset,
			/*dstOffset =*/0u, // Offset in staging buffer will be filled in below.
			sourceExtent
		};

		if (!LockRegion<VkDeviceSize>(_readOffset, sourceExtent, copy.dstOffset)) {
			return;
		}

		vkCmdCopyBuffer(_commands[StandardDownload], source, _stagingBuffer, 1u, &copy);
	}

	// ---------------------------------------------------

	void GpuResourceBus::BindRegion(VkImage target, const VkSparseImageMemoryBind& bind) {
		_pendingImageBinds.EmplaceBack(target, bind);
	}

	// ---------------------------------------------------

	void GpuResourceBus::BindRegion(VkImage target, const VkSparseMemoryBind& bind) {
		_pendingImageOpaqueBinds.EmplaceBack(target, bind);
	}

	// ---------------------------------------------------

	bool GpuResourceBus::CheckCommandsConsumed(Gpu& gpu) const {
		return vkGetFenceStatus(gpu, _commandsConsumed) != VK_NOT_READY;
	}

	// ---------------------------------------------------

	VkResult GpuResourceBus::SubmitCommands(Gpu& gpu) {
		constexpr VkPipelineStageFlags AllCommandsStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
		constexpr VkPipelineStageFlags TransferStage    = VK_PIPELINE_STAGE_TRANSFER_BIT;

		ET_FAIL_UNLESS(vkResetCommandPool(gpu, _commandPool, 0u));
		for (VkCommandBuffer commands : _commands) {
			const VkCommandBufferBeginInfo beginInfo{
				VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
				/*pNext =*/nullptr, // No extension structures.
				VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
				/*pInheritanceInfo =*/nullptr
			};
			ET_FAIL_UNLESS(vkBeginCommandBuffer(commands, &beginInfo));
		}

		// Transition all resources from transfer optimal state into shader optimal state.
		VkMemoryBarrier barrier{
			VK_STRUCTURE_TYPE_MEMORY_BARRIER,
			/*pNext =*/nullptr,
			/*srcAccessMask =*/VK_ACCESS_MEMORY_WRITE_BIT,
			/*dstAccessMask =*/VK_ACCESS_TRANSFER_READ_BIT
		};

		vkCmdPipelineBarrier(_commands[StandardDownload], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0u, 1u, &barrier, 0u, nullptr, 0u, nullptr);

		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;

		vkCmdPipelineBarrier(_commands[StandardUpload], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0u, 1u, &barrier, 0u, nullptr, 0u, nullptr);
		vkCmdPipelineBarrier(_commands[SparseUpload], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0u, 1u, &barrier, 0u, nullptr, 0u, nullptr);

		for (VkCommandBuffer& commands : _commands) {
			ET_FAIL_UNLESS(vkEndCommandBuffer(commands));
		}

		ET_AT_SCOPE_EXIT(_pendingImageOpaqueBinds.Clear());
		auto imageOpaqueBinds(CollectBinds<VkSparseImageOpaqueMemoryBindInfo>(MallocAllocator(""), _pendingImageOpaqueBinds));

		ET_AT_SCOPE_EXIT(_pendingImageBinds.Clear());
		auto imageBinds(CollectBinds<VkSparseImageMemoryBindInfo>(MallocAllocator(""), _pendingImageBinds));

		// clang-format off
		const VkSemaphore          signalWaits[]      = { _phaseCompleted[StandardDownload], _phaseCompleted[StandardUpload], _phaseCompleted[SparseUpload] };
		const VkPipelineStageFlags signalWaitStages[] = {                  AllCommandsStage,                AllCommandsStage,              AllCommandsStage };
		ET_FAIL_UNLESS(gpu.BindAsync(QueueConcept::SparseBinding, VK_NULL_HANDLE, {
			AsBindSparseInfo(0u, nullptr, // No buffer binds.
							 imageOpaqueBinds.GetSize(),  imageOpaqueBinds.GetData(),
							 imageBinds.GetSize(), imageBinds.GetData(),
							 { _phaseCompleted[SparseBind] })}));
		ET_FAIL_UNLESS(vkResetFences(gpu, 1u, &_commandsConsumed));
		ET_FAIL_UNLESS(gpu.SubmitAsync(QueueConcept::Transfer, _commandsConsumed, {
			AsSubmitInfo(/* No waits */                                      { _commands[StandardDownload] }, { _phaseCompleted[StandardDownload] }),
			AsSubmitInfo(/* No waits */                                      { _commands[StandardUpload] },   { _phaseCompleted[StandardUpload] }),
			AsSubmitInfo({ _phaseCompleted[SparseBind] }, { TransferStage }, { _commands[SparseUpload] },     { _phaseCompleted[SparseUpload] }),
			AsSubmitInfo(_countof(signalWaits), signalWaits, signalWaitStages, 0u, nullptr, _signals.GetSize(), _signals.GetData())}));
		// clang-format on

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	VkResult GpuResourceBus::InsertSignalSemaphore(Gpu& gpu, VkSemaphore& outSemaphore) {
		VkSemaphore                 semaphore;
		const VkSemaphoreCreateInfo semaphoreInfo{
			VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
			/*pNext =*/nullptr,
			/*flags =*/0u
		};
		ET_FAIL_UNLESS(vkCreateSemaphore(gpu, &semaphoreInfo, gpu.GetAllocationCallbacks(), &semaphore));

		{
			Lock _(_signalsMutex);
			_signals.Insert(semaphore);
		}
		outSemaphore = semaphore;

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void GpuResourceBus::EraseSignalSemaphore(Gpu& gpu, VkSemaphore semaphore) {
		{
			Lock _(_signalsMutex);
			_signals.Erase(semaphore);
		}

		vkDestroySemaphore(gpu, semaphore, gpu.GetAllocationCallbacks());
	}

	// ---------------------------------------------------

	VkResult GpuResourceBus::BindResources(Gpu& gpu, VkDeviceSize transferBufferSize) {
		using ::Eldritch2::Swap;

		TransferBuffer stagingBuffer;
		ET_FAIL_UNLESS(stagingBuffer.BindResources(gpu, transferBufferSize));
		ET_AT_SCOPE_EXIT(stagingBuffer.FreeResources(gpu));

		VkFence                 commandsConsumed;
		const VkFenceCreateInfo commandsConsumedInfo{
			VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			/*pNext =*/nullptr,
			VK_FENCE_CREATE_SIGNALED_BIT
		};
		ET_FAIL_UNLESS(vkCreateFence(gpu, &commandsConsumedInfo, gpu.GetAllocationCallbacks(), &commandsConsumed));
		ET_AT_SCOPE_EXIT(vkDestroyFence(gpu, commandsConsumed, gpu.GetAllocationCallbacks()));

		VkCommandPool                 commandPool;
		const VkCommandPoolCreateInfo commandPoolInfo{
			VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			/*pNext =*/nullptr,
			VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
			gpu.GetQueueFamilyByConcept(QueueConcept::Transfer)
		};
		ET_FAIL_UNLESS(vkCreateCommandPool(gpu, &commandPoolInfo, gpu.GetAllocationCallbacks(), &commandPool));
		ET_AT_SCOPE_EXIT(vkDestroyCommandPool(gpu, commandPool, gpu.GetAllocationCallbacks()));

		VkCommandBuffer                   commands[_countof(_commands)];
		const VkCommandBufferAllocateInfo commandsInfo{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			/*pNext =*/nullptr,
			commandPool,
			VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			_countof(commands)
		};
		ET_FAIL_UNLESS(vkAllocateCommandBuffers(gpu, &commandsInfo, commands));
		ET_AT_SCOPE_EXIT(if (commandPool) vkFreeCommandBuffers(gpu, commandPool, _countof(commands), commands));

		VkSemaphore phaseCompleted[_countof(_phaseCompleted)];
		ET_AT_SCOPE_EXIT(for (VkSemaphore semaphore
							  : phaseCompleted) vkDestroySemaphore(gpu, semaphore, gpu.GetAllocationCallbacks()););
		for (VkSemaphore& semaphore : phaseCompleted) {
			const VkSemaphoreCreateInfo phaseCompleteInfo{
				VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
				/*pNext =*/nullptr,
				/*flags =*/0u
			};
			ET_FAIL_UNLESS(vkCreateSemaphore(gpu, &phaseCompleteInfo, gpu.GetAllocationCallbacks(), &semaphore));
		}
		//	Swap created resources into the object.
		Swap(_stagingBuffer, stagingBuffer);
		Swap(_phaseCompleted, phaseCompleted);
		Swap(_commandsConsumed, commandsConsumed);
		Swap(_commandPool, commandPool);
		Swap(_commands, commands);

		return VK_SUCCESS;
	}

	// ---------------------------------------------------

	void GpuResourceBus::FreeResources(Gpu& gpu) {
		for (VkSemaphore& semaphore : _phaseCompleted) {
			vkDestroySemaphore(gpu, eastl::exchange(semaphore, nullptr), gpu.GetAllocationCallbacks());
		}

		if (VkCommandPool pool = eastl::exchange(_commandPool, nullptr)) {
			vkFreeCommandBuffers(gpu, pool, _countof(_commands), _commands);
			vkDestroyCommandPool(gpu, pool, gpu.GetAllocationCallbacks());
		}

		vkDestroyFence(gpu, eastl::exchange(_commandsConsumed, nullptr), gpu.GetAllocationCallbacks());

		_stagingBuffer.FreeResources(gpu);

		ET_ASSERT(_signals.IsEmpty(), "Leaking semaphores!");

		_signals.Clear();
		_pendingImageBinds.Clear();
		_pendingImageOpaqueBinds.Clear();
	}

	// ---------------------------------------------------

	void Swap(GpuResourceBus& lhs, GpuResourceBus& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._pendingImageOpaqueBinds, rhs._pendingImageOpaqueBinds);
		Swap(lhs._pendingImageBinds, rhs._pendingImageBinds);
		Swap(lhs._signals, rhs._signals);
		Swap(lhs._stagingBuffer, rhs._stagingBuffer);

		//	Swap read/write offsets.
		VkDeviceSize readOffset(rhs._readOffset.exchange(lhs._readOffset.load(std::memory_order_relaxed), std::memory_order_relaxed));
		VkDeviceSize writeOffset(rhs._writeOffset.exchange(lhs._writeOffset.load(std::memory_order_relaxed), std::memory_order_relaxed));

		lhs._readOffset.store(readOffset, std::memory_order_relaxed);
		rhs._writeOffset.store(writeOffset, std::memory_order_relaxed);

		Swap(lhs._phaseCompleted, rhs._phaseCompleted);
		Swap(lhs._commandsConsumed, rhs._commandsConsumed);
		Swap(lhs._commandPool, rhs._commandPool);
		Swap(lhs._commands, rhs._commands);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
