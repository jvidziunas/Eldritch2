/*==================================================================*\
  SteamworksNetworkingService.Player.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Networking/SteamworksNetworkingService.hpp>
#include <Foundation/World.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Networking;
using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Networking {

	SteamworksNetworkingService::Player::Player( const NetworkID& networkID, SteamworksNetworkingService& networkingService, Allocator& allocator ) : _name( ::Eldritch2::EmptyStringSemantics, allocator, UTF8L( "Player Name Allocator" ) ),
																																					  _networkingService( networkingService ),
																																					  _networkID( networkID ) {}

// ---------------------------------------------------

	SteamworksNetworkingService::Player::~Player() {}

// ---------------------------------------------------

	void SteamworksNetworkingService::Player::Dispose() {
		_networkingService._allocator.Delete( *this );
	}

// ---------------------------------------------------

	const UTF8Char* SteamworksNetworkingService::Player::GetName() const {
		return _name.GetCharacterArray();
	}

}	// namespace Networking
}	// namespace Eldritch2