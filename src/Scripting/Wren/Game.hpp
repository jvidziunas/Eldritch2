/*==================================================================*\
  Game.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

struct	WrenHandle;
struct	WrenVM;

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	class Game {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		Game( const Game& ) = delete;
	//!	Constructs this @ref Game instance.
		Game();

		~Game();

	// ---------------------------------------------------

	public:
		void	HandlePlayerJoin( WrenVM* vm, WrenHandle* unaryCallHandle, const Utf8Char* const name );

		void	HandlePlayerLeave( WrenVM* vm, WrenHandle* unaryCallHandle );

	// ---------------------------------------------------

	public:
		void	SetPlayerJoinHandler( WrenVM* vm, WrenHandle* handler );

		void	SetPlayerLeaveHandler( WrenVM* vm, WrenHandle* handler );

	// ---------------------------------------------------

	public:
		void	FreeResources( WrenVM* vm );

	// - DATA MEMBERS ------------------------------------

	private:
		WrenHandle*				_playerJoinHandler;
		WrenHandle*				_playerLeaveHandler;

		SymbolTable<Utf8Char>	_entityTags;
		ArrayList<WrenHandle*>	_gameObjects;
	};

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2