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

namespace Eldritch2 {
	namespace Core {
		class	World;
	}
}

struct	WrenHandle;
struct	WrenVM;

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	class Game {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Game instance.
		Game( Core::World& world );
	//!	Disable copy construction.
		Game( const Game& ) = delete;

		~Game();

	// ---------------------------------------------------

	public:
		void	HandlePlayerJoin( WrenVM* vm, WrenHandle* unaryCallHandle, const Utf8Char* const name );

		void	HandlePlayerLeave( WrenVM* vm, WrenHandle* unaryCallHandle, const Utf8Char* const name );

	// ---------------------------------------------------

	public:
		void	FreeResources( WrenVM* vm );

	// ---------------------------------------------------

	public:
		double	GetTimeScalar() const;

		void	SetTimeScalar( double value );

	// ---------------------------------------------------

	public:
		void	SetPlayerJoinHandler( WrenVM* vm, WrenHandle* handler );

		void	SetPlayerLeaveHandler( WrenVM* vm, WrenHandle* handler );

	// ---------------------------------------------------

	public:
		void	ShutDown( bool andEngine ) const;

	// - DATA MEMBERS ------------------------------------

	private:
		Core::World*			_world;

		WrenHandle*				_playerJoinHandler;
		WrenHandle*				_playerLeaveHandler;

		SymbolTable<Utf8Char>	_entityTags;
		ArrayList<WrenHandle*>	_gameObjects;
	};

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2