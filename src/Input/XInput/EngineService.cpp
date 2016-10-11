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
#include <Core/Engine.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scheduling;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Core;

namespace Eldritch2 {
namespace Input {
namespace XInput {

	EngineService::EngineService( const Engine& engine ) : Core::EngineService( engine.GetServiceBlackboard() ) {}

// ---------------------------------------------------

	Utf8Literal EngineService::GetName() const {
		return "XInput Service";
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( ApiRegistrar& /*typeRegistrar*/ ) {}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( JobFiber& /*fiber*/, const ServiceTickVisitor ) {
		MICROPROFILE_SCOPEI( GetName(), "XInput Controller Sample", 0xFFFFFF );

		for( auto& controller : _controllers ) {
			controller.SampleInput();
		}
	}

}	// namespace XInput
}	// namespace Input
}	// namespace Eldritch2