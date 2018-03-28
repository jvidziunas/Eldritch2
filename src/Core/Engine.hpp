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

namespace Eldritch2 {
	namespace Scheduling {
		class	JobSystem;
	}
}

namespace Eldritch2 {
namespace Core {

	class Engine {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class ManagementComponent : public EngineComponent {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Disable copy construction.
			ManagementComponent( const ManagementComponent& ) = delete;
		//!	Constructs this @ref ManagementComponent instance.
			ManagementComponent( Engine& owner );

			~ManagementComponent() = default;

		// - ENGINE SERVICE SANDBOX METHODS ------------------

		protected:
			Result<UniquePointer<WorldComponent>>	CreateWorldComponent( Allocator& allocator, const World& world ) override;

			void									AcceptVisitor( Scheduling::JobExecutor& executor, const InitializationVisitor ) override;
			void									AcceptVisitor( Scheduling::JobExecutor& executor, const ServiceTickVisitor ) override;
			void									AcceptVisitor( Scheduling::JobExecutor& executor, const WorldTickVisitor ) override;
			void									AcceptVisitor( Core::PropertyRegistrar& properties ) override;

		// ---------------------------------------------------

		//!	Disable copy assignment.
			ManagementComponent&	operator=( const ManagementComponent& ) = delete;

		// - DATA MEMBERS ------------------------------------

		private:
		/*!	Cached reference to the game engine. This is used to avoid the blackboard lookups that would be otherwise necessary
			for almost every operation the @ref ManagementComponent performs. */
			Engine*	_owner;
		/*!	Limit on the number of unused asset packages that may be destroyed per game tick. Larger numbers may result in
			longer garbage collection pauses, but unreferenced resources will in turn be released more quickly. */
			size_t	_maxPackagesSweptPerFrame;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Engine instance.
	/*!	@param[in] scheduler @ref Scheduling::Scheduler instance the new @ref Engine will use to launch operating system threads. */
		Engine( Scheduling::JobSystem& scheduler );
	//!	Disable copy construction.
		Engine( const Engine& ) = delete;

		~Engine() = default;

	// ---------------------------------------------------

	public:
		const Blackboard&	GetBlackboard() const;

		Logging::Log&		GetLog() const;

		Allocator&			GetAllocator() const;

	// - WORLD MANAGEMENT --------------------------------

	public:
	//!	Creates a new @ref World that will host a game instance.
	/*!	@param[in] executor @ref JobExecutor that will run the world's deserialization and initialization jobs.
		@returns @ref Errors::None if the world was instantiated successfully, or an @ref ErrorCode with information on why the operation failed.
		@remarks See world service implementations for examples of properties that may be configured here. */
		ErrorCode	CreateWorld( Scheduling::JobExecutor& executor );

	// ---------------------------------------------------

	public:
		void	SetShouldShutDown() const;

		bool	ShouldRun() const;

	// ---------------------------------------------------

	public:
		ErrorCode	ApplyConfiguration( const Utf8Char* const path );

	// ---------------------------------------------------

	public:
		template <class... Components>
		int	BootOnCaller( Scheduling::JobExecutor& executor, Components&... components );
		int	BootOnCaller( Scheduling::JobExecutor& executor );

	// ---------------------------------------------------

	private:
		void	TickWorlds( Scheduling::JobExecutor& executor );

	// ---------------------------------------------------

	private:
		void	SweepPackages( size_t collectionLimit );

		void	BuildAssetApi();

		void	ScanPackages();

	// ---------------------------------------------------

	private:
		void	InitializeComponents( Scheduling::JobExecutor& executor );

		void	CreateBootWorld( Scheduling::JobExecutor& executor );

	// ---------------------------------------------------

	private:
		void	RunFrame( Scheduling::JobExecutor& executor );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		Engine&	operator=( const Engine& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
	/*!	Mutable, as objects allocated from the allocator are not conceptually a part of the game engine's state.
		@see Engine::GetAllocator() */
		mutable UsageMixin<MallocAllocator>	_allocator;
		Blackboard							_services;
		Assets::FlatBufferPackageProvider	_packageProvider;
	/*!	Mutable so logs can be written to in const methods.
		@see Engine::GetLog() */
		mutable Logging::FileLog			_log;
	//!	Mutable so requests to shut down the application can be made with a const reference to the engine.
		mutable Atomic<bool>				_shouldRun;
	//!	Mutex responsible for protecting the @ref _worlds collection.
		mutable Mutex						_worldsMutex;
		ArrayList<UniquePointer<World>>		_worlds;
	//!	List of @ref EngineComponent instances the @ref Engine uses to implement its functionality.
		ArrayList<EngineComponent*>			_components;
	};

}	// namespace Core
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Core/Engine.inl>
//------------------------------------------------------------------//