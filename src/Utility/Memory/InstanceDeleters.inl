/*==================================================================*\
  InstanceDeleters.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/Allocator.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	ETInlineHint InstanceDeleter::InstanceDeleter( ::Eldritch2::Allocator& allocator ) : _allocator( allocator ) {}

// ---------------------------------------------------

	template<class T>
	ETInlineHint void InstanceDeleter::operator()( T* const pointer ) const {
		InstanceDeleter::operator()( *pointer );
	}

// ---------------------------------------------------

	template<class T>
	ETInlineHint void InstanceDeleter::operator()( T& object ) const {
		static_assert( 0 < sizeof(T), "Can't delete an incomplete type! Are you missing an #include?" );
		_allocator.Delete( object );
	}

// ---------------------------------------------------

	ETInlineHint AlignedInstanceDeleter::AlignedInstanceDeleter( ::Eldritch2::Allocator& allocator ) : InstanceDeleter( allocator ) {}

// ---------------------------------------------------

	template<class T>
	ETInlineHint void AlignedInstanceDeleter::operator()( T* const object ) const {
		AlignedInstanceDeleter::operator()( *object );
	}

// ---------------------------------------------------

	template<class T>
	ETInlineHint void AlignedInstanceDeleter::operator()( T& object ) const {
		static_assert( 0 < sizeof(T), "Can't delete an incomplete type! Are you missing an #include?" );
		_allocator.Delete( object, ::Eldritch2::AlignedDeallocationSemantics );
	}

}	// namespace Utility
}	// namespace Eldritch2