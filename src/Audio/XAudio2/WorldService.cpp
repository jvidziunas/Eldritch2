/*==================================================================*\
  WorldService.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Audio/XAudio2/WorldService.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Core;

namespace Eldritch2 {
namespace Audio {
namespace XAudio2 {

	WorldService::WorldService( const World& world ) : Core::WorldService( world.GetServiceBlackboard() ), _allocator( world.GetAllocator(), "XAudio2 World Service Root Allocator" ), _log( world.GetLog() ) {}

// ---------------------------------------------------

	Utf8Literal WorldService::GetName() const {
		return "XAudio2 World Service";
	}

}	// namespace XAudio2
}	// namespace Audio
}	// namespace Eldritch2

