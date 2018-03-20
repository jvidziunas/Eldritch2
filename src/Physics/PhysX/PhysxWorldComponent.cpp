/*==================================================================*\
  PhysxWorldComponent.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/PhysXWorldComponent.hpp>
#include <Physics/PhysX/PhysicsScene.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace {

	using namespace ::physx;

	static ETInlineHint ETPureFunctionHint PxReal AsSeconds( uint64 microseconds ) {
		static constexpr float32 SecondsPerMicrosecond = 1.0f / 1000000.0f;

		return AsFloat( microseconds ) * SecondsPerMicrosecond;
	}

}	// anonymous namespace

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;
	using namespace ::Eldritch2::Core;

	PhysxWorldComponent::PhysxWorldComponent(
		const World& owner
	) : WorldComponent( owner.GetServices() ),
		CpuDispatcher( 4u ),
		_log( owner.GetLog() ),
		_shouldJoinScene( false ),
		_scene( nullptr ) {}

// ---------------------------------------------------

	void PhysxWorldComponent::AcceptVisitor( JobExecutor& executor, const EarlyTickVisitor& ) {
		MICROPROFILE_SCOPEI( "World/EarlyTick/PhysX", "Join simulation", 0x32AACD );

		if (ET_LIKELY( _shouldJoinScene )) {
			_shouldJoinScene = false;

			if (ET_UNLIKELY( Failed( _scene->JoinSimulation( executor ) ) )) {
				World& world( FindService<World>() );

				_log.Write( MessageType::Error, "PhysX hardware error simulating world {}, terminating." UTF8_NEWLINE, fmt::ptr( &world ) );
				world.SetShouldShutDown();
			}
		}
	}

// ---------------------------------------------------

	void PhysxWorldComponent::AcceptVisitor( JobExecutor& executor, const LateTickVisitor& tick ) {
		MICROPROFILE_SCOPEI( "World/LateTick/PhysX", "Start simulation", 0x32AACD );

		_scene->BeginSimulation( executor, AsSeconds( tick.durationInMicroseconds ) );
		_shouldJoinScene = true;
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2