/*==================================================================*\
  ResourceView.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <typeinfo>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace FileSystem {

	ETInlineHint ResourceView::ResourceView( const UTF8Char* const name ) : _name( name ) {}

// ---------------------------------------------------

	ETInlineHint const ::Eldritch2::UTF8Char* const ResourceView::GetName() const {
		return _name;
	}

// ---------------------------------------------------

	template <typename PublicType>
	const ResourceView& ResourceView::PublishToLibraryAs( ContentLibrary& owningLibrary ) const {
		return PublishToLibrary( owningLibrary, typeid(PublicType) );
	}

// ---------------------------------------------------

	template <typename PublicType>
	const ResourceView& ResourceView::RemoveFromLibraryAs( ContentLibrary& owningLibrary ) const {
		return RemoveFromLibrary( owningLibrary, typeid(PublicType) );
	}

}	// namespace FileSystem
}	// namespace Eldritch2