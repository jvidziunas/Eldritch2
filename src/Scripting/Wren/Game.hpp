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
#include <Networking/WorldEventClient.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {
	class Dispatcher;
	class Context;
}}} // namespace Eldritch2::Scripting::Wren

struct WrenHandle;
struct WrenVM;

namespace Eldritch2 { namespace Scripting { namespace Wren {

	class GameObject {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		GameObject(const GameObject&) = delete;
		//!	Constructs this @ref GameObject instance.
		GameObject();

		~GameObject();

		// ---------------------------------------------------

	public:
		WrenHandle* FindComponent(WrenVM* vm, WrenHandle* wrenClass) const ETNoexceptHint;

		bool HasComponent(WrenVM* vm, WrenHandle* wrenClass) const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		void InsertComponent(Context& vm, WrenHandle* wrenClass);

		void FreeComponents(Context& vm);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		GameObject& operator=(const GameObject&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		ArrayList<WrenHandle*> _components;
	};

	// ---

	class Game : public Networking::WorldEventClient {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		Game(const Game&) = delete;
		//!	Constructs this @ref Game instance.
		Game();

		~Game();

		// ---------------------------------------------------

	public:
		void HandlePlayerJoin(Dispatcher& dispatcher, StringView name);

		void HandlePlayerLeave(Dispatcher& dispatcher, StringView name);

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(Context& vm, Dispatcher& dispatcher);

		void FreeResources(Context& vm);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Game& operator=(const Game&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		WrenHandle*            _whenPlayerJoinsStub;
		WrenHandle*            _whenPlayerLeavesStub;
		SymbolTable<Utf8Char>  _tags;
		ArrayList<WrenHandle*> _gameObjects;

		// ---------------------------------------------------

		friend void Swap(Game&, Game&) = delete;
	};

}}} // namespace Eldritch2::Scripting::Wren
