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
#include <Platform/UserReadWriteMutex.hpp>
#include <Platform/ContentProvider.hpp>
#include <Logging/FileAppenderLog.hpp>
#include <Core/ServiceBlackboard.hpp>
#include <Utility/UniquePointer.hpp>
#include <Assets/AssetLibrary.hpp>
#include <Core/EngineService.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scheduling {
		class	ThreadScheduler;
	}

	namespace Platform {
		class	PlatformInterface;
	}
}

namespace Eldritch2 {
namespace Core {

	class Engine {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class ManagementService : public Core::EngineService {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref ManagementService instance.
			ManagementService( Engine& engine );
		//!	Disable copying.
			ManagementService( const ManagementService& ) = delete;

			~ManagementService() = default;

		// - DEBUG/LOGGING INFORMATION -----------------------

		public:
			Eldritch2::Utf8Literal	GetName() const override sealed;

		// ---------------------------------------------------

		protected:
			void	AcceptVisitor( Scheduling::JobFiber& executor, const BeginInitializationVisitor ) override;
			void	AcceptVisitor( Scheduling::JobFiber& executor, const WorldTickVisitor ) override;
			void	AcceptVisitor( Configuration::ConfigurationRegistrar& registrar ) override;

		// ---------------------------------------------------

		//!	Disable assignment.
			ManagementService&	operator=( const ManagementService& ) = delete;

		// - DATA MEMBERS ------------------------------------

		private:
		/*!	'Cached' reference to the game engine. This is used to avoid the blackboard lookups that would be otherwise necessary
			for almost every operation the @ref ManagementService performs. */
			Engine&	_engine;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Engine instance.
	/*!	@param[in] platformInterface @ref Platform::PlatformInterface instance the new @ref Engine will use to interact with the operating system.
		@param[in] scheduler @ref Scheduling::ThreadScheduler instance the new @ref Engine will use to launch operating system threads.
		@param[in] allocator @ref Allocator the new @ref Engine will use to perform internal allocations. */
		Engine( Platform::PlatformInterface& platformInterface, Scheduling::ThreadScheduler& scheduler, Eldritch2::Allocator& allocator );
	//!	Disable copying.
		Engine( const Engine& ) = delete;

		~Engine() = default;

	// ---------------------------------------------------

	public:
		template <typename... Arguments>
		void	VisitServices( Scheduling::JobFiber& executor, Arguments&&... arguments );
		template <typename... Arguments>
		void	VisitServices( Arguments&&... arguments );

		template <typename... Arguments>
		void	VisitWorlds( Scheduling::JobFiber& executor, Arguments&&... arguments );
		template <typename... Arguments>
		void	VisitWorlds( Arguments&&... arguments );

	// ---------------------------------------------------

	public:
	//!	Creates a new @ref World that will host a game instance.
	/*!	@param[in] customProperties Set of custom string properties to be set on the new @ref World during creation.
		@returns @ref Errors::None if the world was instantiated succesfully, or an @ref ErrorCode with information on why the operation failed.
		@remarks See world service implementations for examples of properties that may be configured here. */
		Eldritch2::ErrorCode	CreateWorld( std::initializer_list<Eldritch2::Pair<const Eldritch2::Utf8Char*, const Eldritch2::Utf8Char*>> customProperties );

	// ---------------------------------------------------

	public:
		const Core::ServiceBlackboard&	GetServiceBlackboard() const;

		Eldritch2::Allocator&			GetAllocator() const;

		Logging::Log&					GetLog() const;

	// ---------------------------------------------------

	public:
		bool	HasShutDown() const;

		void	ShutDown() const;

	// ---------------------------------------------------

	public:
		void	InitializeWithServices( Scheduling::JobFiber& executor, std::initializer_list<std::reference_wrapper<Core::EngineService>> services );

		void	EnterMainLoopOnCaller( Scheduling::JobFiber& executor );

		size_t	DestroyShutDownWorlds();

	// ---------------------------------------------------

	//!	Disable assignment.
		Engine&	operator=( const Engine& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
	/*!	Mutable, as objects allocated from the allocator are not conceptually a part of the game engine's state.
		@see Engine::GetAllocator() */
		mutable Eldritch2::ChildAllocator										_allocator;
	/*!	Game engine services use this blackboard to share pieces of themselves with other services in a controlled fashion.
		@see EngineService::AcceptVisitor( Core::ServiceBlackboard& )
		@see Engine::GetServiceBlackboard() */
		Core::ServiceBlackboard													_blackboard;
		Platform::ContentProvider												_contentProvider;

	/*!	Mutable so logs can be written even if you only have a const reference to the engine.
		@see Engine::GetLog() */
		mutable Logging::FileAppenderLog										_log;
		Assets::AssetLibrary													_assetLibrary;

	//!	Collection of string properties that will be set on a new @ref World instance during its creation.
		Eldritch2::HashMap<Eldritch2::Utf8String<>, Eldritch2::Utf8String<>>	_worldDefaultProperties;

	//!	Mutable so requests to shut down the application can be made even if callers only have a const reference to the engine.
		mutable std::atomic<bool>												_hasShutDown;
		std::atomic<bool>														_hasInitialized;

	//!	Mutex responsible for protecting the @ref _worlds collection.
		mutable Platform::UserReadWriteMutex									_worldMutex;
		Eldritch2::ResizableArray<Eldritch2::UniquePointer<Core::World>>		_worlds;

	//!	Collection of @ref GameEngineServices instances the @ref Engine uses to implement most functionality.
		Eldritch2::UniquePointer<Core::EngineService*[]>						_services;
	};

}	// namespace Core
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Core/Engine.inl>
//------------------------------------------------------------------//