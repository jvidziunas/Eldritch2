/*==================================================================*\
  WorldView.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptApiRegistrationInitializationVisitor.hpp>
#include <Renderer/Vulkan/WorldView.hpp>
#include <Scheduler/WorkerContext.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	WorldView::WorldView( World& owningWorld ) : Foundation::WorldView( owningWorld ) {}

// ---------------------------------------------------

	void WorldView::ExposeScriptApi( ScriptApiRegistrationInitializationVisitor& /*visitor*/ ) {

	}

// ---------------------------------------------------

	void WorldView::OnPreScriptTick( WorkerContext& /*executingContext*/ ) {
		MICROPROFILE_SCOPEI( "Vulkan renderer", "Dispatch world render commands", 0x0011F0 );
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

