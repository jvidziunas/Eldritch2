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

#if defined( ZeroMemory )
#	undef ZeroMemory
#endif

namespace Eldritch2 {
	namespace Networking {
		namespace Steamworks {
			namespace {

				enum : HSteamUser { InvalidUser = 0u };

			}	// anonymous namespace

			Player::Player() : _localUser(InvalidUser) {
				ZeroMemory(_name);
			}

		// ---------------------------------------------------

			Player::~Player() {
				ET_ASSERT(_localUser == InvalidUser, "Leaking Steamworks user!");
			}

		// ---------------------------------------------------

			const Utf8Char* Player::GetName() const {
				return _name;
			}

		// ---------------------------------------------------

			void Player::NotifyPersonaNameChange(const Utf8Char* const name) {
				CopyString(_name, name);
			}

		// ---------------------------------------------------

			bool Player::IsAttachedToLocalUser() const {
				return _localUser != InvalidUser;
			}

		// ---------------------------------------------------

			bool Player::BindToLocalUser(HSteamPipe pipe) {
				const HSteamUser user(SteamClient()->ConnectToGlobalUser(pipe));
				if (user == InvalidUser) {
					return false;
				}

				DisconnectFromUser(pipe);

				_localUser = user;

				return true;
			}

		// ---------------------------------------------------

			bool Player::BindToAnonymousUser(HSteamPipe pipe) {
				const HSteamUser user(SteamClient()->CreateLocalUser(&pipe, k_EAccountTypeConsoleUser));
				if (user == InvalidUser) {
					return false;
				}

				DisconnectFromUser(pipe);

				_localUser = user;

				return true;
			}

		// ---------------------------------------------------

			void Player::DisconnectFromUser(HSteamPipe pipe) {
				if (HSteamUser user = eastl::exchange(_localUser, InvalidUser)) {
					SteamClient()->ReleaseUser(user, pipe);
				}
			}

		}	// namespace Steamworks
	}	// namespace Networking
}	// namespace Eldritch2