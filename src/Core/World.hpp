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
		World(const ObjectLocator& services);
		//! Disable copy construction.
		World(const World&) = delete;

		~World() = default;

		// - STATE INSPECTION --------------------------------

	public:
		Range<const UniquePointer<WorldComponent>*> GetComponents() const ETNoexceptHint;

		ETConstexpr Logging::Log& GetLog() const ETNoexceptHint;

		float32 GetTimeScalar() const ETNoexceptHint;

		void SetTimeScalar(float32 scalar) ETNoexceptHint;

		// ---------------------------------------------------

	public:
		void Tick(Scheduling::JobExecutor& executor);

		// ---------------------------------------------------

	public:
		bool ShouldShutDown(MemoryOrder order = std::memory_order_consume) const ETNoexceptHint;

		//!	Notifies the world that it should cease updating as soon as possible.
		/*!	@param[in] andEngine boolean option indicating whether or not a shutdown request should be
				placed for the owning @ref Engine as well.
			@remarks Idempotent, thread-safe. */
		void SetShouldShutDown(bool andEngine = false) const ETNoexceptHint;

		bool ShouldRun(MemoryOrder order = std::memory_order_consume) const ETNoexceptHint;

		bool IsRunning(MemoryOrder order = std::memory_order_consume) const ETNoexceptHint;

		void SetShouldPause(MemoryOrder order = std::memory_order_relaxed) ETNoexceptHint;

		// ---------------------------------------------------

	public:
		template <typename Iterator>
		ErrorCode BindResources(Scheduling::JobExecutor& executor, Iterator firstComponent, Iterator lastComponent);
		ErrorCode BindResources(Scheduling::JobExecutor& executor);

		void FreeResources(Scheduling::JobExecutor& executor);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		World& operator=(const World&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		mutable MallocAllocator   _allocator;
		ObjectLocator             _services;
		mutable Logging::ChildLog _log;
		mutable Atomic<bool>      _shouldShutDown;
		Atomic<uint32>            _pauseCounter;
		TimeAccumulator           _timeAccumulator;
		ComponentList<>           _components;
	};

}} // namespace Eldritch2::Core

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Core/World.inl>
//------------------------------------------------------------------//
