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
#include <Core/World.hpp>
//------------------------------------------------------------------//
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;

	Game::Game(
		World& world
	) : _world( eastl::addressof( world ) ),
		_playerJoinHandler( nullptr ),
		_playerLeaveHandler( nullptr ),
		_entityTags( MallocAllocator( "Wren Game Entity Tag Allocator" ) ),
		_gameObjects( MallocAllocator( "Wren Game Object Collection Allocator" ) ) {
	}

// ---------------------------------------------------

	Game::~Game() {
		ET_ASSERT( _gameObjects.IsEmpty(), "Leaking Wren game objects!" );
	}

// ---------------------------------------------------

	void Game::HandlePlayerJoin( WrenVM* vm, WrenHandle* unaryCallHandle, const Utf8Char* const name ) {
		wrenEnsureSlots( vm, 2 );
		wrenSetSlotHandle( vm, 0, _playerJoinHandler );
		wrenSetSlotString( vm, 1, name );
		wrenCall( vm, unaryCallHandle );
	}

// ---------------------------------------------------

	void Game::HandlePlayerLeave( WrenVM* vm, WrenHandle* unaryCallHandle, const Utf8Char* const name ) {
		wrenEnsureSlots( vm, 2 );
		wrenSetSlotHandle( vm, 0, _playerLeaveHandler );
		wrenSetSlotString( vm, 1, name );
		wrenCall( vm, unaryCallHandle );
	}

// ---------------------------------------------------

	void Game::FreeResources( WrenVM* vm ) {
		SetPlayerJoinHandler( vm, nullptr );
		SetPlayerLeaveHandler( vm, nullptr );

		while (_gameObjects) {
			wrenReleaseHandle( vm, _gameObjects.Back() );
			_gameObjects.Pop();
		}

		_entityTags.Clear();
	}

// ---------------------------------------------------

	void Game::SetPlayerJoinHandler( WrenVM* vm, WrenHandle* handler ) {
		if (WrenHandle* const old = eastl::exchange( _playerJoinHandler, handler )) {
			wrenReleaseHandle( vm, old );
		}
	}

// ---------------------------------------------------

	void Game::SetPlayerLeaveHandler( WrenVM* vm, WrenHandle* handler ) {
		if (WrenHandle* const old = eastl::exchange( _playerLeaveHandler, handler )) {
			wrenReleaseHandle( vm, old );
		}
	}

// ---------------------------------------------------

	double Game::GetTimeScalar() const {
		return _world->GetTimeScalar();
	}

// ---------------------------------------------------

	void Game::SetTimeScalar( double value ) {
		_world->SetTimeScalar( static_cast<float32>(value) );
	}

// ---------------------------------------------------

	void Game::ShutDown( bool andEngine ) const {
		_world->SetShouldShutDown( andEngine );
	}

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2