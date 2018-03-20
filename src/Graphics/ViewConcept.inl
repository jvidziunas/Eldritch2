/*==================================================================*\
  ViewConcept.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {

	template <typename Value, class Allocator>
	ETInlineHint ViewConcept<Value, Allocator>::ViewConcept( const AllocatorType& allocator ) : _views( allocator ) {}

// ---------------------------------------------------

	template <typename Value, class Allocator>
	ETInlineHint typename ViewConcept<Value, Allocator>::ConstIterator ViewConcept<Value, Allocator>::Begin() const {
		return _views.Begin();
	}

// ---------------------------------------------------

	template <typename Value, class Allocator>
	ETInlineHint typename ViewConcept<Value, Allocator>::ConstIterator ViewConcept<Value, Allocator>::End() const {
		return _views.End();
	}

}	// namespace Graphics
}	// namespace Eldritch2