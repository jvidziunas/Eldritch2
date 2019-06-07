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
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting { namespace Wren {
	class ApiBuilder;
}} // namespace Scripting::Wren

namespace Scheduling {
	class JobExecutor;
} // namespace Scheduling
} // namespace Eldritch2

namespace Eldritch2 { namespace Core {

	class ETPureAbstractHint AbstractWorld {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref AbstractWorld instance.
		AbstractWorld(const ObjectInjector& services) ETNoexceptHint;
		//! Disable copy construction.
		AbstractWorld(const AbstractWorld&) = delete;

		virtual ~AbstractWorld() = default;

		// - STATE INSPECTION --------------------------------

	public:
		//!	Notifies the world that it should cease updating as soon as possible.
		/*!	@remarks Idempotent, thread-safe. */
		void SetShouldShutDown(MemoryOrder order = std::memory_order_relaxed) const ETNoexceptHint;

		bool ShouldShutDown(MemoryOrder order = std::memory_order_consume) const ETNoexceptHint;

		bool ShouldSimulate(MemoryOrder order = std::memory_order_consume) const ETNoexceptHint;

		bool ShouldExecute(MemoryOrder order = std::memory_order_consume) const ETNoexceptHint;

		//!	Notifies the world that it should cease updating as soon as possible.
		/*!	@remarks Idempotent, thread-safe. */
		void SetShouldPause(MemoryOrder order = std::memory_order_relaxed) ETNoexceptHint;

		// ---------------------------------------------------

	public:
		void Tick(Scheduling::JobExecutor& executor) ETNoexceptHint;

		// ---------------------------------------------------

	public:
		template <typename ApiBuilder>
		ApiBuilder BuildApi(ApiBuilder api) const;

		virtual void BindResources(Scheduling::JobExecutor& executor) ETNoexceptHint abstract;

		virtual void FreeResources(Scheduling::JobExecutor& executor) ETNoexceptHint abstract;

		// ---------------------------------------------------

	protected:
		virtual void VariableTick(Scheduling::JobExecutor& executor, MicrosecondTime duration, float32 blendFraction) ETNoexceptHint abstract;

		virtual void FixedTick(Scheduling::JobExecutor& executor, MicrosecondTime duration) ETNoexceptHint abstract;

		// ---------------------------------------------------

	protected:
		virtual void PublishComponents(Scripting::Wren::ApiBuilder&) const abstract;

		virtual void PublishComponents(ObjectInjector&) const abstract;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		AbstractWorld& operator=(const AbstractWorld&) = delete;

		// - DATA MEMBERS ------------------------------------

	protected:
		ObjectInjector _services;

		// - DATA MEMBERS ------------------------------------

	private:
		mutable Atomic<bool> _shouldShutDown;
		Atomic<uint32>       _pauseCounter;
		TimeAccumulator      _timeAccumulator;
	};

	// ---

	template <typename... Components>
	class World : public AbstractWorld {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref World instance.
		World(const ObjectInjector& services) ETNoexceptHint;
		//! Disable copy construction.
		World(const World&) = delete;

		~World() override = default;

		// ---------------------------------------------------

	public:
		void BindResources(Scheduling::JobExecutor& executor) ETNoexceptHint override;

		void FreeResources(Scheduling::JobExecutor& executor) ETNoexceptHint override;

		// ---------------------------------------------------

	protected:
		void VariableTick(Scheduling::JobExecutor& executor, MicrosecondTime duration, float32 blendFraction) ETNoexceptHint override;

		void FixedTick(Scheduling::JobExecutor& executor, MicrosecondTime duration) ETNoexceptHint override;

		// ---------------------------------------------------

	protected:
		void PublishComponents(Scripting::Wren::ApiBuilder&) const override;

		void PublishComponents(ObjectInjector&) const override;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		AbstractWorld& operator=(const AbstractWorld&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		mutable Tuple<Components...> _components;

		// ---------------------------------------------------

		//!	Disable swap.
		void Swap(World&, World&) = delete;
	};

}} // namespace Eldritch2::Core

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Core/World.inl>
//------------------------------------------------------------------//
