/*==================================================================*\
  EngineService.Player.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Networking/Steamworks/EngineService.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Core;

namespace Eldritch2 {
namespace Networking {
namespace Steamworks {

	EngineService::Player::Player( EngineService& service, Allocator& allocator ) : _name( { allocator, "Player Name Allocator" } ), _service( service ) {}

// ---------------------------------------------------

	const Utf8Char* EngineService::Player::GetName() const {
		return _name;
	}

}	// namespace Steamworks
}	// namespace Networking
}	// namespace Eldritch2