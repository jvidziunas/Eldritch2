/*==================================================================*\
  Engine.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Flatbuffers/FlatBufferPackageProvider.hpp>
#include <Core/EngineComponent.hpp>
#include <Logging/FileLog.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Core {

	class Engine {
		// - TYPE PUBLISHING ---------------------------------

	public:
		class ManagementComponent : public EngineComponent {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Disable copy construction.
			ManagementComponent(const ManagementComponent&) = delete;
			//!	Constructs this @ref ManagementComponent instance.
			ManagementComponent(Engine& owner);

			~ManagementComponent() = default;

			// - ENGINE SERVICE SANDBOX METHODS ------------------

		protected:
			void BindResourcesEarly(Scheduling::JobExecutor& executor) override;

			void PublishConfiguration(Core::PropertyRegistrar& properties) override;

			// - ENGINE SERVICE SANDBOX METHODS ------------------

		protected:
			void TickEarly(Scheduling::JobExecutor& executor) override;

			void Tick(Scheduling::JobExecutor& executor) override;

			// ---------------------------------------------------

			//!	Disable copy assignment.
			ManagementComponent& operator=(const ManagementComponent&) = delete;

			// - DATA MEMBERS ------------------------------------

		private:
			Engine* _owner;
			size_t  _packageSweepLimitPerFrame;
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		Engine(const Engine&) = delete;
		//!	Constructs this @ref Engine instance.
		Engine();

		~Engine() = default;

		// ---------------------------------------------------

	public:
		const ObjectLocator& GetServiceLocator() const;

		Logging::Log& GetLog() const;

		// - WORLD MANAGEMENT --------------------------------

	public:
		//!	Creates a new @ref World that will host a game instance.
		/*!	@param[in] executor @ref JobExecutor that will run the world's deserialization and initialization jobs.
			@returns @ref Errors::None if the world was instantiated successfully, or an @ref ErrorCode with information on why the operation failed.
			@remarks See world service implementations for examples of properties that may be configured here. */
		ErrorCode CreateWorld(Scheduling::JobExecutor& executor);

		// ---------------------------------------------------

	public:
		void SetShouldShutDown(MemoryOrder order = std::memory_order_release) const;

		bool ShouldRun(MemoryOrder order = std::memory_order_consume) const;

		// ---------------------------------------------------

	public:
		ErrorCode ApplyConfiguration(PlatformStringView filePath);

		// ---------------------------------------------------

	public:
		template <class... Components>
		ErrorCode BootOnCaller(Scheduling::JobExecutor& executor, Components&... components);
		ErrorCode BootOnCaller(Scheduling::JobExecutor& executor);

		// ---------------------------------------------------

	private:
		void RunFrame(Scheduling::JobExecutor& executor);

		void TickWorlds(Scheduling::JobExecutor& executor);

		void DestroyGarbage(size_t collectionLimit);

		// ---------------------------------------------------

	private:
		void BindComponents(Scheduling::JobExecutor& executor);

		void CreateBootWorld(Scheduling::JobExecutor& executor);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Engine& operator=(const Engine&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		mutable UsageMixin<MallocAllocator>    _allocator;
		ObjectLocator                          _services;
		mutable Logging::FileLog               _log;
		mutable Atomic<bool>                   _shouldRun;
		FlatBuffers::FlatBufferPackageProvider _packageProvider;
		ArrayList<EngineComponent*>            _components;
		mutable Mutex                          _worldsMutex;
		ArrayList<UniquePointer<World>>        _worlds;
	};

}} // namespace Eldritch2::Core

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Core/Engine.inl>
//------------------------------------------------------------------//
