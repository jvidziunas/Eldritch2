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

namespace Eldritch2 { namespace Core {
	class World;
}} // namespace Eldritch2::Core

struct WrenHandle;
struct WrenVM;

namespace Eldritch2 { namespace Scripting { namespace Wren {

	class Dispatcher {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using ScriptTime = uint32;

		// ---

		struct Event {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref Event instance.
			Event(ScriptTime dispatchTime, WrenHandle* receiver);
			//!	Disable copy construction.
			Event(const Event&) = default;
			//!	Constructs this @ref Event instance.
			Event() = default;

			~Event();

			// ---------------------------------------------------

		public:
			bool ShouldDispatch(ScriptTime now) const;

			// ---------------------------------------------------

		public:
			bool Call(WrenVM* wren, WrenHandle* arity0Stub);

			void FreeResources(WrenVM* wren);

			// ---------------------------------------------------

		public:
			Event& operator=(const Event&) = default;

			// - DATA MEMBERS ------------------------------------

		public:
			ScriptTime  when;
			WrenHandle* receiver;
		};

		// ---

	public:
		template <typename Allocator = MallocAllocator>
		using EventQueue = PriorityQueue<Event, ArrayList<Event, Allocator>>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref Dispatcher instance.
		Dispatcher(Core::World& world);
		//!	Disable copy construction.
		Dispatcher(const Dispatcher&) = delete;

		~Dispatcher();

		// ---------------------------------------------------

	public:
		ScriptTime GetNow() const;

		MicrosecondTime SetGameTime(MicrosecondTime microseconds);

		double GetWorldTimeScalar() const;

		void SetWorldTimeScalar(double value);

		// ---------------------------------------------------

	public:
		void CallAtGameTime(ScriptTime time, WrenHandle* receiver);

		void ExecuteScriptEvents(WrenVM* wren, MicrosecondTime deltaMicroseconds);

		// ---------------------------------------------------

	public:
		void ShutDownWorld(bool andEngine) const;

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(WrenVM* wren);

		void FreeResources(WrenVM* wren);

		// - DATA MEMBERS ------------------------------------

	private:
		Core::World* _world;
		/*!	Current 'age of the universe', in microseconds. Note that as a virtual value this can advance at a
			different rate relative to (or not at all, depending on pause state) a player's wall clock. */
		uint64       _gameTime;
		EventQueue<> _events;
	};

	// ---

	ETPureFunctionHint Dispatcher::ScriptTime AsScriptTime(MicrosecondTime when);

	ETPureFunctionHint MicrosecondTime AsMicroseconds(Dispatcher::ScriptTime when);

}}} // namespace Eldritch2::Scripting::Wren

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/Wren/Dispatcher.inl>
//------------------------------------------------------------------//
