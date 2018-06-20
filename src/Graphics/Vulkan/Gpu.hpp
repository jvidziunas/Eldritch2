/*==================================================================*\
  Gpu.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  �2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/HostMixin.hpp>
//------------------------------------------------------------------//
#include <vulkan/vulkan_core.h>
#include <vk_mem_alloc.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	enum QueueConcept : uint32 {
		Drawing,
		BulkComputation,
		Presentation,
		SparseBinding,
		Transfer,

		COUNT
	};

	// ---

	class Gpu {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using HostAllocator = HostMixin<UsageMixin<MallocAllocator>>;

		// ---

	public:
		class CommandQueue {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Disable copy construction.
			CommandQueue(const CommandQueue&) = delete;
			//!	Constructs this @ref CommandQueue instance.
			CommandQueue();

			~CommandQueue() = default;

			// ---------------------------------------------------

		public:
			void BindResources(VkDevice device, uint32_t family);

			// - DATA MEMBERS ------------------------------------

		public:
			std::atomic_flag lock;
			uint32_t         family;
			VkQueue          queue;
		};

	public:
		union QueueIndices {
			uint8_t byConcept[QueueConcept::COUNT];
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
		VkResult SubmitAsync(QueueConcept target, VkFence commandsConsumed, uint32_t submitCount, const VkSubmitInfo* begin);
		template <uint32_t count>
		VkResult SubmitAsync(QueueConcept target, VkFence commandsConsumed, const VkSubmitInfo (&submits)[count]);

		VkResult BindAsync(QueueConcept target, VkFence commandsConsumed, uint32_t bindCount, const VkBindSparseInfo* begin);
		template <uint32_t count>
		VkResult BindAsync(QueueConcept target, VkFence commandsConsumed, const VkBindSparseInfo (&binds)[count]);

#if VK_KHR_swapchain
		VkResult PresentAsync(QueueConcept target, const VkPresentInfoKHR& submit);
#endif // VK_KHR_swapchain

		// ---------------------------------------------------

	public:
		void AddGarbage(VkBuffer buffer, VmaAllocation backing);
		void AddGarbage(VkImage image, VmaAllocation backing);
		void AddGarbage(VmaAllocation allocation);
		void AddGarbage(VkBuffer buffer);
		void AddGarbage(VkImage image);

		void DestroyGarbage();

		// ---------------------------------------------------

	public:
		const VkAllocationCallbacks* GetAllocationCallbacks() const;

		VkPipelineCache GetPipelineCache();

		operator VkPhysicalDevice();

		operator VmaAllocator();

		operator VkDevice();

		// ---------------------------------------------------

	public:
		uint32_t GetQueueFamilyByConcept(QueueConcept concept) const;

		bool SharesQueues(QueueConcept first, QueueConcept second) const;

		// ---------------------------------------------------

	public:
		VkResult BindResources(VkPhysicalDevice physicalDevice, VkDeviceSize heapBlockSize, uint32 frameUseCount);

		void FreeResources();

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Gpu& operator=(const Gpu&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		mutable HostAllocator    _allocator;
		VkPhysicalDevice         _physicalDevice;
		VkDevice                 _device;
		VkPipelineCache          _pipelineCache;
		QueueIndices             _indices;
		CommandQueue             _queues[QueueConcept::COUNT];
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
