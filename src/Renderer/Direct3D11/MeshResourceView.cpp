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
#include <Packages/ContentPackage.hpp>
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

	MeshResourceView::MeshResourceView( const UTF8Char* const name, Allocator& allocator ) : ResourceView( name ), _submeshes( { allocator, UTF8L("Direct3D11 Mesh View Submesh Allocator") } ) {}

// ---------------------------------------------------

	ErrorCode MeshResourceView::AttachToPackage( const Range<const char*> /*bytes*/, ContentPackage& /*package*/, ContentLibrary& library ) {
		PublishToLibraryAs<decltype(*this)>( library );

		return Error::None;
	}

// ---------------------------------------------------

	void MeshResourceView::DetachFromPackage( ContentPackage& /*package*/, ContentLibrary& library ) const {
		RemoveFromLibraryAs<decltype(*this)>( library );
	}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const MeshResourceView::GetSerializedDataTag() {
		return UTF8L("Mesh");
	}

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2