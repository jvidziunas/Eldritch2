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
#include <Packages/ResourceView.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace FileSystem {

	ResourceView::ResourceView( const UTF8Char* const name, Allocator& nameAllocator ) : _name( name, FindEndOfString( name ), { nameAllocator, UTF8L("Resource View Name Allocator") } ) {}

}	// namespace FileSystem
}	// namespace Eldritch2