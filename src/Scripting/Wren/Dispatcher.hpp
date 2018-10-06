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
	class Context;
}}} // namespace Eldritch2::Scripting::Wren

struct WrenHandle;

namespace Eldritch2 { namespace Scripting { namespace Wren {

	class ScriptEvent {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using Time = uint64;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ScriptEvent instance.
		ETConstexpr ScriptEvent(Time when, WrenHandle* receiver) ETNoexceptHint;
		//!	Disable copy construction.
		ETConstexpr ScriptEvent(const ScriptEvent&) ETNoexceptHint = delete;
		//!	Constructs this @ref ScriptEvent instance.
		ScriptEvent(ScriptEvent&&) ETNoexceptHint;
		//!	Constructs this @ref Event instance.
		ScriptEvent() ETNoexceptHint = default;

		~ScriptEvent();

		// ---------------------------------------------------

	public:
		bool ShouldDispatch(Time now) const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		ScriptEvent& operator=(const ScriptEvent&) ETNoexceptHint = delete;

		// - DATA MEMBERS ------------------------------------

	public:
		Time        when;
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
		Dispatcher();

		~Dispatcher();

		// ---------------------------------------------------

	public:
		ETConstexpr ScriptEvent::Time GetNow() const ETNoexceptHint;

		ETConstexpr MicrosecondTime SetGameTime(MicrosecondTime) ETNoexceptHint;

		// ---------------------------------------------------

	public:
		void CallAtGameTime(ScriptEvent::Time when, WrenHandle* receiver);

		void CallOnDelay(ScriptEvent::Time delay, WrenHandle* receiver);

		// ---------------------------------------------------

	public:
		ETConstexpr double GetTimeScalar() const ETNoexceptHint;

		ETConstexpr double SetTimeScalar(double) ETNoexceptHint;

		// ---------------------------------------------------

	public:
		void ExecuteScriptEvents(Context& context, MicrosecondTime tickDelta);

		// ---------------------------------------------------

	public:
		ETConstexpr bool ShouldShutDownWorld() const ETNoexceptHint;

		ETConstexpr void ShutDownWorld() ETNoexceptHint;

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(Context& context, double timeScalar, MicrosecondTime now = 0u);

		void FreeResources(Context& context);

		// - DATA MEMBERS ------------------------------------

	private:
		bool            _requestedShutdown;
		double          _timeScalar;
		MicrosecondTime _gameTime;
		EventQueue      _events;
	};

	// ---

	ETConstexpr ETPureFunctionHint ScriptEvent::Time AsScriptTime(MicrosecondTime when) ETNoexceptHint;

	ETConstexpr ETPureFunctionHint MicrosecondTime AsMicroseconds(ScriptEvent::Time when) ETNoexceptHint;

}}} // namespace Eldritch2::Scripting::Wren

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/Wren/Dispatcher.inl>
//------------------------------------------------------------------//
