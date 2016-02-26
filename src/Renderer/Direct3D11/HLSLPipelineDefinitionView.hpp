/*==================================================================*\
  HLSLPipelineDefinitionView.hpp
  ------------------------------------------------------------------
  Purpose:
  
  
  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Direct3D11/ForwardDeclarations.hpp>
#include <Utility/Containers/HashMap.hpp>
#include <Packages/ResourceView.hpp>
#include <Utility/COMPointer.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	ErrorCode;
}

namespace Eldritch2 {
namespace Renderer {
namespace Direct3D11 {

	class HLSLPipelineDefinitionView : public FileSystem::ResourceView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref HLSLPipelineDefinitionView instance.
		HLSLPipelineDefinitionView( const Utility::COMPointer<::ID3D11Device>& device, FileSystem::ContentLibrary& owningLibrary, FileSystem::ContentPackage& package, const ::Eldritch2::UTF8Char* const name, ::Eldritch2::Allocator& allocator );

		//! Destroys this @ref HLSLPipelineDefinitionView instance.
		~HLSLPipelineDefinitionView() = default;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	UpdateFromByteStream( const ::Eldritch2::Range<const char*> bytes ) override;

	// ---------------------------------------------------

		static ETNoAliasHint const ::Eldritch2::UTF8Char* const	GetSerializedDataTag();

	// - TYPE PUBLISHING ---------------------------------

	private:
		struct UsageDefinition {
			void	PrepareContext( ::ID3D11DeviceContext* const deviceContext ) const;

		// ---------------------------------------------------

			Utility::COMPointer<::ID3D11VertexShader>		vertexShader;
			Utility::COMPointer<::ID3D11HullShader>			hullShader;
			Utility::COMPointer<::ID3D11DomainShader>		domainShader;
			Utility::COMPointer<::ID3D11GeometryShader>		geometryShader;
			Utility::COMPointer<::ID3D11PixelShader>		pixelShader;
			Utility::COMPointer<::ID3D11RasterizerState>	rasterizerState;
			Utility::COMPointer<::ID3D11DepthStencilState>	depthStencilState;
		};

	// - DATA MEMBERS ------------------------------------

		Utility::COMPointer<::ID3D11Device>	_device;
	};

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2