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
		using Timestamp = uint32;
		enum : Timestamp { EndOfTime = static_cast<Timestamp>(-1) };

		// ---

		struct Event {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref Event instance.
			Event(Timestamp dispatchTime, WrenHandle* receiver);
			//!	Disable copy construction.
			Event(const Event&) = default;
			//!	Constructs this @ref Event instance.
			Event() = default;

			~Event();

			// ---------------------------------------------------

		public:
			bool ShouldDispatch(Timestamp now) const;

			// ---------------------------------------------------

		public:
			bool Call(WrenVM* wren, WrenHandle* arity0Stub);

			void FreeResources(WrenVM* wren);

			// ---------------------------------------------------

		public:
			Event& operator=(const Event&) = default;

			// - DATA MEMBERS ------------------------------------

		public:
			Timestamp   dispatchTime;
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
		Timestamp GetNow() const;

		uint64 SetGameTime(uint64 microseconds);

		double GetWorldTimeScalar() const;

		void SetWorldTimeScalar(double value);

		// ---------------------------------------------------

	public:
		void CallAtGameTime(Timestamp time, WrenHandle* receiver);

		void ProcessTick(WrenVM* wren, uint64 deltaMicroseconds);

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

	ETPureFunctionHint Dispatcher::Timestamp AsTimestamp(uint64 microseconds);

	ETPureFunctionHint uint64 AsMicroseconds(Dispatcher::Timestamp timestamp);

}}} // namespace Eldritch2::Scripting::Wren

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/Wren/Dispatcher.inl>
//------------------------------------------------------------------//
