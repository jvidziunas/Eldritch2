/*==================================================================*\
  Direct3D11HLSLPipelineDefinitionView.hpp
  ------------------------------------------------------------------
  Purpose:
  
  
  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/D3D11/D3D11ForwardDeclarations.hpp>
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

	class Direct3D11HLSLPipelineDefinitionView : public FileSystem::ResourceView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref Direct3D11HLSLPipelineDefinitionView instance.
		Direct3D11HLSLPipelineDefinitionView( const Initializer& initializer, ::Eldritch2::Allocator& allocator );

		//! Destroys this @ref Direct3D11HLSLPipelineDefinitionView instance.
		~Direct3D11HLSLPipelineDefinitionView() = default;

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

}	// namespace Renderer
}	// namespace Eldritch2