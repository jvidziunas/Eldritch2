/*==================================================================*\
  ContentLibrary.OrphanContentNotifier.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Packages/ContentLibrary.hpp>
#include <Packages/ContentPackage.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace FileSystem {

	ContentLibrary::OrphanContentNotifier::OrphanContentNotifier( ContentProvider& contentProvider, Allocator& allocator ) : _logger( contentProvider, UTF8L("Logs") ET_UTF8_DIR_SEPARATOR UTF8L("OrphanContent.log"), allocator ) {}

// ---------------------------------------------------

	Result<ResourceView> ContentLibrary::OrphanContentNotifier::AllocateResourceView( Allocator& /*allocator*/, const UTF8Char* const name ) const {
		_logger( UTF8L("Orphan content '{}'!") ET_UTF8_NEWLINE_LITERAL, name );
		return { Error::None };
	}

}	// namespace FileSystem
}	// namespace Eldritch2

