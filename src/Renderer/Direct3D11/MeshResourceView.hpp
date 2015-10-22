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
#include <Renderer/Direct3D11/ForwardDeclarations.hpp>
#include <Utility/Containers/ResizableArray.hpp>
#include <Packages/ResourceView.hpp>
#include <Utility/MPL/IntTypes.hpp>
#include <Utility/COMPointer.hpp>
//------------------------------------------------------------------//
#include <D3D11.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Renderer {
		namespace Direct3D11 {
			class	HLSLPipelineDefinitionView;
		}
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

namespace Direct3D11 {

	class MeshResourceView : public FileSystem::ResourceView {
	// - TYPE PUBLISHING ---------------------------------

	public:
		struct Submesh {
			Utility::COMPointer<::ID3D11InputLayout>	inputLayout;
			Direct3D11::HLSLPipelineDefinitionView*		pipeline;
			::Eldritch2::uint16							baseIndex;
			::Eldritch2::uint16							indexCount;
			::Eldritch2::uint16							maximumUsedBone;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//! Constructs this @ref MeshResourceView instance.
		MeshResourceView( const Initializer& initializer, ::Eldritch2::Allocator& allocator );

		// Destroys this @ref MeshResourceView instance.
		~MeshResourceView() = default;

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

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2