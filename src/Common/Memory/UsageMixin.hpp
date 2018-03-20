/*==================================================================*\
  UsageMixin.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Memory/Allocator.hpp>
#include <Common/Mpl/Compiler.hpp>
//------------------------------------------------------------------//
#include <EASTL/type_traits.h>
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <class BackingAllocator>
	class UsageMixin : public BackingAllocator {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using SizeType = typename BackingAllocator::SizeType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	//!	Constructs this @ref TrackingAllocatorMixin instance.
		template <class = eastl::enable_if<eastl::is_copy_constructible<BackingAllocator>::value>>
		UsageMixin( const UsageMixin& );
	//!	Constructs this @ref TrackingAllocatorMixin instance.
		template <typename... Arguments>
		UsageMixin( Arguments&&... args );

		~UsageMixin() = default;

	// - MEMORY ALLOCATION/DEALLOCATION ------------------

	public:
		ETRestrictHint void*	Allocate( SizeType sizeInBytes, SizeType alignmentInBytes, SizeType offsetInBytes, AllocationDuration duration = AllocationDuration::Normal ) override;
		ETRestrictHint void*	Allocate( SizeType sizeInBytes, AllocationDuration duration = AllocationDuration::Normal ) override;

		void					Deallocate( void* const address, typename BackingAllocator::SizeType sizeInBytes ) override;

	// ---------------------------------------------------

	public:
		typename BackingAllocator::SizeType	GetPeakUsageInBytes() const;

	// ---------------------------------------------------

	public:
		void	Swap( UsageMixin& allocator );

	// - DATA MEMBERS ------------------------------------

	private:
	//!	Amount of memory currently being used from this allocator, specified in bytes.
	/*!	@remarks Updated during each call to @ref Allocate(). */
		std::atomic<SizeType>	_usage;
	//!	Peak amount of memory used from this allocator, specified in bytes.
	/*!	@remarks Updated during each call to @ref Allocate(). */
		std::atomic<SizeType>	_peakUsage;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Memory/UsageMixin.inl>
//------------------------------------------------------------------//