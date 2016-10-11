/*==================================================================*\
  WorldService.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/SmartPointers.hpp>
#include <Renderer/Vulkan/SceneRenderer.hpp>
#include <Renderer/Vulkan/UiRenderer.hpp>
#include <Core/WorldService.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	ApiRegistrar;
	}

	namespace Renderer {
		namespace Vulkan {
			namespace ScriptComponents {
				class	SceneCamera;
			}
		}
	}
}

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	class WorldService : public Core::WorldService {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref WorldService instance.
		WorldService( const Core::World& world );
	//!	Disable copying.
		WorldService( const WorldService& ) = delete;

		~WorldService() = default;

	// - DEBUG/LOGGING INFORMATION -----------------------

	public:
		Eldritch2::Utf8Literal	GetName() const override;

	// - SCRIPT API REFERENCE ----------------------------

	public:
	//! Registers all script-callable methods for the @ref WorldService type (and select script components) with the specified script type registrar.
		static void	RegisterScriptApi( Scripting::ApiRegistrar& registrar );

	// ---------------------------------------------------

	public:
		void	AcceptVisitor( Core::ServiceBlackboard& blackboard ) override;

	// ---------------------------------------------------

	public:
		void	OnScriptPreTick( Scheduling::JobFiber& executor ) override;

	// - DATA MEMBERS ------------------------------------

	private:
		Vulkan::SceneRenderer	_sceneRenderer;
		Vulkan::UiRenderer		_uiRenderer;
	};

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2