/*==================================================================*\
  MeshResourceView.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
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
#if( ET_COMPILER_IS_MSVC )
//	MSVC complains about macro redefinitions, since a few DirectX components separately
//	define some HRESULT values without an include guard. The definitions themselves are consistent,
//	so just disable the warning.
#	pragma warning( push )
#	pragma warning( disable : 4005 )
#endif
#include <D3D11.h>
#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif
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

	MeshResourceView::MeshResourceView( const UTF8Char* const name, Allocator& allocator ) : ResourceView( name, allocator ), _submeshes( { allocator, UTF8L("Direct3D11 Mesh View Submesh Allocator") } ) {}

// ---------------------------------------------------

	ErrorCode MeshResourceView::UpdateFromByteStream( const Range<const char*> /*bytes*/ ) {
		return Error::OPERATION_NOT_SUPPORTED;
	}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const MeshResourceView::GetSerializedDataTag() {
		return UTF8L("Mesh");
	}

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2