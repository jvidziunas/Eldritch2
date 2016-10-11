/*==================================================================*\
  Engine.inl
  ------------------------------------------------------------------
  Purpose:
  Implements an 'engine' object that binds together most subsystems
  into a coherent whole. Also serves as an intermediate layer
  between higher-level OS-dependent application systems and
  low-level generic simulation engines.

  ------------------------------------------------------------------
  ©2010-2012 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Core {

	ETInlineHint const Core::ServiceBlackboard& Engine::GetServiceBlackboard() const {
		return _blackboard;
	}

// ---------------------------------------------------
	
	ETInlineHint Eldritch2::Allocator& Engine::GetAllocator() const {
		return _allocator;
	}

// ---------------------------------------------------

	ETInlineHint Logging::Log& Engine::GetLog() const {
		return _log;
	}

// ---------------------------------------------------

	template <typename... Arguments>
	ETInlineHint void Engine::VisitServices( Scheduling::JobFiber& executor, Arguments&&... arguments ) {
		Scheduling::JobBarrier	barrier( 0 );

		for( const auto& service : _services ) {
			executor.Enqueue( barrier, [&service, &arguments...]( Scheduling::JobFiber& executor ) {
			//	We are deliberately not forwarding here.
				service->AcceptVisitor( executor, arguments... );
			} );
		}

		executor.AwaitBarrier( barrier );
	}

// ---------------------------------------------------

	template <typename... Arguments>
	ETInlineHint void Engine::VisitServices( Arguments&&... arguments ) {
		for( const auto& service : _services ) {
		//	We are deliberately not forwarding here.
			service->AcceptVisitor( arguments... );
		}
	}

// ---------------------------------------------------

	template <typename... Arguments>
	ETInlineHint void Engine::VisitWorlds( Scheduling::JobFiber& executor, Arguments&&... arguments ) {
		Scheduling::JobBarrier	barrier( 0 );

		for( const auto& world : _worlds ) {
			executor.Enqueue( barrier, [&world, &arguments...]( Scheduling::JobFiber& executor ) {
			//	We are deliberately *not* forwarding here.
				world->AcceptVisitor( executor, arguments... );
			} );
		}

		executor.AwaitBarrier( barrier );
	}

// ---------------------------------------------------

	template <typename... Arguments>
	ETInlineHint void Engine::VisitWorlds( Arguments&&... arguments ) {
		for( const auto& world : _worlds ) {
		//	We are deliberately *not* forwarding here.
			world->AcceptVisitor( arguments... );
		}
	}

// ---------------------------------------------------

	ETInlineHint bool Engine::HasShutDown() const {
		return _hasShutDown.load( std::memory_order_consume );
	}

// ---------------------------------------------------

	ETInlineHint void Engine::ShutDown() const {
		_hasShutDown.store( true, std::memory_order_release );
	}

}	// namespace Core
}	// namespace Eldritch2