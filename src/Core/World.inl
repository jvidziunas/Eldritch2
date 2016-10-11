/*==================================================================*\
  World.inl
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
namespace Core {

	template <typename... Arguments>
	ETInlineHint void World::VisitServices( Scheduling::JobFiber& executor, Arguments&&... arguments ) {
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
	ETInlineHint void World::VisitServices( Arguments&&... arguments ) {
		for( const auto& service : _services ) {
		//	We are deliberately not forwarding here.
			service->AcceptVisitor( arguments... );
		}
	}

// ---------------------------------------------------

	ETInlineHint const Core::ServiceBlackboard& World::GetServiceBlackboard() const {
		return _serviceBlackboard;
	}

// ---------------------------------------------------

	ETInlineHint Eldritch2::Allocator& World::GetAllocator() const {
		return _allocator;
	}

// ---------------------------------------------------

	ETInlineHint Logging::Log& World::GetLog() const {
		return _log;
	}

// ---------------------------------------------------

	ETInlineHint void World::ShutDown() const {
		_hasShutDown.store( true, std::memory_order_release );
	}

// ---------------------------------------------------

	ETInlineHint void World::SetProperties( std::initializer_list<Pair<const Eldritch2::Utf8Char*, const Eldritch2::Utf8Char*>> keyValuePairs ) {
		for( const auto& keyValuePair : keyValuePairs ) {
			SetProperty( keyValuePair.first, keyValuePair.second );
		}
	}

// ---------------------------------------------------

	ETInlineHint bool World::HasShutDown() const {
		return _hasShutDown.load( std::memory_order_consume );
	}

// ---------------------------------------------------

	ETInlineHint bool World::IsRunningGame() const {
		return !IsPaused();
	}

// ---------------------------------------------------

	ETInlineHint bool World::IsPaused() const {
		return _isPaused.load( std::memory_order_consume );
	}

}	// namespace Core
}	// namespace Eldritch2