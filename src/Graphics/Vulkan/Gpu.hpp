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

	enum QueueConcept : uint32 {
		Drawing,
		BulkComputation,
		Presentation,
		SparseBinding,
		Transfer,

		QueueConcepts
	};

	// ---

	enum PoolConcept : uint32 {
		ShaderResources,
		FramebufferAttachments,
		StagingBuffers,

		PoolConcepts
	};

	// ---

	class Gpu {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using AllocatorType = HostMixin<UsageMixin<MallocAllocator>>;

		// ---

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
			void BindResources(VkDevice device, uint32_t family);

			// - DATA MEMBERS ------------------------------------

		public:
			Mutex    mutex;
			uint32_t family;
			VkQueue  queue;
		};

		// ---

	public:
		union QueueIndices {
			uint8_t byConcept[QueueConcepts];
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		Gpu(const Gpu&) = delete;
		//!	Constructs this @ref Gpu instance.
		Gpu();

		~Gpu();

		// ---------------------------------------------------

	public:
		VkResult SubmitAsync(QueueConcept target, VkFence commandsConsumed, uint32_t submitCount, const VkSubmitInfo* begin) ETNoexceptHint;
		template <uint32_t count>
		VkResult SubmitAsync(QueueConcept target, VkFence commandsConsumed, const VkSubmitInfo (&submits)[count]) ETNoexceptHint;

		VkResult BindAsync(VkFence commandsConsumed, uint32_t bindCount, const VkBindSparseInfo* begin) ETNoexceptHint;
		template <uint32_t count>
		VkResult BindAsync(VkFence commandsConsumed, const VkBindSparseInfo (&binds)[count]) ETNoexceptHint;

#if VK_KHR_swapchain
		VkResult PresentAsync(const VkPresentInfoKHR& submit) ETNoexceptHint;
#endif // VK_KHR_swapchain

		// ---------------------------------------------------

	public:
		uint32 GetFrameAfrDeviceMask() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		template <typename HostType>
		VkResult Map(HostType*& outBase, VmaAllocation allocation) ETNoexceptHint;

		void Unmap(VmaAllocation allocation) ETNoexceptHint;

		VkResult AllocateMemory(const VmaAllocationCreateInfo& allocationInfo) ETNoexceptHint;

		void AddGarbage(VkBuffer buffer, VmaAllocation backing);
		void AddGarbage(VkImage image, VmaAllocation backing);
		void AddGarbage(VmaAllocation allocation);
		void AddGarbage(VkBuffer buffer);
		void AddGarbage(VkImage image);

		void DestroyGarbage();

		// ---------------------------------------------------

	public:
		ETConstexpr const VkAllocationCallbacks* GetAllocationCallbacks() const ETNoexceptHint;

		ETConstexpr VkPipelineCache GetPipelineCache() ETNoexceptHint;

		ETConstexpr operator VkPhysicalDevice() ETNoexceptHint;

		ETConstexpr operator VmaAllocator() ETNoexceptHint;

		ETConstexpr operator VkInstance() ETNoexceptHint;

		ETConstexpr operator VkDevice() ETNoexceptHint;

		// ---------------------------------------------------

	public:
		ETConstexpr uint32_t GetQueueFamilyByConcept(QueueConcept concept) const ETNoexceptHint;

		ETConstexpr bool RequiresSemaphore(QueueConcept first, QueueConcept second) const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult BindResources(VkInstance vulkan, VkPhysicalDevice physicalDevice, VkDeviceSize heapBlockSize, uint32 frameUseCount);

		void FreeResources();

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Gpu& operator=(const Gpu&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		mutable AllocatorType    _allocator;
		VkInstance               _vulkan;
		VkPhysicalDevice         _physicalDevice;
		VkDevice                 _device;
		VkPipelineCache          _pipelineCache;
		QueueIndices             _indices;
		CommandQueue             _queues[QueueConcepts];
		mutable Mutex            _gpuAllocatorMutex;
		VmaAllocator             _gpuAllocator;
		mutable Mutex            _garbageMutex;
		ArrayList<VkImage>       _imageGarbage;
		ArrayList<VkBuffer>      _bufferGarbage;
		ArrayList<VmaAllocation> _allocationGarbage;
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/Gpu.inl>
//------------------------------------------------------------------//
