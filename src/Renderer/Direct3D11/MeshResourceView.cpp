/*==================================================================*\
  MeshResourceView.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Direct3D11/HLSLPipelineDefinitionView.hpp>
#include <Renderer/Direct3D11/MeshResourceView.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//
#include <D3D11.h>
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

namespace Eldritch2 {
namespace Renderer {
namespace Direct3D11 {

	MeshResourceView::MeshResourceView( const Initializer& initializer, Allocator& allocator ) : ResourceView( initializer, allocator ), _submeshes( { allocator, UTF8L("Direct3D11 Mesh View Submesh Allocator") } ) {}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const MeshResourceView::GetSerializedDataTag() {
		return UTF8L("Mesh");
	}

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2