/*==================================================================*\
  EaStlAllocatorAdapterMixin.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <eastl/utility.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Detail {

	template <typename EldritchAllocator>
	ETInlineHint EaStlAllocatorAdapterMixin<EldritchAllocator>::EaStlAllocatorAdapterMixin( EaStlAllocatorAdapterMixin<EldritchAllocator>&& allocator ) : EldritchAllocator( eastl::forward<EldritchAllocator>( allocator ) ) {}

// ---------------------------------------------------

	template <typename EldritchAllocator>
	ETInlineHint EaStlAllocatorAdapterMixin<EldritchAllocator>::EaStlAllocatorAdapterMixin( const PublicType& allocator ) : EldritchAllocator( allocator ) {}

// ---------------------------------------------------

	template <typename EldritchAllocator>
	ETInlineHint EaStlAllocatorAdapterMixin<EldritchAllocator>::EaStlAllocatorAdapterMixin( PublicType&& allocator ) : EldritchAllocator( eastl::forward<EldritchAllocator>( allocator ) ) {}

// ---------------------------------------------------

	template <typename EldritchAllocator>
	ETInlineHint ETRestrictHint void* EaStlAllocatorAdapterMixin<EldritchAllocator>::allocate( size_t allocationSizeInBytes, size_t alignmentInBytes, size_t offsetInBytes, int /*flags*/ ) {
		return EldritchAllocator::Allocate( allocationSizeInBytes, alignmentInBytes, offsetInBytes );
	}

// ---------------------------------------------------

	template <typename EldritchAllocator>
	ETInlineHint ETRestrictHint void* EaStlAllocatorAdapterMixin<EldritchAllocator>::allocate( size_t allocationSizeInBytes, int /*flags*/ ) {
		return EldritchAllocator::Allocate( allocationSizeInBytes );
	}

// ---------------------------------------------------

	template <typename EldritchAllocator>
	ETInlineHint void EaStlAllocatorAdapterMixin<EldritchAllocator>::deallocate( void* pointer, size_t allocationSizeInBytes ) {
		EldritchAllocator::Deallocate( pointer, allocationSizeInBytes );
	}

// ---------------------------------------------------

	template <typename EldritchAllocator>
	ETInlineHint const char* EaStlAllocatorAdapterMixin<EldritchAllocator>::get_name() const {
		return EldritchAllocator::GetName();
	}

// ---------------------------------------------------

	template <typename EldritchAllocator>
	ETInlineHint void EaStlAllocatorAdapterMixin<EldritchAllocator>::set_name( const char* name ) {
		EldritchAllocator::SetName( name );
	}

// ---------------------------------------------------

	template <typename EldritchAllocator>
	ETInlineHint bool operator==( const EaStlAllocatorAdapterMixin<EldritchAllocator>& /*operand0*/, const EaStlAllocatorAdapterMixin<EldritchAllocator>& /*operand1*/ ) {
		return false;
	}

}	// namespace Detail
}	// namespace Eldritch2

namespace eastl {

	template <typename T>
	ETInlineHint void swap( Eldritch2::Detail::EaStlAllocatorAdapterMixin<T>& a, Eldritch2::Detail::EaStlAllocatorAdapterMixin<T>& b ) {
		a.Swap( b );
	}

}	// namespace eastl