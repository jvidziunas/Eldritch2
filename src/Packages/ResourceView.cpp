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
#include <Utility/Memory/StandardLibrary.hpp>
#include <Utility/Concurrency/Lock.hpp>
#include <Packages/ContentLibrary.hpp>
#include <Packages/ContentPackage.hpp>
#include <Packages/ResourceView.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace FileSystem {

	ResourceView::ResourceView( ContentLibrary& owningLibrary, ContentPackage& package, const UTF8Char* const name, Allocator& nameAllocator ) : _name( name, FindEndOfString( name ), { nameAllocator, UTF8L("Resource View Name Allocator") } ) {
		package.GetExports().PushFront( *this );

		{	ScopedLock	_( *owningLibrary._resourceViewDirectoryMutex );
			owningLibrary._resourceViewDirectory.Insert( { { *this }, this } );
		}	// End of lock scope.
	}

}	// namespace FileSystem
}	// namespace Eldritch2