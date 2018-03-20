/*==================================================================*\
  Player.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Networking/Steamworks/Player.hpp>
//------------------------------------------------------------------//

#if defined( ZeroMemory )
#	undef ZeroMemory
#endif

namespace Eldritch2 {
namespace Networking {
namespace Steamworks {

	Player::Player( SteamworksEngineComponent& owner ) : _owner( &owner ) {
		ZeroMemory( _name );
	}

// ---------------------------------------------------

	const Utf8Char* Player::GetName() const {
		return eastl::begin( _name );
	}

// ---------------------------------------------------

	void Player::NotifyPersonaNameChange( const Utf8Char* const name ) {
		CopyString( _name, name );
	}

}	// namespace Steamworks
}	// namespace Networking
}	// namespace Eldritch2