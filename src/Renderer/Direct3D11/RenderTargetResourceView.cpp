/*==================================================================*\
  RenderTargetResourceView.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Direct3D11/RenderTargetResourceView.hpp>
#include <Utility/Containers/Range.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Renderer {
namespace Direct3D11 {

	RenderTargetResourceView::RenderTargetResourceView( ContentLibrary& owningLibrary, ContentPackage& package, const UTF8Char* const name, Allocator& allocator ) : ResourceView( owningLibrary, package, name, allocator ) {}

// ---------------------------------------------------

	ErrorCode RenderTargetResourceView::UpdateFromByteStream( const Range<const char*> /*bytes*/ ) {
		return Error::NONE;
	}

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2