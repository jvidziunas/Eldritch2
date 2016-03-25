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

	RenderTargetResourceView::RenderTargetResourceView( const UTF8Char* const name ) : ImageResourceView( name ) {}

// ---------------------------------------------------

	ErrorCode RenderTargetResourceView::AttachToPackage( const Range<const char*> bytes, ContentPackage& package, ContentLibrary& library ) {
		return ImageResourceView::AttachToPackage( bytes, package, library );
	}

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2