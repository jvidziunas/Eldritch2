/*==================================================================*\
  ResourceView.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Packages/ContentLibrary.hpp>
#include <Packages/ContentPackage.hpp>
#include <Packages/ResourceView.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace FileSystem {

	const ResourceView& ResourceView::PublishToLibrary( ContentLibrary& owningLibrary, const ::std::type_info& publicType ) const {
		ScopedLock	_( *owningLibrary._resourceViewDirectoryMutex );

		owningLibrary._resourceViewDirectory.Insert( { { GetName(), &publicType }, static_cast<const void*>(this) } );

		return *this;
	}

// ---------------------------------------------------

	const ResourceView& ResourceView::RemoveFromLibrary( ContentLibrary& owningLibrary, const ::std::type_info& publicType ) const {
		ScopedLock	_( *owningLibrary._resourceViewDirectoryMutex );
		auto&		directory( owningLibrary._resourceViewDirectory );
		const auto	candidate( directory.Find( { GetName(), &publicType } ) );

		// Remove ourselves from the library ONLY if the stored entry actually refers to us.
		if( candidate != directory.End() && candidate->second == this ) {
			directory.Erase( candidate );
		}
		return *this;
	}

}	// namespace FileSystem
}	// namespace Eldritch2