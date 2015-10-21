/*==================================================================*\
  Direc3D11MeshView.hpp
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
#include <Utility/Containers/ResizableArray.hpp>
#include <Packages/ResourceView.hpp>
#include <Utility/MPL/IntTypes.hpp>
#include <Utility/COMPointer.hpp>
#include <D3D11.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Renderer {
		class	Direct3D11HLSLPipelineDefinitionView;
		class	Direct3D11RendererService;
	}

	class	ErrorCode;
	template <typename Iterator>
	class	Range;
}

namespace Eldritch2 {
namespace Renderer {

	namespace Renderer	= ::Eldritch2::Renderer;
	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	class Direct3D11MeshView : public FileSystem::ResourceView {
	// - TYPE PUBLISHING ---------------------------------

	public:
		struct Submesh {
			Utility::COMPointer<::ID3D11InputLayout>		inputLayout;
			Renderer::Direct3D11HLSLPipelineDefinitionView*	pipeline;
			::Eldritch2::uint16								baseIndex;
			::Eldritch2::uint16								indexCount;
			::Eldritch2::uint16								maximumUsedBone;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//! Constructs this @ref Direct3D11MeshView instance.
		Direct3D11MeshView( const Initializer& initializer, ::Eldritch2::Allocator& allocator );

		// Destroys this @ref Direct3D11MeshView instance.
		~Direct3D11MeshView() = default;

	// ---------------------------------------------------

		static ETNoAliasHint const ::Eldritch2::UTF8Char* const	GetSerializedDataTag();

	// ---------------------------------------------------

		bool	PrepareContext( ::ID3D11DeviceContext* const deviceContext, const ::Eldritch2::UTF8Char* const usageName ) const;

	// - DATA MEMBERS ------------------------------------

	private:
		Utility::COMPointer<::ID3D11Buffer>		_vertexBuffer;
		Utility::COMPointer<::ID3D11Buffer>		_indexBuffer;

		::Eldritch2::uint32						_baseOffsetInVertexBufferInBytes;
		::Eldritch2::uint32						_baseOffsetInIndexBufferInBytes;
		::Eldritch2::ResizableArray<Submesh>	_submeshes;
	};

}	// namespace Renderer
}	// namespace Eldritch2