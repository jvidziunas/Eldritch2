/*==================================================================*\
  WorldService.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/WorldService.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scheduling;
using namespace ::Eldritch2::Scripting;

namespace Eldritch2 {
namespace Core {

	void WorldService::AcceptVisitor( JobFiber& /*fiber*/, const GameStartVisitor ) {
	//	Default implementation should not do anything.
		MICROPROFILE_SCOPEI( GetName(), "Default game start function", 0xA011F0 );
	}

// ---------------------------------------------------

	void WorldService::AcceptVisitor( JobFiber& /*fiber*/, const GameStopVisitor ) {
	//	Default implementation should not do anything.
		MICROPROFILE_SCOPEI( GetName(), "Default game stop function", 0xA011F0 );
	}

// ---------------------------------------------------

	void WorldService::AcceptVisitor( JobFiber& /*fiber*/, FixedTickVisitor& /*visitor*/ ) {
	//	Default implementation should not do anything.
		MICROPROFILE_SCOPEI( GetName(), "Default fixed-rate tick function", 0xA011F0 );
	}

// ---------------------------------------------------

	void WorldService::AcceptVisitor( JobFiber& /*fiber*/, VariableTickVisitor& /*visitor*/ ) {
	//	Default implementation should not do anything.
		MICROPROFILE_SCOPEI( GetName(), "Default variable-rate tick function", 0xA011F0 );
	}

// ---------------------------------------------------

	void WorldService::AcceptVisitor( Scheduling::JobFiber& /*executor*/, const PreScriptTickVisitor ) {
	//	Default implementation should not do anything.
		MICROPROFILE_SCOPEI( GetName(), "Default script pre-tick function", 0xA011F0 );
	}

// ---------------------------------------------------

	void WorldService::AcceptVisitor( Scheduling::JobFiber& /*executor*/, const ScriptTickVisitor ) {
	//	Default implementation should not do anything.
		MICROPROFILE_SCOPEI( GetName(), "Default script tick function", 0xA011F0 );
	}

// ---------------------------------------------------

	void WorldService::AcceptVisitor( Scheduling::JobFiber& /*executor*/, const PostScriptTickVisitor ) {
	//	Default implementation should not do anything.
		MICROPROFILE_SCOPEI( GetName(), "Default script post-tick function", 0xA011F0 );
	}

// ---------------------------------------------------

	void WorldService::AcceptVisitor( MessageBus& /*messageBus*/ ) {
	//	Default implementation should not do anything.
		MICROPROFILE_SCOPEI( GetName(), "Default message dispatch function", 0xA011F0 );
	}

// ---------------------------------------------------

	void WorldService::AcceptVisitor( ServiceBlackboard& /*blackboard*/ ) {
	//	Default implementation should not do anything.
		MICROPROFILE_SCOPEI( GetName(), "Default service attach function", 0xA011F0 );
	}

}	// namespace Core
}	// namespace Eldritch2