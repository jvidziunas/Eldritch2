/*==================================================================*\
  ShaderResourceResourceView.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Direct3D11/ShaderResourceResourceView.hpp>
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

	ShaderResourceResourceView::ShaderResourceResourceView( ContentLibrary& owningLibrary, ContentPackage& package, const UTF8Char* const name, Allocator& allocator ) : ResourceView( owningLibrary, package, name, allocator ) {}

// ---------------------------------------------------

	ErrorCode ShaderResourceResourceView::UpdateFromByteStream( const Range<const char*> /*sourceBytes*/ ) {
		return Error::OPERATION_NOT_SUPPORTED;
	}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const ShaderResourceResourceView::GetSerializedDataTag() {
		return UTF8L("ShaderResource");
	}

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2