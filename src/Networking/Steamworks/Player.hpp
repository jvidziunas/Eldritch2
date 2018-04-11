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
ET_PUSH_COMPILER_WARNING_STATE()
//	(6340) Valve has a few mismatches in their printf specifiers, it seems! We can't fix these, so disable the warning.
	ET_SET_MSVC_WARNING_STATE( disable : 6340 )
#	include <isteamclient.h>
ET_POP_COMPILER_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Networking {
namespace Steamworks {

	class Player {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum : size_t {
			NameMaxLength = 128u
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		Player( const Player& ) = delete;
	//!	Constructs this @ref Player instance.
		Player();

		~Player();

	// ---------------------------------------------------

	public:
		const Utf8Char*	GetName() const;

		void			NotifyPersonaNameChange( const Utf8Char* const name );

	// ---------------------------------------------------

	public:
		bool	IsAttachedToLocalUser() const;

	// ---------------------------------------------------

	public:
		bool	BindToLocalUser( HSteamPipe pipe );

		bool	BindToAnonymousUser( HSteamPipe pipe );

		void	DisconnectFromUser( HSteamPipe pipe );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		Player&	operator=( const Player& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		Utf8Char	_name[NameMaxLength];
		HSteamUser	_localUser;
	};

}	// namespace Steamworks
}	// namespace Networking
}	// namespace Eldritch2