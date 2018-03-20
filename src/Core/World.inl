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
#include <Scheduling/JobExecutor.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Core {

	ETInlineHint const World::ComponentList<WorldComponent>& World::GetComponents() const {
		return _components;
	}

// ---------------------------------------------------

	ETInlineHint const Blackboard& World::GetServices() const {
		return _services;
	}

// ---------------------------------------------------

	ETInlineHint Logging::Log& World::GetLog() const {
		return _log;
	}

// ---------------------------------------------------

	ETInlineHint float32 World::GetTimeScalar() const {
		return _timeAccumulator.GetTimeScalar();
	}

// ---------------------------------------------------

	ETInlineHint void World::SetTimeScalar( float32 scalar ) {
		_timeAccumulator.SetTimeScalar( scalar );
	}

// ---------------------------------------------------

	template <typename Iterator>
	ETInlineHint ErrorCode World::BindResources( Scheduling::JobExecutor& executor, Iterator firstComponent, Iterator lastComponent ) {
		ComponentList<WorldComponent>	components( _components.GetAllocator() );

		components.Reserve( eastl::distance( firstComponent, lastComponent ) );

		for (; firstComponent != lastComponent; ++firstComponent) {
			Result<UniquePointer<WorldComponent>> result( (*firstComponent)->CreateWorldComponent( _allocator, *this ) );
			if (Failed( result )) {
				_log.Write( Logging::MessageType::Error, "Failed to create component for world {}: {}!" UTF8_NEWLINE, fmt::ptr( this ), AsCString( result ) );
				return result;
			}

			if (result->Get() != nullptr) {
				components.Append( eastl::move( *result ) );
			}
		}

		components.ShrinkToFit();

		Swap( _components, components );

		return BindResources( executor );
	}

// ---------------------------------------------------

	ETInlineHint bool World::IsRunning( std::memory_order order ) const {
		return _shouldShutDown.load( order ) == false;
	}

// ---------------------------------------------------

	ETInlineHint bool World::ShouldShutDown( std::memory_order order ) const {
		return _shouldShutDown.load( order );
	}

}	// namespace Core
}	// namespace Eldritch2