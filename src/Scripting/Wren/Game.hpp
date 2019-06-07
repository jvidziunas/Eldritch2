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
	class ScriptExecutor;
	class Dispatcher;
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
		GameObject() ETNoexceptHint;

		~GameObject();

		// ---------------------------------------------------

	public:
		WrenHandle* FindComponent(WrenHandle* wrenClass) const ETNoexceptHint;

		bool HasComponent(WrenHandle* wrenClass) const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		bool BindComponent(ScriptExecutor& vm, WrenHandle* wrenClass);

		void FreeComponents(ScriptExecutor& vm) ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		GameObject& operator=(const GameObject&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		ArrayMap<WrenHandle*, WrenHandle*> _componentByClass;
	};

	// ---

	class Game : public Networking::WorldEventClient {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		Game(const Game&) = delete;
		//!	Constructs this @ref Game instance.
		Game() ETNoexceptHint;

		~Game();

		// ---------------------------------------------------

	public:
		void QueueEvents(Dispatcher& dispatcher);

		// ---------------------------------------------------

	public:
		Result BindResources(ScriptExecutor& vm);

		void FreeResources(ScriptExecutor& vm) ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Game& operator=(const Game&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		WrenHandle*            _whenPlayerJoinsStub;
		WrenHandle*            _whenPlayerLeavesStub;
		SymbolSet<Utf8Char>    _tags;
		ArrayList<WrenHandle*> _gameObjects;

		// ---------------------------------------------------

		friend void Swap(Game&, Game&) = delete;
	};

}}} // namespace Eldritch2::Scripting::Wren
