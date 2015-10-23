/*==================================================================*\
  RDESTLAllocatorAdapterMixin.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <type_traits>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Detail {

	template <typename PublicAllocatorType>
	ETForceInlineHint RDESTLAllocatorAdapterMixin<PublicAllocatorType>::RDESTLAllocatorAdapterMixin( Detail::RDESTLAllocatorAdapterMixin<PublicAllocatorType>&& allocator ) : RDESTLAllocatorAdapterMixin<PublicAllocatorType>( static_cast<PublicAllocatorType&&>( allocator ) ) {}

// ---------------------------------------------------

	template <typename PublicAllocatorType>
	ETForceInlineHint RDESTLAllocatorAdapterMixin<PublicAllocatorType>::RDESTLAllocatorAdapterMixin( PublicAllocatorType&& allocator ) : PublicAllocatorType( ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <typename PublicAllocatorType>
	ETForceInlineHint void* RDESTLAllocatorAdapterMixin<PublicAllocatorType>::allocate( unsigned int bytes, int /*flags*/ ) {
		return this->allocate_aligned( bytes, sizeof(void*) );
	}

// ---------------------------------------------------

	template <typename PublicAllocatorType>
	ETForceInlineHint void* RDESTLAllocatorAdapterMixin<PublicAllocatorType>::allocate_aligned( unsigned int bytes, unsigned int alignment, int /*flags*/ ) {
		return this->Allocate( bytes, alignment, AllocationOption::TEMPORARY_ALLOCATION );
	}

// ---------------------------------------------------

	template <typename PublicAllocatorType>
	ETForceInlineHint void RDESTLAllocatorAdapterMixin<PublicAllocatorType>::deallocate( void* ptr, unsigned int /*bytes*/ ) {
		this->Deallocate( ptr, ::Eldritch2::AlignedDeallocationSemantics );
	}

// ---------------------------------------------------

	template <typename PublicAllocatorType>
	ETForceInlineHint const char* RDESTLAllocatorAdapterMixin<PublicAllocatorType>::get_name() const {
		return this->GetName();
	}

}	// namespace Detail
}	// namespace Eldritch2