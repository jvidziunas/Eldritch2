/*==================================================================*\
  GameEngine.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/IntrusiveForwardList.hpp>
#include <Configuration/ConfigurablePODVariable.hpp>
#include <Utility/Containers/IntrusiveList.hpp>
#include <Foundation/GameEngineService.hpp>
#include <Logging/FileAppendingLogger.hpp>
#include <Packages/ContentLibrary.hpp>
#include <Foundation/World.hpp>
//------------------------------------------------------------------//
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scheduler {
		class	ThreadScheduler;
	}

	namespace System {
		class	SystemInterface;
	}

	class	Allocator;
}

namespace Eldritch2 {
namespace Foundation {

	class GameEngine {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref GameEngine instance.
		GameEngine( System::SystemInterface& systemInterface, Scheduler::ThreadScheduler& scheduler, FileSystem::ContentProvider& contentProvider, ::Eldritch2::Allocator& allocator );

		~GameEngine() = default;

	// ---------------------------------------------------

		int	ApplicationEntryPoint();

	// - LOGGING -----------------------------------------

		Foundation::Logger&	GetLoggerForMessageType( const Foundation::LogMessageType type ) const;

	// ---------------------------------------------------

		ETInlineHint const ::Eldritch2::IntrusiveForwardList<Foundation::GameEngineService>&	GetAttachedServices() const;
		ETInlineHint ::Eldritch2::IntrusiveForwardList<Foundation::GameEngineService>&			GetAttachedServices();

		ETInlineHint const FileSystem::ContentLibrary&											GetContentLibrary() const;
		ETInlineHint FileSystem::ContentLibrary&												GetContentLibrary();

		//! Retrieves the @ref ThreadScheduler instance this @ref GameEngine runs threads on.
		ETInlineHint const Scheduler::ThreadScheduler&											GetThreadScheduler() const;
		//! Retrieves the @ref ThreadScheduler instance this @ref GameEngine runs threads on.
		ETInlineHint Scheduler::ThreadScheduler&												GetThreadScheduler();

		//! Retrieves a read-only view the @ref SystemInterface describing the hardware this @ref GameEngine instance is executing on.
		ETInlineHint const System::SystemInterface&												GetSystemInterface() const;

		ETInlineHint ::Eldritch2::Allocator&													GetAllocator();

		ETInlineHint size_t																		GetWorldArenaSizeInBytes() const;

	// ---------------------------------------------------

		ETInlineHint void	NotifyOfNewService( Foundation::GameEngineService& service );

		void				NotifyOfNewWorld( Foundation::World& world );

		void				NotifyOfWorldDestruction( Foundation::World& world );

		void				ClearAttachedServices();

		void				ClearAttachedWorlds();

	// - TYPE PUBLISHING ---------------------------------

	private:
		class ManagementService : public Foundation::GameEngineService {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref ManagementService instance.
			ManagementService( Foundation::GameEngine& owningEngine );

			~ManagementService() = default;

		// - DEBUG/LOGGING INFORMATION -----------------------

			const ::Eldritch2::UTF8Char* const	GetName() const override sealed;

		// ---------------------------------------------------

			void	InitializeEngineAndLaunchFrameLoop( Scheduler::WorkerContext& executingContext );

		// ---------------------------------------------------

		protected:
			void	OnEngineInitializationStarted( Scheduler::WorkerContext& executingContext ) override sealed;

			void	AcceptInitializationVisitor( Configuration::ConfigurationPublishingInitializationVisitor& visitor ) override sealed;

		// ---------------------------------------------------
			
			void	OnWorldTickStarted( Scheduler::WorkerContext& executingContext ) override sealed;
		};

	// - DATA MEMBERS ------------------------------------

		::Eldritch2::ChildAllocator											_allocator;

		// Mutable so logs can be written even if we only have a const reference to the engine.
		mutable Foundation::FileAppendingLogger								_logger;
		FileSystem::ContentLibrary											_contentLibrary;

		System::SystemInterface&											_systemInterface;
		Scheduler::ThreadScheduler&											_scheduler;

		Configuration::ConfigurablePODVariable<LogMessageType>				_logEchoThreshold;
		Configuration::ConfigurablePODVariable<size_t>						_worldArenaSizeInBytes;

		::Eldritch2::AlignedInstancePointer<Utility::ReaderWriterUserMutex>	_worldMutex;
		::Eldritch2::IntrusiveList<Foundation::World>						_worlds;
		::std::atomic<size_t>												_worldCount;

		::Eldritch2::IntrusiveForwardList<Foundation::GameEngineService>	_attachedServices;
		ManagementService													_managementService;
	};

}	// namespace Foundation
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Foundation/GameEngine.inl>
//------------------------------------------------------------------//