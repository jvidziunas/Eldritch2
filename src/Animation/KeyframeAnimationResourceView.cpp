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

	KeyframeAnimationResourceView::KeyframeAnimationResourceView( ContentLibrary& owningLibrary, ContentPackage& package, const UTF8Char* const name, Allocator& allocator ) : ResourceView( owningLibrary, package, name, allocator ) {}

// ---------------------------------------------------

	ErrorCode KeyframeAnimationResourceView::UpdateFromByteStream( const Range<const char*> /*bytes*/ ) {
		return Error::None;
	}

}	// namespace Animation
}	// namespace Eldritch2
