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
#include <Foundation/World.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Networking;
using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Networking {
namespace Steamworks {

	EngineService::Player::Player( const ObjectHandle<World>& world, EngineService& service, Allocator& allocator ) : _name( { allocator, UTF8L("Player Name Allocator") } ),
																													  _service( service ),
																													  _world( world ) {}

// ---------------------------------------------------

	void EngineService::Player::Dispose() {
		_service._allocator.Delete( *this );
	}

// ---------------------------------------------------

	const UTF8Char* EngineService::Player::GetName() const {
		return _name.AsCString();
	}

}	// namespace Steamworks
}	// namespace Networking
}	// namespace Eldritch2