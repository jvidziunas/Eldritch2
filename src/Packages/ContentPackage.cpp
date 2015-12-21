/*==================================================================*\
  ContentPackage.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <FileSystem/ContentProvider.hpp>
#include <Packages/ContentPackage.hpp>
#include <Packages/ContentLibrary.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace FileSystem {

	ContentPackage::ContentPackage( const UTF8Char* const name, ContentLibrary& owningLibrary, Allocator& allocator ) : ReferenceTypeBase( 0u ),
																														_allocator( allocator, UTF8L("Package Allocator") ),
																														_name( name, name + StringLength( name ), { allocator, UTF8L("Package Name Allocator") } ),
																														_owningLibrary( owningLibrary ),
																														_residencyState( ResidencyState::LOADING ),
																														_referencedPackages( { allocator, UTF8L("Package Dependency Collection Allocator") } ) {}

// ---------------------------------------------------

	ContentPackage::~ContentPackage() {
		auto&	globalContentLibrary( GetContentLibrary() );
		auto&	allocator( GetAllocator() );
		
		// Erase all the views from the shared library.
		{	ScopedLock	_( *globalContentLibrary._resourceViewDirectoryMutex );
			// Caching.
			auto&	globalResourceViewLibrary( globalContentLibrary._resourceViewDirectory );

			for( const auto& view : GetExports() ) {
				const auto findViewResult( globalResourceViewLibrary.Find( { view } ) );

				// Ensure we're not accidentally erasing another resource.
				if( findViewResult != globalResourceViewLibrary.End() && findViewResult->second == &view ) {
					globalResourceViewLibrary.Erase( findViewResult );
				}
			}
		}

		// It's not necessary to maintain a lock on the content table while we delete resources
		// (they've already been unpublished, we're the only thing that can see them)
		GetExports().ClearAndDispose( [&allocator] ( ResourceView& currentResource ) {
			allocator.Delete( currentResource );
		} );

		// Remove the package from the shared library.
		{	ScopedLock	_( *globalContentLibrary._contentPackageDirectoryMutex );
			globalContentLibrary._contentPackageDirectory.Erase( GetName() );
		}
	}

}	// namespace FileSystem
}	// namespace Eldritch2