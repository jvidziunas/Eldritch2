/*==================================================================*\
  HLSLPipelineDefinitionView.cpp
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

	void HLSLPipelineDefinitionView::UsageDefinition::PrepareContext( ::ID3D11DeviceContext* const deviceContext ) const {
		deviceContext->VSSetShader( vertexShader.GetUnadornedPointer(), nullptr, 0u );
		deviceContext->HSSetShader( hullShader.GetUnadornedPointer(), nullptr, 0u );
		deviceContext->DSSetShader( domainShader.GetUnadornedPointer(), nullptr, 0u );
		deviceContext->GSSetShader( geometryShader.GetUnadornedPointer(), nullptr, 0u );
		deviceContext->PSSetShader( pixelShader.GetUnadornedPointer(), nullptr, 0u );
		deviceContext->OMSetDepthStencilState( depthStencilState.GetUnadornedPointer(), 0u );
		deviceContext->RSSetState( rasterizerState.GetUnadornedPointer() );
	}

// ---------------------------------------------------

	HLSLPipelineDefinitionView::HLSLPipelineDefinitionView( const UTF8Char* const name, const COMPointer<::ID3D11Device>& device ) : ResourceView( name ), _device( device ) {}

// ---------------------------------------------------

	ErrorCode HLSLPipelineDefinitionView::AttachToPackage( const Range<const char*> /*bytes*/, ContentPackage& /*package*/, ContentLibrary& library ) {
		PublishToLibraryAs<HLSLPipelineDefinitionView>( library );

		return Error::None;
	}

// ---------------------------------------------------

	void HLSLPipelineDefinitionView::DetachFromPackage( ContentPackage& /*package*/, ContentLibrary& library ) const {
		RemoveFromLibraryAs<HLSLPipelineDefinitionView>( library );
	}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const HLSLPipelineDefinitionView::GetSerializedDataTag() {
		return UTF8L("HLSLPipelineDefinition");
	}

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2