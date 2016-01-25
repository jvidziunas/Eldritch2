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
#include <Utility/Memory/InstanceNew.hpp>
#include <Utility/ErrorCode.hpp>
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

	void EngineService::AcceptTaskVisitor( WorkerContext& executingContext, WorkerContext::FinishCounter& finishCounter, const ServiceTickTaskVisitor ) {
		executingContext.Enqueue( finishCounter, { this, [] ( void* service, WorkerContext& /*executingContext*/ ) {
			for( auto& controller : static_cast<EngineService*>(service)->_controllers ) {
				controller.ReadInput();
			}
		} } );
	}

}	// namespace XInput
}	// namespace Input
}	// namespace Eldritch2