/*==================================================================*\
  Dispatcher.hpp
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

namespace Eldritch2 { namespace Scripting { namespace Wren {
	class ScriptExecutor;
}}} // namespace Eldritch2::Scripting::Wren

struct WrenHandle;

namespace Eldritch2 { namespace Scripting { namespace Wren {

	enum class GameTime : uint64 {};

	class ScriptEvent {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ScriptEvent instance.
		ETConstexpr ScriptEvent(GameTime when, WrenHandle* receiver) ETNoexceptHint;
		//!	Disable copy construction.
		ETConstexpr ScriptEvent(const ScriptEvent&) ETNoexceptHint = delete;
		//!	Constructs this @ref ScriptEvent instance.
		ScriptEvent(ScriptEvent&&) ETNoexceptHint;
		//!	Constructs this @ref Event instance.
		ScriptEvent() ETNoexceptHint = default;

		~ScriptEvent();

		// ---------------------------------------------------

	public:
		bool ShouldDispatch(GameTime now) const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		//!	Disable copy assignment.
		ScriptEvent& operator=(const ScriptEvent&) ETNoexceptHint = delete;
		ScriptEvent& operator                                     =(ScriptEvent&&) ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	public:
		GameTime    when;
		WrenHandle* receiver;
	};

	// ---

	class Dispatcher {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using EventQueue = PriorityQueue<ScriptEvent, ArrayList<ScriptEvent, MallocAllocator>>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		Dispatcher(const Dispatcher&) = delete;
		//!	Constructs this @ref Dispatcher instance.
		Dispatcher() ETNoexceptHint;

		~Dispatcher();

		// ---------------------------------------------------

	public:
		ETConstexpr GameTime GetNow() const ETNoexceptHint;

		ETCpp14Constexpr MicrosecondTime SetGameTime(MicrosecondTime) ETNoexceptHint;

		// ---------------------------------------------------

	public:
		void CallAtGameTime(GameTime when, WrenHandle* receiver);

		void CallOnDelay(GameTime delay, WrenHandle* receiver);

		// ---------------------------------------------------

	public:
		ETCpp14Constexpr double SetTimeScalar(double) ETNoexceptHint;

		ETConstexpr double GetTimeScalar() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		void ExecuteScriptEvents(ScriptExecutor& context, MicrosecondTime tickDelta);

		// ---------------------------------------------------

	public:
		ETConstexpr bool ShouldShutDownWorld() const ETNoexceptHint;

		ETConstexpr void ShutDownWorld() ETNoexceptHint;

		// ---------------------------------------------------

	public:
		Result BindResources(ScriptExecutor& context, MicrosecondTime now);

		void FreeResources(ScriptExecutor& context) ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	private:
		bool            _requestedShutdown;
		double          _targetTimeScalar;
		MicrosecondTime _gameTime;
		EventQueue      _events;
	};

	// ---

	ETConstexpr ETPureFunctionHint MicrosecondTime AsMicrosecondTime(GameTime when) ETNoexceptHint;

	ETConstexpr ETPureFunctionHint GameTime AsGameTime(MicrosecondTime when) ETNoexceptHint;

}}} // namespace Eldritch2::Scripting::Wren

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/Wren/Dispatcher.inl>
//------------------------------------------------------------------//
