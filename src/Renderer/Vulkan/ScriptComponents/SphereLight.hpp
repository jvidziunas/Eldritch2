/*==================================================================*\
  SphereLight.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Mpl/VectorTypes.hpp>
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
}

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {
namespace ScriptComponents {

	class SphereLight {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref SphereLight instance.
		SphereLight( const Animation::ScriptComponents::Armature& armature, Eldritch2::Float4 color );
	//!	Constructs this @ref SphereLight instance.
		SphereLight( const SphereLight& );

		~SphereLight();

	// - SCRIPT API REFERENCE ----------------------------

	public:
	//! Registers all script-callable methods for the @ref RenderMesh type with the specified script type registrar.
		static void	RegisterScriptApi( Scripting::ApiRegistrar& registrar );

	// - DATA MEMBERS ------------------------------------

	private:
		const Animation::ScriptComponents::Armature*	_armature;
		Eldritch2::Float4								_color;
	};

}	// namespace ScriptComponents
}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2