/*==================================================================*\
  DeviceMemoryPool.inl
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
namespace Renderer {
namespace Vulkan {

	ETInlineHint DeviceMemoryPool::Allocation::Allocation( Eldritch2::IdentifierPool<VkDeviceSize>::IdentifierRange range, DeviceMemoryPool& pool ) : _range( range ), _pool( &pool ) {}

// ---------------------------------------------------

	ETInlineHint DeviceMemoryPool::Allocation::Allocation( Allocation&& other ) : _range( other._range ), _pool( other._pool ) {
	//	Clear the source allocation out to avoid double-free.
		other._pool						= nullptr;
		other._range.firstIdentifier	= 0u;
		other._range.lastIdentifier		= 0u;
	}

// ---------------------------------------------------

	ETInlineHint DeviceMemoryPool::Allocation::~Allocation() {
		if( !_pool ) {
			return;
		}

		_pool->_availableRegions.ReleaseRange( _range );
	}

// ---------------------------------------------------

	ETInlineHint DeviceMemoryPool::Allocation& DeviceMemoryPool::Allocation::operator=( Allocation&& other ) {
		if( _pool ) {
			_pool->_availableRegions.ReleaseRange( _range );
		}

		_range	= other._range;
		_pool	= other._pool;

	//	Clear the source allocation out to avoid double-free.
		other._pool						= nullptr;
		other._range.firstIdentifier	= 0u;
		other._range.lastIdentifier		= 0u;

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint VkDeviceSize DeviceMemoryPool::Allocation::GetOffsetIntoPoolInBytes() const {
		return _range.firstIdentifier;
	}

// ---------------------------------------------------

	ETInlineHint VkDeviceSize DeviceMemoryPool::Allocation::GetSizeInBytes() const {
		return _range.GetSize();
	}

// ---------------------------------------------------

	ETInlineHint DeviceMemoryPool::DeviceMemoryPool( Vulkan::UniquePointer<VkDeviceMemory> deviceMemory, VkDeviceSize memorySizeInBytes, Eldritch2::Allocator& allocator ) : _deviceMemory( eastl::move( deviceMemory ) ), _availableRegions( memorySizeInBytes, { allocator, "Vulkan Memory Pool Available Region Allocator" } ) {}

// ---------------------------------------------------

	ETInlineHint DeviceMemoryPool::DeviceMemoryPool( DeviceMemoryPool&& other ) : _deviceMemory( eastl::move( other._deviceMemory ) ), _availableRegions( eastl::move( other._availableRegions ) ) {}

// ---------------------------------------------------

	ETInlineHint VkDeviceMemory	DeviceMemoryPool::GetDeviceMemory() const {
		return _deviceMemory.Get();
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

