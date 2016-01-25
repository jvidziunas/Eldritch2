/*==================================================================*\
  GameEngineService.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Foundation {

	template <typename InitializationVisitor>
	void GameEngineService::BroadcastInitializationVisitor( InitializationVisitor&& visitor ) {
		for( auto& service : _owningEngine._attachedServices ) {
			service.AcceptInitializationVisitor( visitor );
		}
	}

// ---------------------------------------------------

	template <typename TaskVisitor>
	void GameEngineService::BroadcastTaskVisitor( Scheduler::WorkerContext& executingContext, Scheduler::WorkerContext::FinishCounter& finishCounter, TaskVisitor&& visitor ) {
		for( auto& service : _owningEngine._attachedServices ) {
			service.AcceptTaskVisitor( executingContext, finishCounter, visitor );
		}
	}

// ---------------------------------------------------

	ETInlineHint const Foundation::GameEngine& GameEngineService::GetGameEngine() const {
		return _owningEngine;
	}

}	// namespace Foundation
}	// namespace Eldritch2