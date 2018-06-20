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
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {

	class Game {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		Game(const Game&) = delete;
		//!	Constructs this @ref Game instance.
		Game();

		~Game();

		// ---------------------------------------------------

	public:
		WrenInterpretResult HandlePlayerJoin(WrenVM* vm, const Utf8Char* const name);

		WrenInterpretResult HandlePlayerLeave(WrenVM* vm, const Utf8Char* const name);

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(WrenVM* vm);

		void FreeResources(WrenVM* vm);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Game& operator=(const Game&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		WrenHandle* _game;

		WrenHandle* _whenPlayerJoinsStub;
		WrenHandle* _whenPlayerLeavesStub;

		SymbolTable<Utf8Char>  _tags;
		ArrayList<WrenHandle*> _gameObjects;
	};

}}} // namespace Eldritch2::Scripting::Wren
