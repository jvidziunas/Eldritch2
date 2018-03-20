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
#	include <steamclientpublic.h>
ET_POP_COMPILER_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Networking {
		namespace Steamworks {
			class	SteamworksEngineComponent;
		}
	}
}

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
	//!	Constructs this @ref Player instance.
		Player( SteamworksEngineComponent& owner );
	//!	Disable copy construction.
		Player( const Player& ) = delete;

		~Player() = default;

	// ---------------------------------------------------

	public:
		const Utf8Char*	GetName() const;

		void			NotifyPersonaNameChange( const Utf8Char* const name );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		Player&	operator=( const Player& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		Utf8Char					_name[NameMaxLength];
		CSteamID					_id;
		SteamworksEngineComponent*	_owner;
	};

}	// namespace Steamworks
}	// namespace Networking
}	// namespace Eldritch2