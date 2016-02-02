/*==================================================================*\
  EngineService.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Input/XInput/EngineService.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Input;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Input {
namespace XInput {

	EngineService::EngineService( GameEngine& owningEngine ) : GameEngineService( owningEngine ) {}

// ---------------------------------------------------

	const UTF8Char* const EngineService::GetName() const {
		return UTF8L("XInput Service");
	}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( ScriptAPIRegistrationInitializationVisitor& typeRegistrar ) {
		Controller::ExposeScriptAPI( typeRegistrar );
	}

// ---------------------------------------------------

	void EngineService::OnServiceTickStarted( WorkerContext& /*executingContext*/ ) {
		MICROPROFILE_SCOPEI( "Input", "XInput Controller Sample", 0xFFFFFF );

		for( auto& controller : _controllers ) {
			controller.ReadInput();
		}
	}

}	// namespace XInput
}	// namespace Input
}	// namespace Eldritch2