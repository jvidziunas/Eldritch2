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
#include <Packages/DeserializationContext.hpp>
#include <Packages/ContentPackage.hpp>
#include <Packages/ContentLibrary.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace FileSystem {

	ContentPackage::ContentPackage( const UTF8Char* const name, ContentLibrary& owningLibrary, Allocator& allocator ) : ContentPackage( owningLibrary, allocator ) {
		_name.Assign( name );
	}

// ---------------------------------------------------

	ContentPackage::ContentPackage( ContentLibrary& owningLibrary, Allocator& allocator ) : _allocator( allocator, UTF8L("Package Resource View Allocator") ),
																							_owningLibrary( owningLibrary ),
																							_name( ::Eldritch2::EmptyStringSemantics, allocator, UTF8L("Package Name Allocator") ),
																							_referencedPackages( 0u, allocator, UTF8L("Package Dependency Collection Allocator") ) {}

// ---------------------------------------------------

	ContentPackage::~ContentPackage() {
		DeleteContent();

		DeserializationContext::UnpublishPackage( *this );
	}

// ---------------------------------------------------

	void ContentPackage::Dispose() {
		_owningLibrary._allocator.Delete( *this );
	}

// ---------------------------------------------------

	void ContentPackage::DeleteContent() {
		auto&	allocator( _allocator );

		// It's not necessary to maintain a lock on the content table while we delete resources
		// (they've already been unpublished, we're the only thing that can see them)
		_exportedResources.ClearAndDispose( [&allocator] ( ResourceView& currentResource ) {
			allocator.Delete( currentResource );
		} );
	}

}	// namespace FileSystem
}	// namespace Eldritch2