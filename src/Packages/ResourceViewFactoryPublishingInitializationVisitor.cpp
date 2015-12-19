/*==================================================================*\
  ResourceViewFactoryPublishingInitializationVisitor.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Packages/ResourceViewFactoryPublishingInitializationVisitor.hpp>
#include <Packages/ContentLibrary.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace FileSystem {

	ResourceViewFactoryPublishingInitializationVisitor::ResourceViewFactoryPublishingInitializationVisitor( ContentLibrary& contentLibrary ) : _contentLibrary( contentLibrary ) {}

// ---------------------------------------------------

	ResourceViewFactoryPublishingInitializationVisitor& ResourceViewFactoryPublishingInitializationVisitor::PublishFactory( const UTF8Char* className, ResourceViewFactory& factory ) {
		_contentLibrary._resourceFactoryDirectory[::std::move( className )].PushFront( factory );

		return *this;
	}

}	// namespace FileSystem
}	// namespace Eldritch2
