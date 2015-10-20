/*==================================================================*\
D3D11ResourceCommon.cpp
------------------------------------------------------------------
Purpose:


------------------------------------------------------------------
©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/D3D11/Direct3D11HLSLPipelineDefinitionView.hpp>
#include <Utility/MessagePackReader.hpp>
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

	void Direct3D11HLSLPipelineDefinitionView::UsageDefinition::PrepareContext( ::ID3D11DeviceContext* const deviceContext ) const {
		deviceContext->VSSetShader( vertexShader.GetUnadornedPointer(), nullptr, 0u );
		deviceContext->HSSetShader( hullShader.GetUnadornedPointer(), nullptr, 0u );
		deviceContext->DSSetShader( domainShader.GetUnadornedPointer(), nullptr, 0u );
		deviceContext->GSSetShader( geometryShader.GetUnadornedPointer(), nullptr, 0u );
		deviceContext->PSSetShader( pixelShader.GetUnadornedPointer(), nullptr, 0u );
		deviceContext->OMSetDepthStencilState( depthStencilState.GetUnadornedPointer(), 0u );
		deviceContext->RSSetState( rasterizerState.GetUnadornedPointer() );
	}

// ---------------------------------------------------

	Direct3D11HLSLPipelineDefinitionView::Direct3D11HLSLPipelineDefinitionView( const Initializer& initializer, Allocator& allocator ) : ResourceView( initializer, allocator ), _usages( { allocator, UTF8L("Direct3D11 Pipeline Definition Usage Directory Allocator") } ) {}

// ---------------------------------------------------

	ErrorCode Direct3D11HLSLPipelineDefinitionView::InstantiateFromByteArray( const Range<const char*>& /*sourceBytes*/, const COMPointer<::ID3D11Device>& /*device*/ ) {
		return Error::NONE;
	}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const Direct3D11HLSLPipelineDefinitionView::GetSerializedDataTag() {
		return UTF8L("HLSLPipelineDefinition");
	}

// ---------------------------------------------------

	bool Direct3D11HLSLPipelineDefinitionView::PrepareContext( ::ID3D11DeviceContext* const deviceContext, const UTF8Char* const usageName ) const {
		const auto	usageCandidate( _usages.Find( usageName ) );

		if( usageCandidate != _usages.End() ) {
			usageCandidate->second.PrepareContext( deviceContext );
			return true;
		}

		return false;
	}

}	// namespace Renderer
}	// namespace Eldritch2