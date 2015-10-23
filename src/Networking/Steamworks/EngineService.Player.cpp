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
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Networking {
namespace Steamworks {

	EngineService::Player::Player( const NetworkID& networkID, EngineService& networkingService, Allocator& allocator ) : _name( ::Eldritch2::EmptyStringSemantics, { allocator, UTF8L("Player Name Allocator") } ),
																														  _networkingService( networkingService ),
																														  _networkID( networkID ) {}

// ---------------------------------------------------

	void EngineService::Player::Dispose() {
		_networkingService._allocator.Delete( *this );
	}

// ---------------------------------------------------

	const UTF8Char* EngineService::Player::GetName() const {
		return _name.GetCharacterArray();
	}

}	// namespace Steamworks
}	// namespace Networking
}	// namespace Eldritch2