/*==================================================================*\
  ResourceView.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Packages/ContentPackage.hpp>
#include <Packages/ResourceView.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2;
using namespace ::std;

namespace Eldritch2 {
namespace FileSystem {

	ResourceView::ResourceView( const Initializer& initializer, Allocator& nameAllocator ) : _name( initializer.name.first, initializer.name.Size(), nameAllocator, UTF8L("Package Name Allocator") ) {}

}	// namespace FileSystem
}	// namespace Eldritch2