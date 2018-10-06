/*==================================================================*\
  Game.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/WrenInternal.hpp>
#include <Scripting/Wren/Dispatcher.hpp>
#include <Scripting/Wren/Context.hpp>
#include <Scripting/Wren/Game.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {

	namespace {

		template <typename InputIterator>
		ETInlineHint ETForceInlineHint InputIterator FindComponentByClass(WrenVM* vm, WrenHandle* wrenClass, InputIterator begin, InputIterator end) ETNoexceptHint {
			return FindIf(begin, end, [=](WrenHandle* component) {
				return wrenGetClassInline(vm, component->value) == AS_CLASS(wrenClass->value);
			});
		}

	} // anonymous namespace

	GameObject::GameObject() :
		_components(MallocAllocator("GameObject Component Allocator")) {}

	// ---------------------------------------------------

	GameObject::~GameObject() {
		ET_ASSERT(_components.IsEmpty(), "Leaking Wren game object components!");
	}

	// ---------------------------------------------------

	WrenHandle* GameObject::FindComponent(WrenVM* vm, WrenHandle* wrenClass) const ETNoexceptHint {
		const auto candidate(FindComponentByClass(vm, wrenClass, _components.Begin(), _components.End()));
		return candidate != _components.End() ? *candidate : nullptr;
	}

	// ---------------------------------------------------

	bool GameObject::HasComponent(WrenVM* vm, WrenHandle* wrenClass) const ETNoexceptHint {
		return FindComponentByClass(vm, wrenClass, _components.Begin(), _components.End()) == _components.End();
	}

	// ---------------------------------------------------

	void GameObject::InsertComponent(Context& vm, WrenHandle* wrenClass) {
		const auto existing(FindComponentByClass(vm, wrenClass, _components.Begin(), _components.End()));

		if (existing != _components.End()) {
			//	Discard the old component, replacing it with the passed-in object.
			wrenReleaseHandle(vm, eastl::exchange(*existing, wrenClass));
		}

		_components.Append(wrenClass);
	}

	// ---------------------------------------------------

	void GameObject::FreeComponents(Context& vm) {
		for (WrenHandle* component : _components) {
			wrenReleaseHandle(vm, component);
		}

		_components.Clear(ReleaseMemorySemantics());
	}

	// ---------------------------------------------------

	Game::Game() :
		_whenPlayerJoinsStub(nullptr),
		_whenPlayerLeavesStub(nullptr),
		_tags(MallocAllocator("Wren Game Entity Tag Allocator")),
		_gameObjects(MallocAllocator("Wren Game Object Collection Allocator")) {
	}

	// ---------------------------------------------------

	Game::~Game() {
		ET_ASSERT(_whenPlayerJoinsStub == nullptr, "Leaking Wren player join handler {}!", fmt::ptr(_whenPlayerJoinsStub));
		ET_ASSERT(_whenPlayerLeavesStub == nullptr, "Leaking Wren player leave handler {}!", fmt::ptr(_whenPlayerLeavesStub));
		ET_ASSERT(_gameObjects.IsEmpty(), "Leaking Wren game objects!");
	}

	// ---------------------------------------------------

	void Game::HandlePlayerJoin(Dispatcher& dispatcher, StringView name) {
		dispatcher.CallAtGameTime(0, _whenPlayerJoinsStub);
	}

	// ---------------------------------------------------

	void Game::HandlePlayerLeave(Dispatcher& dispatcher, StringView name) {
		dispatcher.CallAtGameTime(0, _whenPlayerLeavesStub);
	}

	// ---------------------------------------------------

	ErrorCode Game::BindResources(Context& vm, Dispatcher& /*dispatcher*/) {
		SymbolTable<Utf8Char>  tags(_tags.GetAllocator());
		ArrayList<WrenHandle*> gameObjects(_gameObjects.GetAllocator());
		ET_AT_SCOPE_EXIT(for (WrenHandle* object
							  : gameObjects) vm.Free(object));

		WrenHandle* whenPlayerJoinsStub(vm.BindCallStub("whenPlayerJoins(_)"));
		WrenHandle* whenPlayerLeavesStub(vm.BindCallStub("whenPlayerLeaves(_)"));

		/*	Import the boot script into the world scope. This will do things like configure callbacks and create the basic entity set
		 *	for gameplay. */
		ET_ABORT_UNLESS(vm.Interpret(R"(import "scripts/Boot" for Game)") ? Error::None : Error::Unspecified);

		Swap(_tags, tags);
		Swap(_gameObjects, gameObjects);

		return Error::None;
	}

	// ---------------------------------------------------

	void Game::FreeResources(Context& vm) {
		vm.Free(eastl::exchange(_whenPlayerJoinsStub, nullptr));
		vm.Free(eastl::exchange(_whenPlayerLeavesStub, nullptr));

		for (WrenHandle*& object : _gameObjects) {
			vm.Free(eastl::exchange(object, nullptr));
		}

		_gameObjects.Clear();
		_tags.Clear();
	}

}}} // namespace Eldritch2::Scripting::Wren
