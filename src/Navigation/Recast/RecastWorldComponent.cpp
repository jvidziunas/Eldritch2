/*==================================================================*\
  RecastWorldComponent.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Navigation/Recast/RecastWorldComponent.hpp>
#include <Navigation/Recast/NavigationScene.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Navigation {
namespace Recast {
namespace {

	static ETInlineHint ETPureFunctionHint float32 AsSeconds( uint64 microseconds ) {
		static constexpr float32 SecondsPerMicrosecond = 1.0f / 1000000.0f;

		return AsFloat( microseconds ) * SecondsPerMicrosecond;
	}

}	// anonymous namespace

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Core;

	RecastWorldComponent::RecastWorldComponent( const World& owner ) : WorldComponent( owner.GetServices() ), _scene( nullptr ) {}

// ---------------------------------------------------

	void RecastWorldComponent::AcceptVisitor( JobExecutor& /*executor*/, const LateTickVisitor& tick ) {
		MICROPROFILE_SCOPEI( "World/LateTick", "Update Recast navigation", 0xBBBBBB );

		_scene->Simulate( AsSeconds( tick.durationInMicroseconds ) );
	}

}	// namespace Recast
}	// namespace Navigation
}	// namespace Eldritch2