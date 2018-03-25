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
#include <vulkan/vulkan_core.h>
//------------------------------------------------------------------//
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

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
		class CommandQueue {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Disable copy construction.
			CommandQueue( const CommandQueue& ) = delete;
		//!	Constructs this @ref CommandQueue instance.
			CommandQueue();

			~CommandQueue() = default;

		// ---------------------------------------------------

		public:
			void	BindResources( VkDevice device, uint32_t family );

		// - DATA MEMBERS ------------------------------------

		public:
			std::atomic_flag	lock;
			uint32_t			familyIndex;
			VkQueue				queue;
		};

	// ---

	public:
		struct QueueIndices {
			uint8_t	byConcept[QueueConcept::COUNT];
		};

	// ---

	public:
		using HostAllocator = HostMixin<UsageMixin<MallocAllocator>>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		Gpu( const Gpu& ) = delete;
	//!	Constructs this @ref Gpu instance.
		Gpu();

		~Gpu();

	// ---------------------------------------------------

	public:
		VkResult	SubmitAsync( QueueConcept target, VkFence commandsConsumed, uint32_t submitCount, const VkSubmitInfo* begin );
		template <uint32_t submitCount>
		VkResult	SubmitAsync( QueueConcept target, VkFence commandsConsumed, const VkSubmitInfo (&submits)[submitCount] );

		VkResult	BindAsync( QueueConcept target, VkFence commandsConsumed, uint32_t bindCount, const VkBindSparseInfo* begin );
		template <uint32_t bindCount>
		VkResult	BindAsync( QueueConcept target, VkFence commandsConsumed, const VkBindSparseInfo (&binds)[bindCount] );

#	if VK_KHR_swapchain
		VkResult	PresentAsync( QueueConcept target, const VkPresentInfoKHR& submit );
#	endif	// VK_KHR_swapchain

	// ---------------------------------------------------

	public:
		const VkAllocationCallbacks*	GetAllocationCallbacks() const;

		VkPipelineCache					GetPipelineCache();

		operator						VkPhysicalDevice();

		operator						VkDevice();

	// ---------------------------------------------------

	public:
		uint32_t	GetQueueFamilyByConcept( QueueConcept concept ) const;

		bool		SharesQueues( QueueConcept first, QueueConcept second ) const;

	// ---------------------------------------------------

	public:
		VkResult	BindResources( VkPhysicalDevice physicalDevice );

		void		FreeResources();

	// ---------------------------------------------------

	//!	Disable copy assignment.
		Gpu&	operator=( const Gpu& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		mutable HostAllocator	_allocator;

		VkPhysicalDevice		_physicalDevice;
		VkDevice				_device;
		VkPipelineCache			_pipelineCache;

		QueueIndices			_indices;
		CommandQueue			_queues[QueueConcept::COUNT];
	};

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/Gpu.inl>
//------------------------------------------------------------------//