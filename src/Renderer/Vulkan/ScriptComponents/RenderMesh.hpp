/*==================================================================*\
  RenderMesh.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/CountedPointer.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Animation {
		namespace ScriptComponents {
			class	Armature;
		}
	}

	namespace Scripting {
		class	ApiRegistrar;
	}

	namespace Renderer {
		namespace Vulkan {
			namespace AssetViews {
				class	MeshView;
			}
		}
	}
}

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {
namespace ScriptComponents {

	class RenderMesh {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref RenderMesh instance.
		RenderMesh( const Animation::ScriptComponents::Armature& armature, const AssetViews::MeshView& asset );
	//!	Constructs this @ref RenderMesh instance.
		RenderMesh( const RenderMesh& );

		~RenderMesh();

	// - SCRIPT API REFERENCE ----------------------------

	public:
	//! Registers all script-callable methods for the @ref RenderMesh type with the specified script type registrar.
		static void	RegisterScriptApi( Scripting::ApiRegistrar& registrar );

	// - DATA MEMBERS ------------------------------------

	private:
		const Animation::ScriptComponents::Armature*	_armature;
		const AssetViews::MeshView*						_asset;
	};

}	// namespace ScriptComponents
}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2