/*==================================================================*\
  DeviceMemoryPool.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/SmartPointers.hpp>
#include <Utility/IdentifierPool.hpp>
//------------------------------------------------------------------//
#include <vulkan/vulkan.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	template <typename>
	class	Result;
}

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	class DeviceMemoryPool {
	// - TYPE PUBLISHING ---------------------------------

	public:
	/*!	Utility wrapper for managing sub-allocations within the memory pool. Carries similar ownership transfer semantics
	 *	to the more traditional UniquePointer/unique_ptr family of smart pointers. */
		class Allocation {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref Allocation instance.
		/*!	@param[in] range Region of the memory pool this allocation covers.
			@param[in] pool Memory pool the region was allocated from/will be returned to. */
			Allocation( Eldritch2::IdentifierPool<VkDeviceSize>::IdentifierRange range, DeviceMemoryPool& pool );
		//!	Disable copying.
			Allocation( const Allocation& ) = delete;
		//!	Constructs this @ref Allocation instance.
			Allocation( Allocation&& );

			~Allocation();

		// ---------------------------------------------------

		public:
			VkDeviceSize	GetOffsetIntoPoolInBytes() const;

			VkDeviceSize	GetSizeInBytes() const;

		// ---------------------------------------------------

		public:
			Allocation&	operator=( const Allocation& ) = delete;
			Allocation&	operator=( Allocation&& );

		// - DATA MEMBERS ------------------------------------

		private:
			Eldritch2::IdentifierPool<VkDeviceSize>::IdentifierRange	_range;
			DeviceMemoryPool*											_pool;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref DeviceMemoryPool instance.
		DeviceMemoryPool( Vulkan::UniquePointer<VkDeviceMemory> deviceMemory, VkDeviceSize memorySizeInBytes, Eldritch2::Allocator& allocator );
	//!	Disable copying.
		DeviceMemoryPool( const DeviceMemoryPool& );
	//!	Constructs this @ref DeviceMemoryPool instance.
		DeviceMemoryPool( DeviceMemoryPool&& );

		~DeviceMemoryPool();

	// ---------------------------------------------------

	public:
		Eldritch2::Result<Allocation>	TryAllocateRegion( VkDeviceSize regionSizeInBytes );

	// ---------------------------------------------------

	public:
		VkDeviceMemory	GetDeviceMemory() const;

	// - DATA MEMBERS ------------------------------------

	private:
		Vulkan::UniquePointer<VkDeviceMemory>	_deviceMemory;
		Eldritch2::IdentifierPool<VkDeviceSize>	_availableRegions;
	};

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Renderer/Vulkan/DeviceMemoryPool.inl>
//------------------------------------------------------------------//