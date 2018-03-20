/*==================================================================*\
  Mesh.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/CharTypes.hpp>
#include <Renderer/Geometry.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Animation {
		namespace ScriptClasses {
			class	Armature;
		}
	}

	namespace Scripting {
		namespace Wren {
			class	ApiBuilder;
		}
	}

	namespace Renderer {
		namespace AssetViews {
			class	MeshAsset;
		}
	}
}

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {
namespace ScriptClasses {

	class Mesh : public Renderer::Geometry {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Mesh instance.
		Mesh(
			const Renderer::AssetViews::MeshAsset& asset,
			const Animation::ScriptClasses::Armature& armature
		);
	//!	Constructs this @ref Mesh instance.
		Mesh( const Mesh& ) = default;

		~Mesh() = default;

	// ---------------------------------------------------

	public:
		const Utf8Char* const	GetAssetPath() const;

		void					SetAsset( const Renderer::AssetViews::MeshAsset& asset );

	// - SCRIPT API --------------------------------------

	public:
	//! Registers all script-callable methods for the @ref StaticMesh type with the specified script API builder.
		static void	DefineClass( Scripting::Wren::ApiBuilder& api );
	};

}	// namespace ScriptClasses
}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2