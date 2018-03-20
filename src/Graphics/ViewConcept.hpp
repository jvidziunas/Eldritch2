/*==================================================================*\
  ViewConcept.hpp
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
	class ViewConcept {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using ConstIterator	= typename ArrayList<const Value*, Allocator>::ConstIterator;
		using AllocatorType = Allocator;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ViewConcept instance.
		ViewConcept( const AllocatorType& allocator );
	//!	Disable copy construction.
		ViewConcept( const ViewConcept& ) = delete;

		~ViewConcept() = default;

	// ---------------------------------------------------

	public:
		ConstIterator	Begin() const;

		ConstIterator	End() const;

	// ---------------------------------------------------

	//!	Disable copy assignment.
		ViewConcept&	operator=( const ViewConcept& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		mutable Mutex							_viewsMutex;
		ArrayList<const Value*, AllocatorType>	_views;
	};

}	// namespace Graphics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/ViewConcept.inl>
//------------------------------------------------------------------//