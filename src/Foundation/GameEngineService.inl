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

	template <void (GameEngineService::*InitializationFunction)( Scheduler::WorkerContext& )>
	ETInlineHint void GameEngineService::InvokeInitializationFunction( Scheduler::WorkerContext& executingContext ) {
		WorkerContext::FinishCounter	finishCounter( 0 );

		InvokeTickFunction<InitializationFunction>( finishCounter, executingContext );

		executingContext.WaitForCounter( finishCounter );
	}

// ---------------------------------------------------

	template <void (GameEngineService::*InitializationFunction)( Scheduler::WorkerContext& )>
	ETInlineHint void GameEngineService::InvokeInitializationFunction( Scheduler::WorkerContext::FinishCounter& finishCounter, Scheduler::WorkerContext& executingContext ) {
		for( GameEngineService& service : _owningEngine.GetAttachedServices() ) {
			executingContext.Enqueue( finishCounter, { &service, [] ( void* parameter, WorkerContext& executingContext ) {
				(static_cast<GameEngineService*>(parameter)->*InitializationFunction)( executingContext );
			} } );
		}
	}

// ---------------------------------------------------

	template <typename InitializationVisitor>
	ETInlineHint void GameEngineService::BroadcastInitializationVisitor( InitializationVisitor&& visitor ) {
		for( auto& service : _owningEngine.GetAttachedServices() ) {
			service.AcceptInitializationVisitor( visitor );
		}
	}

// ---------------------------------------------------

	template <void (GameEngineService::*TickFunction)( Scheduler::WorkerContext& )>
	ETInlineHint void GameEngineService::InvokeTickFunction( Scheduler::WorkerContext& executingContext ) {
		WorkerContext::FinishCounter	finishCounter( 0 );

		InvokeTickFunction<TickFunction>( finishCounter, executingContext );

		executingContext.WaitForCounter( finishCounter );
	}

// ---------------------------------------------------

	template <void (GameEngineService::*TickFunction)( Scheduler::WorkerContext& )>
	ETInlineHint void GameEngineService::InvokeTickFunction( Scheduler::WorkerContext::FinishCounter& finishCounter, Scheduler::WorkerContext& executingContext ) {
		for( GameEngineService& service : _owningEngine.GetAttachedServices() ) {
			executingContext.Enqueue( finishCounter, { &service, [] ( void* parameter, WorkerContext& executingContext ) {
				(static_cast<GameEngineService*>(parameter)->*TickFunction)( executingContext );
			} } );
		}
	}

// ---------------------------------------------------

	ETInlineHint const Foundation::GameEngine& GameEngineService::GetGameEngine() const {
		return _owningEngine;
	}

}	// namespace Foundation
}	// namespace Eldritch2