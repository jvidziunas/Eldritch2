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
#include <Utility/Containers/IntrusiveVyukovMPSCQueue.hpp>
#include <Utility/Containers/IntrusiveForwardList.hpp>
#include <Configuration/ConfigurablePODVariable.hpp>
#include <Utility/Containers/ResizableArray.hpp>
#include <Foundation/GameEngineService.hpp>
#include <Logging/FileAppendingLogger.hpp>
#include <Scripting/ReferenceTypeBase.hpp>
#include <Packages/ContentLibrary.hpp>
#include <Foundation/World.hpp>
//------------------------------------------------------------------//
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Foundation {
		class	GameEngine;
	}

	namespace Scheduler {
		class	TaskScheduler;
	}

	namespace System {
		class	SystemInterface;
	}

	class	ErrorCode;
	class	Allocator;
	template <typename Iterator>
	class	Range;
}

namespace Eldritch2 {
namespace Foundation {

	class GameEngine : public Scripting::ReferenceTypeBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref GameEngine instance.
		GameEngine( System::SystemInterface& systemInterface, Scheduler::TaskScheduler& scheduler, FileSystem::ContentProvider& contentProvider, ::Eldritch2::Allocator& allocator );

		//! Destroys this @ref GameEngine instance.
		~GameEngine() = default;

	// ---------------------------------------------------

		int	ApplicationEntryPoint();

	// - LOGGING -----------------------------------------

		Foundation::Logger&	GetLoggerForMessageType( const Foundation::LogMessageType type );

	// ---------------------------------------------------

		//! Retrieves a read-only view of the @ref TaskScheduler instance this @ref GameEngine runs threads on.
		ETInlineHint const Scheduler::TaskScheduler&	GetTaskScheduler() const;
		//! Retrieves a view of the @ref TaskScheduler instance this @ref GameEngine runs threads on.
		ETInlineHint Scheduler::TaskScheduler&			GetTaskScheduler();

	// ---------------------------------------------------

		//! Retrieves a read-only view the @ref SystemInterface describing the hardware this @ref GameEngine instance is executing on.
		ETInlineHint const System::SystemInterface&	GetSystemInterface() const;

	// ---------------------------------------------------

	protected:
		::Eldritch2::ErrorCode	InstantiateViewsForWorld( ::Eldritch2::Allocator& allocator, Foundation::World& world );

		size_t					CalculateFrameArenaSizeInBytes() const;

		void					ClearAttachedServices();

		void					Dispose() override sealed;

	// - TYPE PUBLISHING ---------------------------------

	private:
		class ManagementService : public Foundation::GameEngineService {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref ManagementService instance.
			ManagementService( Foundation::GameEngine& owningEngine );

			//! Destroys this @ref ManagementService instance.
			~ManagementService() = default;

		// - DEBUG/LOGGING INFORMATION -----------------------

			const ::Eldritch2::UTF8Char* const	GetName() const override sealed;

		// ---------------------------------------------------

			void	BootstrapEngine( const size_t threadCount );

		// ---------------------------------------------------

		protected:
			void	AcceptInitializationVisitor( Configuration::ConfigurationPublishingInitializationVisitor& visitor ) override sealed;

			void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::Task& visitingTask, Scheduler::WorkerContext& executingContext, const InitializeEngineTaskVisitor ) override sealed;
			void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::Task& visitingTask, Scheduler::WorkerContext& executingContext, const WorldTickTaskVisitor ) override sealed;
		};

	// ---

		struct WorldViewFactory {
			using FactoryFunctionPointer	= ::Eldritch2::ErrorCode (*)( ::Eldritch2::Allocator&, Foundation::World&, void* );

		// ---------------------------------------------------

			FactoryFunctionPointer	factoryFunction;
			void*					userParameter;
		};

	// - DATA MEMBERS ------------------------------------

		::Eldritch2::ChildAllocator												_allocator;

		Foundation::FileAppendingLogger											_logger;
		FileSystem::ContentLibrary												_contentLibrary;

		System::SystemInterface&												_systemInterface;
		Scheduler::TaskScheduler&												_scheduler;

		::Eldritch2::IntrusiveForwardList<Foundation::GameEngineService>		_attachedServices;
		::Eldritch2::ResizableArray<WorldViewFactory>							_worldViewFactories;
		size_t																	_worldViewAllocationHintInBytes;

		Configuration::ConfigurablePODVariable<LogMessageType>					_logEchoThreshold;
		Configuration::ConfigurablePODVariable<size_t>							_taskArenaPerThreadAllocationSizeInBytes;

		::Eldritch2::IntrusiveVyukovMPSCQueue<Foundation::World>				_tickingWorlds;

		ManagementService														_managementService;		

	// - FRIEND CLASS DECLARATION ------------------------

		friend class ::Eldritch2::Foundation::WorldViewFactoryPublishingInitializationVisitor;
		friend class ::Eldritch2::Foundation::GameEngineService;
		friend class ::Eldritch2::Foundation::WorldView;
		friend class ::Eldritch2::Foundation::World;
	};

}	// namespace Foundation
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Foundation/GameEngine.inl>
//------------------------------------------------------------------//