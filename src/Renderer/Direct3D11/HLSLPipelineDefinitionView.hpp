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
#include <Utility/Containers/UnorderedMap.hpp>
#include <Utility/StringOperators.hpp>
#include <Packages/ResourceView.hpp>
#include <Utility/COMPointer.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	ErrorCode;
}

namespace Eldritch2 {
namespace Renderer {

	namespace Renderer	= ::Eldritch2::Renderer;
	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

namespace Direct3D11 {

	class HLSLPipelineDefinitionView : public FileSystem::ResourceView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref HLSLPipelineDefinitionView instance.
		HLSLPipelineDefinitionView( const Initializer& initializer, ::Eldritch2::Allocator& allocator );

		//! Destroys this @ref HLSLPipelineDefinitionView instance.
		~HLSLPipelineDefinitionView() = default;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	InstantiateFromByteArray( const ::Eldritch2::Range<const char*>& sourceBytes, const Utility::COMPointer<::ID3D11Device>& device );

	// ---------------------------------------------------

		static ETNoAliasHint const ::Eldritch2::UTF8Char* const	GetSerializedDataTag();

	// ---------------------------------------------------

		bool	PrepareContext( ::ID3D11DeviceContext* const deviceContext, const ::Eldritch2::UTF8Char* const usageName ) const;

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

		::Eldritch2::UnorderedMap<const ::Eldritch2::UTF8Char*, UsageDefinition, Utility::StringHash, Utility::StringEqualComparator<>>	_usages;
	};

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2