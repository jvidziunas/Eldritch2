/*==================================================================*\
  EventCoordinator.hpp
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

	class EventCoordinator {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using GameTimestamp	= uint32;

	// ---

		enum : GameTimestamp {
			EndOfTime = static_cast<GameTimestamp>(-1)
		};

	// ---

		struct ScriptEvent {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref ScriptEvent instance.
			ScriptEvent( GameTimestamp dispatchTime, WrenHandle* handler );
		//!	Disable copy construction.
			ScriptEvent( const ScriptEvent& ) = default;
		//!	Constructs this @ref ScriptEvent instance.
			ScriptEvent() = default;

			~ScriptEvent();

		// ---------------------------------------------------

		public:
			bool	ShouldDispatch( GameTimestamp now ) const;

		// ---------------------------------------------------

		public:
			bool	Dispatch( WrenVM* wren, WrenHandle* callStub );

			void	FreeResources( WrenVM* wren );

		// ---------------------------------------------------

		public:
			ScriptEvent&	operator=( const ScriptEvent& ) = default;

		// - DATA MEMBERS ------------------------------------

		public:
			GameTimestamp	dispatchTime;
			WrenHandle*		receiver;
		};

	// ---

		using EventQueue	= PriorityQueue<ScriptEvent>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		EventCoordinator( const EventCoordinator& ) = delete;
	//!	Constructs this @ref EventCoordinator instance.
		EventCoordinator();

		~EventCoordinator();

	// ---------------------------------------------------

	public:
		GameTimestamp	GetNow() const;
		
	// ---------------------------------------------------

	public:
		void	SetPaused( bool value = true );

		bool	IsPaused() const;

	// ---------------------------------------------------

	public:
		void	CallAtGameTime( GameTimestamp time, WrenHandle* function );

		void	ProcessTick( WrenVM* wren, uint64 deltaMicroseconds );

	// ---------------------------------------------------

	public:
		ErrorCode	BindResources( WrenVM* wren );

		void		FreeResources( WrenVM* wren );

	// - DATA MEMBERS ------------------------------------

	private:
	/*!	Current 'age of the universe', in microseconds. Note that as a virtual value this can advance at a
		different rate relative to (or not at all, depending on pause state) a player's wall clock. */
		uint64		_gameTime;
		bool		_isPaused;

		WrenHandle*	_dispatchStub;
		EventQueue	_events;
	};

// ---

	ETPureFunctionHint EventCoordinator::GameTimestamp	AsTimestamp( uint64 microseconds );

	ETPureFunctionHint uint64							AsMicroseconds( EventCoordinator::GameTimestamp timestamp );

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/Wren/EventCoordinator.inl>
//------------------------------------------------------------------//