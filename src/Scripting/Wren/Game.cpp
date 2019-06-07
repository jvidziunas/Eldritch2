/*==================================================================*\
  Game.cpp
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
#include <Scripting/Wren/ScriptExecutor.hpp>
#include <Scripting/Wren/WrenInternal.hpp>
#include <Scripting/Wren/Dispatcher.hpp>
#include <Scripting/Wren/Game.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {

	GameObject::GameObject() ETNoexceptHint : _componentByClass(MallocAllocator("Wren Game Object Component/Class Map Allocator")) {}

	// ---------------------------------------------------

	GameObject::~GameObject() {
		ETAssert(_componentByClass.IsEmpty(), "Leaking Wren game object components!");
	}

	// ---------------------------------------------------

	WrenHandle* GameObject::FindComponent(WrenHandle* wrenClass) const ETNoexceptHint {
		const auto candidate(_componentByClass.Find(wrenClass));
		return candidate != _componentByClass.End() ? candidate->second : nullptr;
	}

	// ---------------------------------------------------

	bool GameObject::HasComponent(WrenHandle* wrenClass) const ETNoexceptHint {
		return _componentByClass.Contains(wrenClass);
	}

	// ---------------------------------------------------

	bool GameObject::BindComponent(ScriptExecutor& vm, WrenHandle* wrenClass) {
		if (!vm.New(wrenClass)) {
			return false;
		}

		const auto existing(_componentByClass.Find(wrenClass));
		if (existing == _componentByClass.End()) {
			_componentByClass.Emplace(wrenClass, nullptr);
		} else {
			vm.Free(Exchange(existing->second, wrenClass));
		}
	}

	// ---------------------------------------------------

	void GameObject::FreeComponents(ScriptExecutor& vm) ETNoexceptHint {
		_componentByClass.ClearAndDispose([&](WrenHandle* component) ETNoexceptHint { vm.Free(component); });
	}

	// ---------------------------------------------------

	Game::Game() ETNoexceptHint : _whenPlayerJoinsStub(nullptr),
								  _whenPlayerLeavesStub(nullptr),
								  _tags(MallocAllocator("Wren Game Object Tag Set Allocator")),
								  _gameObjects(MallocAllocator("Wren Game Object List Allocator")) {
	}

	// ---------------------------------------------------

	Game::~Game() {
		ETAssert(_whenPlayerJoinsStub == nullptr, "Leaking Wren player join handler {}!", fmt::ptr(_whenPlayerJoinsStub));
		ETAssert(_whenPlayerLeavesStub == nullptr, "Leaking Wren player leave handler {}!", fmt::ptr(_whenPlayerLeavesStub));
		ETAssert(_gameObjects.IsEmpty(), "Leaking Wren game objects!");
	}

	// ---------------------------------------------------

	void Game::QueueEvents(Dispatcher& /*dispatcher*/) {}

	// ---------------------------------------------------

	Result Game::BindResources(ScriptExecutor& vm) {
		using ::Eldritch2::Swap;

		SymbolSet<Utf8Char>  tags(_tags.GetAllocator());
		ArrayList<WrenHandle*> gameObjects(_gameObjects.GetAllocator());
		ET_AT_SCOPE_EXIT(gameObjects.ClearAndDispose(ReleaseMemorySemantics(), [&](WrenHandle* object) ETNoexceptHint { vm.Free(object); }));

		WrenHandle* whenPlayerJoinsStub(vm.BindCallStub("whenPlayerJoins(_)"));
		ET_AT_SCOPE_EXIT(vm.Free(whenPlayerJoinsStub));
		WrenHandle* whenPlayerLeavesStub(vm.BindCallStub("whenPlayerLeaves(_)"));
		ET_AT_SCOPE_EXIT(vm.Free(whenPlayerLeavesStub));

		/*	Import the boot script into the world scope. This will do things like configure callbacks and create the basic entity set
		 *	for gameplay. */
		ET_ABORT_UNLESS(vm.Interpret(R"(import "scripts/Boot" for Game)") ? Result::Success : Result::Unspecified);

		Swap(_tags, tags);
		Swap(_gameObjects, gameObjects);
		Swap(_whenPlayerJoinsStub, whenPlayerJoinsStub);
		Swap(_whenPlayerLeavesStub, whenPlayerLeavesStub);

		return Result::Success;
	}

	// ---------------------------------------------------

	void Game::FreeResources(ScriptExecutor& vm) ETNoexceptHint {
		vm.Free(Exchange(_whenPlayerJoinsStub, nullptr));
		vm.Free(Exchange(_whenPlayerLeavesStub, nullptr));

		_gameObjects.ClearAndDispose(ReleaseMemorySemantics(), [&](WrenHandle* object) ETNoexceptHint { vm.Free(object); });
		_tags.Clear();
	}

}}} // namespace Eldritch2::Scripting::Wren
