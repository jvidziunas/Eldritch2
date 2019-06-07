/*==================================================================*\
  Player.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Networking/Steamworks/Player.hpp>
//------------------------------------------------------------------//
#include <steam_api.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Networking { namespace Steamworks {

	namespace {

		enum : HSteamUser { InvalidUser = 0u };

	} // anonymous namespace

	Player::Player() ETNoexceptHint : _user(InvalidUser) {
		Fill(Begin(_name), End(_name), '\0');
	}

	// ---------------------------------------------------

	Player::~Player() {
		ETAssert(_user == InvalidUser, "Leaking Steamworks user {}!", _user);
	}

	// ---------------------------------------------------

	StringSpan Player::GetName() const ETNoexceptHint {
		return _name;
	}

	// ---------------------------------------------------

	void Player::SetName(StringSpan name) ETNoexceptHint {
		name.Copy(_name, ETCountOf(_name));
	}

	// ---------------------------------------------------

	bool Player::IsLocal() const ETNoexceptHint {
		return _user != InvalidUser;
	}

	// ---------------------------------------------------

	Result Player::BindResources(HSteamPipe pipe, EAccountType accountType) {
		using ::Eldritch2::Swap;

		HSteamUser user(SteamClient()->CreateLocalUser(ETAddressOf(pipe), accountType));
		ET_ABORT_UNLESS(user != InvalidUser ? Result::Success : Result::Unspecified);
		ET_AT_SCOPE_EXIT(if (user != InvalidUser) SteamClient()->ReleaseUser(user, pipe));

		Swap(_user, user);

		return Result::Success;
	}

	// ---------------------------------------------------

	Result Player::BindResources(HSteamPipe pipe) {
		using ::Eldritch2::Swap;

		HSteamUser user(SteamClient()->ConnectToGlobalUser(pipe));
		ET_ABORT_UNLESS(user != InvalidUser ? Result::Success : Result::Unspecified);
		ET_AT_SCOPE_EXIT(if (user != InvalidUser) SteamClient()->ReleaseUser(user, pipe));

		Swap(_user, user);

		return Result::Success;
	}

	// ---------------------------------------------------

	void Player::FreeResources(HSteamPipe pipe) ETNoexceptHint {
		if (const HSteamUser user = Exchange(_user, InvalidUser)) {
			SteamClient()->ReleaseUser(user, pipe);
		}
	}

}}} // namespace Eldritch2::Networking::Steamworks
