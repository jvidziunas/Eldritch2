/*==================================================================*\
  ImageResourceView.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Direct3D11/ImageResourceView.hpp>
#include <Utility/Containers/Range.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "D3D11.lib" )
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;
using namespace ::std;

namespace Eldritch2 {
namespace Renderer {
namespace Direct3D11 {

	ImageResourceView::ImageResourceView( const UTF8Char* const name ) : ResourceView( name ) {}

// ---------------------------------------------------

	ErrorCode ImageResourceView::AttachToPackage( const Range<const char*> /*bytes*/, ContentPackage& /*package*/, ContentLibrary& library ) {
		PublishToLibraryAs<ImageResourceView>( library );

		return Error::None;
	}

// ---------------------------------------------------

	void ImageResourceView::DetachFromPackage( ContentPackage& /*package*/, ContentLibrary& library ) const {
		RemoveFromLibraryAs<ImageResourceView>( library );
	}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const ImageResourceView::GetSerializedDataTag() {
		return UTF8L("Image");
	}

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2