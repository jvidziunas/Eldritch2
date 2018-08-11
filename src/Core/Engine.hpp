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
#include <Assets/FlatBufferPackageProvider.hpp>
#include <Core/EngineComponent.hpp>
#include <Logging/FileLog.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scheduling {
	class JobSystem;
}} // namespace Eldritch2::Scheduling

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
		//!	Constructs this @ref Engine instance.
		/*!	@param[in] scheduler @ref Scheduling::JobSystem instance the new @ref Engine will use to launch operating system threads. */
		Engine(Scheduling::JobSystem& scheduler);
		//!	Disable copy construction.
		Engine(const Engine&) = delete;

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
		void SetShouldShutDown() const;

		bool ShouldRun() const;

		// ---------------------------------------------------

	public:
		ErrorCode ApplyConfiguration(StringView<PlatformChar> path);

		// ---------------------------------------------------

	public:
		template <class... Components>
		int BootOnCaller(Scheduling::JobExecutor& executor, Components&... components);
		int BootOnCaller(Scheduling::JobExecutor& executor);

		// ---------------------------------------------------

	private:
		void RunFrame(Scheduling::JobExecutor& executor);

		void TickWorlds(Scheduling::JobExecutor& executor);

		void SweepPackages(size_t collectionLimit);

		// ---------------------------------------------------

	private:
		void ScanPackages();

		// ---------------------------------------------------

	private:
		void BindComponents(Scheduling::JobExecutor& executor);

		void CreateBootWorld(Scheduling::JobExecutor& executor);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Engine& operator=(const Engine&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		mutable UsageMixin<MallocAllocator> _allocator;
		ObjectLocator                       _services;
		Assets::FlatBufferPackageProvider   _packageProvider;
		mutable Logging::FileLog            _log;
		mutable Atomic<bool>                _shouldRun;
		mutable Mutex                       _worldsMutex;
		ArrayList<UniquePointer<World>>     _worlds;
		ArrayList<EngineComponent*>         _components;
	};

}} // namespace Eldritch2::Core

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Core/Engine.inl>
//------------------------------------------------------------------//
