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
#include <steam_api.h>
//------------------------------------------------------------------//

#if defined(ZeroMemory)
#	undef ZeroMemory
#endif

namespace Eldritch2 { namespace Networking { namespace Steamworks {

	namespace {

		enum : HSteamUser { InvalidUser = 0u };

	} // anonymous namespace

	Player::Player() ETNoexceptHint : _user(InvalidUser) {
		ZeroMemory(_name);
	}

	// ---------------------------------------------------

	Player::~Player() {
		ET_ASSERT(_user == InvalidUser, "Leaking Steamworks user {}!", _user);
	}

	// ---------------------------------------------------

	StringView Player::GetName() const ETNoexceptHint {
		return _name;
	}

	// ---------------------------------------------------

	void Player::SetName(StringView name) ETNoexceptHint {
		name.Copy(_name, ETCountOf(_name));
	}

	// ---------------------------------------------------

	bool Player::IsLocal() const ETNoexceptHint {
		return _user != InvalidUser;
	}

	// ---------------------------------------------------

	ErrorCode Player::BindResources(HSteamPipe pipe, EAccountType accountType) {
		using ::Eldritch2::Swap;

		HSteamUser user(SteamClient()->CreateLocalUser(ETAddressOf(pipe), accountType));
		ET_ABORT_UNLESS(user != InvalidUser ? Error::None : Error::Unspecified);
		ET_AT_SCOPE_EXIT(if (user != InvalidUser) SteamClient()->ReleaseUser(user, pipe));

		Swap(_user, user);

		return Error::None;
	}

	// ---------------------------------------------------

	ErrorCode Player::BindResources(HSteamPipe pipe) {
		using ::Eldritch2::Swap;

		HSteamUser user(SteamClient()->ConnectToGlobalUser(pipe));
		ET_ABORT_UNLESS(user != InvalidUser ? Error::None : Error::Unspecified);
		ET_AT_SCOPE_EXIT(if (user != InvalidUser) SteamClient()->ReleaseUser(user, pipe));

		Swap(_user, user);

		return Error::None;
	}

	// ---------------------------------------------------

	void Player::FreeResources(HSteamPipe pipe) {
		if (const HSteamUser user = eastl::exchange(_user, InvalidUser)) {
			SteamClient()->ReleaseUser(user, pipe);
		}
	}

}}} // namespace Eldritch2::Networking::Steamworks
