/*==================================================================*\
  Player.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
//	(6340) Valve has a few mismatches in their printf specifiers, disable the warning.
ET_PUSH_MSVC_WARNING_STATE(disable : 6340)
#include <isteamclient.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Networking { namespace Steamworks {

	class Player {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		Player(const Player&) = delete;
		//!	Constructs this @ref Player instance.
		Player() ETNoexceptHint;

		~Player();

		// ---------------------------------------------------

	public:
		StringSpan GetName() const ETNoexceptHint;

		void SetName(StringSpan name) ETNoexceptHint;

		// ---------------------------------------------------

	public:
		bool IsLocal() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		Result BindResources(HSteamPipe pipe, EAccountType type);
		Result BindResources(HSteamPipe pipe);

		void FreeResources(HSteamPipe pipe) ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Player& operator=(const Player&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		Utf8Char   _name[128];
		HSteamUser _user;
	};

}}} // namespace Eldritch2::Networking::Steamworks
