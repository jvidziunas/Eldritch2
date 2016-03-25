/*==================================================================*\
  WorldView.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Foundation\WorldView.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	class WorldView : public Foundation::WorldView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		WorldView( Foundation::World& owningWorld );

		~WorldView() = default;

	// ---------------------------------------------------

		static void	ExposeScriptApi( Scripting::ScriptApiRegistrationInitializationVisitor& visitor );

	// ---------------------------------------------------

		void	OnPreScriptTick( Scheduler::WorkerContext& executingContext ) override;


	private:

	// - DATA MEMBERS ------------------------------------

	};

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2