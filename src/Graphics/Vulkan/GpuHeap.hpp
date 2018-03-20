/*==================================================================*\
  GpuHeap.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <vk_mem_alloc.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Graphics {
		namespace Vulkan {
			class	Gpu;
		}
	}
}

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	class GpuHeap {
	// - TYPE PUBLISHING ---------------------------------

	public:
		template <typename Resource>
		using GarbageList	= ArrayList<Pair<Resource, VmaAllocation>>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		GpuHeap( const GpuHeap& ) = delete;
	//!	Constructs this @ref GpuHeap instance.
		GpuHeap( GpuHeap&& ) ETNoexceptHint;
	//!	Constructs this @ref GpuHeap instance.
		GpuHeap();

		~GpuHeap();

	// ---------------------------------------------------

	public:
		operator VmaAllocator();

	// ---------------------------------------------------

	public:
		void	AddGarbage( GarbageList<VkBuffer>::ValueType garbage );
		void	AddGarbage( GarbageList<VkImage>::ValueType garbage );

		void	DestroyGarbage();

	// ---------------------------------------------------

	public:
		VkResult	BindResources( Gpu& gpu, VkDeviceSize heapBlockSize );

		void		FreeResources( Gpu& gpu );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		GpuHeap&	operator=( const GpuHeap& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		mutable Mutex			_allocatorMutex;
		VmaAllocator			_allocator;

		mutable Mutex			_garbageMutex;
		GarbageList<VkImage>	_imageGarbage;
		GarbageList<VkBuffer>	_bufferGarbage;

	// ---------------------------------------------------

		friend void	Swap( GpuHeap&, GpuHeap& );
	};

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/GpuHeap.inl>
//------------------------------------------------------------------//