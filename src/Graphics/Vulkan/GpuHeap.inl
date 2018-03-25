/*==================================================================*\
  GpuHeap.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	ETInlineHint GpuHeap::operator VmaAllocator() {
		return _allocator;
	}

// ---------------------------------------------------

	ETInlineHint void GpuHeap::AddGarbage( VkBuffer buffer, VmaAllocation backing ) {
		Lock	_( _garbageMutex );

		_bufferGarbage.Append( { buffer, backing } );
	}

// ---------------------------------------------------

	ETInlineHint void GpuHeap::AddGarbage( VkImage image, VmaAllocation backing ) {
		Lock	_( _garbageMutex );

		_imageGarbage.Append( { image, backing } );
	}

// ---------------------------------------------------

	ETInlineHint void Swap( GpuHeap& heap0, GpuHeap& heap1 ) {
		using ::Eldritch2::Swap;

		Swap( heap0._allocator,     heap1._allocator );
		Swap( heap0._imageGarbage,  heap1._imageGarbage );
		Swap( heap0._bufferGarbage, heap1._bufferGarbage );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2