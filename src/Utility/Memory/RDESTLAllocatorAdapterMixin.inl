/*==================================================================*\
  RDESTLAllocatorAdapterMixin.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <type_traits>
//------------------------------------------------------------------//

namespace Eldritch2Detail {

	template <typename PublicAllocatorType>
	template <typename... AllocatorConstructorArguments>
	ETForceInlineHint RDESTLAllocatorAdapterMixin<PublicAllocatorType>::RDESTLAllocatorAdapterMixin( AllocatorConstructorArguments&&... allocatorConstructorArguments ) : PublicAllocatorType( ::std::forward<AllocatorConstructorArguments>( allocatorConstructorArguments )... ) {}

// ---------------------------------------------------

	template <typename PublicAllocatorType>
	ETForceInlineHint RDESTLAllocatorAdapterMixin<PublicAllocatorType>::RDESTLAllocatorAdapterMixin( ::Eldritch2Detail::RDESTLAllocatorAdapterMixin<PublicAllocatorType>&& allocator ) : PublicAllocatorType( ::std::move( allocator ) ) {}

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

}	// namespace Eldritch2Detail