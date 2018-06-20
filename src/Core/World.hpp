/*==================================================================*\
  World.hpp
  ------------------------------------------------------------------
  Purpose:
  Encapsulates all functionality and bookkeeping associated with
  running a game 'world'-- a global environment that contains a
  number of entities and their respective collections of constituent
  components.

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/TimeAccumulator.hpp>
#include <Core/WorldComponent.hpp>
#include <Logging/ChildLog.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scheduling {
	class JobExecutor;
}} // namespace Eldritch2::Scheduling

namespace Eldritch2 { namespace Core {

	class World {
		// - TYPE PUBLISHING ---------------------------------

	public:
		template <typename Allocator = MallocAllocator>
		using ComponentList = ArrayList<UniquePointer<WorldComponent>, MallocAllocator>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref World instance.
		World(const Blackboard& services, Logging::Log& log);
		//! Disable copy construction.
		World(const World&) = delete;

		~World() = default;

		// - STATE INSPECTION --------------------------------

	public:
		const ComponentList<>& GetComponents() const;

		const Blackboard& GetServices() const;

		Logging::Log& GetLog() const;

		// ---------------------------------------------------

	public:
		template <typename Iterator>
		ErrorCode BindResources(Scheduling::JobExecutor& executor, Iterator firstComponent, Iterator lastComponent);
		ErrorCode BindResources(Scheduling::JobExecutor& executor);

		void FreeResources(Scheduling::JobExecutor& executor);

		// ---------------------------------------------------

	public:
		void Tick(Scheduling::JobExecutor& executor);

		// ---------------------------------------------------

	public:
		float32 GetTimeScalar() const;

		void SetTimeScalar(float32 scalar);

		// ---------------------------------------------------

	public:
		bool ShouldShutDown(MemoryOrder order = std::memory_order_consume) const;

		bool ShouldPause(MemoryOrder order = std::memory_order_consume) const;

		bool IsRunning(MemoryOrder order = std::memory_order_consume) const;

		// ---------------------------------------------------

	public:
		//!	Notifies the world that it should cease updating as soon as possible.
		/*!	@param[in] andEngine boolean option indicating whether or not a shutdown request should be
				placed for the owning @ref Engine as well.
			@remarks Idempotent, thread-safe. */
		void SetShouldShutDown(bool andEngine = false) const;

		void SetShouldPause();

		// ---------------------------------------------------

		//!	Disable copy assignment.
		World& operator=(const World&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		mutable MallocAllocator   _allocator;
		mutable Logging::ChildLog _log;
		mutable Atomic<bool>      _shouldShutDown;
		Atomic<uint32>            _pauseCounter;
		Blackboard                _services;
		TimeAccumulator           _timeAccumulator;
		ComponentList<>           _components;
	};

}
} // namespace Eldritch2::Core

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Core/World.inl>
//------------------------------------------------------------------//
