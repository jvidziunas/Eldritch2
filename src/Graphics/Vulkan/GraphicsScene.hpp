/*==================================================================*\
  GraphicsScene.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/VulkanGraphicsScene.hpp>
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

	class GraphicsScene : public VulkanGraphicsScene {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref GraphicsScene instance.
		GraphicsScene( Allocator& allocator );
	//!	Disable copy construction.
		GraphicsScene( const GraphicsScene& ) = delete;

		~GraphicsScene() = default;

	// - SCRIPT API --------------------------------------

	public:
	//! Registers all script-callable methods for the @ref GraphicsScene type with the specified script API builder.
		void	DefineClass( Scripting::Wren::ApiBuilder& api );
	};

}	// namespace ScriptClasses
}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2