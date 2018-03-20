/*==================================================================*\
  Light.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Light.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		namespace Wren {
			class	ApiBuilder;
		}
	}
}

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {
namespace ScriptClasses {

	class Light : public Renderer::Light {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Light instance.
		Light( Transformation localToWorld, RgbColor color, float16 radius );
	//!	Disable copy construction.
		Light( const Light& ) = delete;

		~Light() = default;

	// - SCRIPT API --------------------------------------

	public:
	//! Registers all script-callable methods for the @ref Light type with the specified script API builder.
		static void	DefineClass( Scripting::Wren::ApiBuilder& api );
	};

}	// namespace ScriptClasses
}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2