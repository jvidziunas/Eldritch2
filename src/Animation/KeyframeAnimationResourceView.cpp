/*==================================================================*\
  KeyframeAnimationResourceView.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/KeyframeAnimationResourceView.hpp>
#include <Utility/Containers/Range.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Animation;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Animation {

	KeyframeAnimationResourceView::KeyframeAnimationResourceView( const UTF8Char* const name ) : ResourceView( name ) {}

// ---------------------------------------------------

	ErrorCode KeyframeAnimationResourceView::AttachToPackage( const Range<const char*> /*bytes*/, ContentPackage& /*package*/, ContentLibrary& library ) {
		PublishToLibraryAs<KeyframeAnimationResourceView>( library );
		return Error::None;
	}

// ---------------------------------------------------

	void KeyframeAnimationResourceView::DetachFromPackage( ContentPackage& /*package*/, ContentLibrary& library ) const {
		RemoveFromLibraryAs<KeyframeAnimationResourceView>( library );
	}

}	// namespace Animation
}	// namespace Eldritch2
