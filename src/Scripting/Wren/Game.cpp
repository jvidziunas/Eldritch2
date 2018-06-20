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
#include <Scripting/Wren/Game.hpp>

//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		namespace Wren {

			Game::Game(
			) : _game(nullptr),
				_whenPlayerJoinsStub(nullptr),
				_whenPlayerLeavesStub(nullptr),
				_tags(MallocAllocator("Wren Game Entity Tag Allocator")),
				_gameObjects(MallocAllocator("Wren Game Object Collection Allocator")) {
			}

		// ---------------------------------------------------

			Game::~Game() {
				ET_ASSERT(_game == nullptr, "Leaking Wren game handle!");
				ET_ASSERT(_whenPlayerJoinsStub == nullptr, "Leaking Wren player join handler!");
				ET_ASSERT(_whenPlayerLeavesStub == nullptr, "Leaking Wren player leave handler!");
				ET_ASSERT(_gameObjects.IsEmpty(), "Leaking Wren game objects!");
			}

		// ---------------------------------------------------

			WrenInterpretResult Game::HandlePlayerJoin(WrenVM* vm, const Utf8Char* const name) {
				wrenEnsureSlots(vm, 2);
				wrenSetSlotHandle(vm, 0, _game);
				wrenSetSlotString(vm, 1, name);
				return wrenCall(vm, _whenPlayerJoinsStub);
			}

		// ---------------------------------------------------

			WrenInterpretResult Game::HandlePlayerLeave(WrenVM* vm, const Utf8Char* const name) {
				wrenEnsureSlots(vm, 2);
				wrenSetSlotHandle(vm, 0, _game);
				wrenSetSlotString(vm, 1, name);
				return wrenCall(vm, _whenPlayerLeavesStub);
			}

		// ---------------------------------------------------

			ErrorCode Game::BindResources(WrenVM* vm) {
			/*	Import the boot script into the world scope. This will do things like configure callbacks and create the basic entity set
			 *	for gameplay. */
				if (wrenInterpret(vm, R"(import "scripts/Boot" for Game)") != WREN_RESULT_SUCCESS) {
					return Error::Unspecified;
				}

				wrenEnsureSlots(vm, 16);
				wrenGetVariable(vm, "main", "Game", 0);

				_game = wrenGetSlotHandle(vm, 0);
				_whenPlayerJoinsStub = wrenMakeCallHandle(vm, "whenPlayerJoins(_)");
				_whenPlayerLeavesStub = wrenMakeCallHandle(vm, "whenPlayerLeaves(_)");

				return Error::None;
			}

		// ---------------------------------------------------

			void Game::FreeResources(WrenVM* vm) {
				if (WrenHandle* const stub = eastl::exchange(_whenPlayerJoinsStub, nullptr)) {
					wrenReleaseHandle(vm, stub);
				}

				if (WrenHandle* const stub = eastl::exchange(_whenPlayerLeavesStub, nullptr)) {
					wrenReleaseHandle(vm, stub);
				}

				if (WrenHandle* const game = eastl::exchange(_game, nullptr)) {
					wrenReleaseHandle(vm, game);
				}

				while (_gameObjects) {
					wrenReleaseHandle(vm, _gameObjects.Back());
					_gameObjects.Pop();
				}

				_tags.Clear();
			}

		}	// namespace Wren
	}	// namespace Scripting
}	// namespace Eldritch2