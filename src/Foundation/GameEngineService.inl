/*==================================================================*\
  GameEngineService.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Foundation {

	namespace Foundation	= ::Eldritch2::Foundation;
	namespace Scheduler		= ::Eldritch2::Scheduler;

// ---------------------------------------------------

	template <typename InitializationVisitor>
	void GameEngineService::BroadcastInitializationVisitor( InitializationVisitor&& visitor ) {
		for( auto& service : _owningEngine._attachedServices ) {
			service.AcceptInitializationVisitor( visitor );
		}
	}

// ---------------------------------------------------

	template <typename TaskVisitor>
	void GameEngineService::BroadcastTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::Task& visitingTask, Scheduler::WorkerContext& executingContext, TaskVisitor&& visitor ) {
		for( auto& service : _owningEngine._attachedServices ) {
			service.AcceptTaskVisitor( subtaskAllocator, visitingTask, executingContext, visitor );
		}
	}

}	// namespace Foundation
}	// namespace Eldritch2