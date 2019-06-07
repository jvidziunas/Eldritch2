/*==================================================================*\
  Gpu.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/HostMixin.hpp>
//------------------------------------------------------------------//
#include <vk_mem_alloc.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	enum QueueClass : uint32 {
		Drawing,
		BulkComputation,
		Presentation,
		SparseBinding,
		Transfer,

		QueueClasses
	};

	// ---

	enum PoolClass : uint32 {
		ShaderResources,
		FramebufferAttachments,
		StagingBuffers,

		PoolClasses
	};

	// ---

	class Gpu {
		// - TYPE PUBLISHING ---------------------------------

	public:
		class CommandQueue {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Disable copy construction.
			CommandQueue(const CommandQueue&) = delete;
			//!	Constructs this @ref CommandQueue instance.
			CommandQueue() ETNoexceptHint;

			~CommandQueue() = default;

			// ---------------------------------------------------

		public:
			ETConstexpr bool IsIndependent(const CommandQueue&) const ETNoexceptHint;

			ETConstexpr uint32 GetFamily() const ETNoexceptHint;

			// ---------------------------------------------------

		public:
			VkResult Submit(VkFence consumed, uint32_t submitCount, const VkSubmitInfo submits[]) ETNoexceptHint;

			VkResult Bind(VkFence consumed, uint32_t bindCount, const VkBindSparseInfo binds[]) ETNoexceptHint;

#if VK_KHR_swapchain
			VkResult Present(const VkPresentInfoKHR& present) ETNoexceptHint;
#endif // VK_KHR_swapchain

			// ---------------------------------------------------

		public:
			void BindResources(VkDevice device, uint32_t family);

			// - DATA MEMBERS ------------------------------------

		private:
			ETCacheLineAligned Mutex _mutex;
			VkQueue                  _queue;
			uint32_t                 _family;
		};

		// ---

	public:
		template <typename... Args>
		struct ETPureAbstractHint Finalizer : public IntrusiveForwardListBaseHook {
			virtual void operator()(Args... args) ETNoexceptHint abstract;
		};

		// ---

	public:
		union QueueIndices {
			uint8_t byConcept[QueueClasses];
		};

		// ---

	public:
		template <typename Allocator>
		using AllocatorType = HostMixin<UsageMixin<Allocator>>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		Gpu(const Gpu&) = delete;
		//!	Constructs this @ref Gpu instance.
		Gpu() ETNoexceptHint;

		~Gpu();

		// ---------------------------------------------------

	public:
		ETConstexpr const CommandQueue& GetQueue(QueueClass queue) const ETNoexceptHint;
		ETConstexpr CommandQueue& GetQueue(QueueClass queue) ETNoexceptHint;

		uint32 GetFrameDeviceMask() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		template <typename HostType>
		VkResult Map(HostType*& outBase, VmaAllocation allocation) ETNoexceptHint;

		void Unmap(VmaAllocation allocation) ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult AllocateMemory(VmaAllocation& allocation, const VmaAllocationCreateInfo& createInfo, VkBuffer buffer);
		VkResult AllocateMemory(VmaAllocation& allocation, const VmaAllocationCreateInfo& createInfo, VkImage image);

		void DeallocateMemory(VmaAllocation allocation);

		// ---------------------------------------------------

	public:
		VkResult GetTemporarySemaphore(VkSemaphore& outSemaphore);

		// ---------------------------------------------------

	public:
		template <typename BinaryPredicate>
		void Finalize(PoolClass pool, BinaryPredicate finalizer) ETNoexceptHint;
		template <typename UnaryPredicate>
		void Finalize(UnaryPredicate finalizer) ETNoexceptHint;

		void DestroyGarbage() ETNoexceptHint;

		// ---------------------------------------------------

	public:
		ETConstexpr const VkAllocationCallbacks* GetAllocationCallbacks() const ETNoexceptHint;

		ETConstexpr operator VkPhysicalDevice() ETNoexceptHint;

		ETConstexpr operator VkInstance() ETNoexceptHint;

		ETConstexpr operator VkDevice() ETNoexceptHint;

		// ---------------------------------------------------

	public:
		ETConstexpr uint32_t GetQueueFamilyByClass(QueueClass concept) const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult BindResources(VkInstance vulkan, VkPhysicalDevice physicalDevice, VkDeviceSize heapBlockSize, uint32 frameUseCount);

		void FreeResources() ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Gpu& operator=(const Gpu&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		mutable AllocatorType<MallocAllocator>            _allocator;
		VkInstance                                        _vulkan;
		VkPhysicalDevice                                  _physicalDevice;
		VkDevice                                          _device;
		QueueIndices                                      _indices;
		CommandQueue                                      _queues[QueueClasses];
		ETCacheLineAligned mutable Mutex                  _gpuAllocatorMutex;
		VmaAllocator                                      _gpuAllocator;
		ArenaChildAllocator                               _finalizerArena;
		ArenaChildAllocator::Checkpoint                   _frameStartCheckpoint;
		IntrusiveMpscQueue<Finalizer<Gpu&>>               _finalizers;
		IntrusiveMpscQueue<Finalizer<Gpu&, VmaAllocator>> _allocationFinalizers;
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/Gpu.inl>
//------------------------------------------------------------------//
