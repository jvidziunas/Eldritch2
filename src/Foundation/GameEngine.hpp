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
		class	SystemInterface;
		class	GameEngine;
	}

	namespace Scheduler {
		class	TaskScheduler;
	}

	namespace Utility {
		template <typename ResultObjectType>
		struct	Result;
	}

	class	ErrorCode;
	template <typename Iterator>
	class	Range;
}

#ifdef ERROR
#	if( ET_COMPILER_IS_MSVC )
	// Good old fashioned poor decisions from our friends in Redmond. Somebody thought defining ERROR as a macro was a great idea!
#		pragma push_macro( "ERROR" )
#	endif
#	undef ERROR
#	define CLEANUP_ERROR
#endif

namespace Eldritch2 {
namespace Foundation {

	namespace Configuration	= ::Eldritch2::Configuration;
	namespace Foundation	= ::Eldritch2::Foundation;
	namespace FileSystem	= ::Eldritch2::FileSystem;
	namespace Scripting		= ::Eldritch2::Scripting;
	namespace Scheduler		= ::Eldritch2::Scheduler;
	namespace Utility		= ::Eldritch2::Utility;

// ---------------------------------------------------

	class GameEngine : public Scripting::ReferenceTypeBase {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum LogMessageType : ::Eldritch2::uint32 {
			VERBOSE_WARNING = 0u,
			WARNING,
			ERROR,
			MESSAGE
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//! Constructs this @ref GameEngine instance.
		GameEngine( Foundation::SystemInterface& systemInterface, Scheduler::TaskScheduler& scheduler, FileSystem::ContentProvider& contentProvider, ::Eldritch2::Allocator& allocator );

		//! Destroys this @ref GameEngine instance.
		~GameEngine() = default;

	// ---------------------------------------------------

		int	ApplicationEntryPoint();

	// - LOGGING -----------------------------------------

		Foundation::Logger&	GetLoggerForMessageType( const LogMessageType type );

	// ---------------------------------------------------

		//! Retrieves a read-only view the @ref SystemInterface describing the hardware this @ref GameEngine instance is executing on.
		ETInlineHint const Foundation::SystemInterface&	GetSystemInterface() const;

		//! Retrieves a read-only view of the @ref TaskScheduler instance this @ref GameEngine runs threads on.
		ETInlineHint const Scheduler::TaskScheduler&	GetTaskScheduler() const;
		//! Retrieves a view of the @ref TaskScheduler instance this @ref GameEngine runs threads on.
		ETInlineHint Scheduler::TaskScheduler&			GetTaskScheduler();

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
			::Eldritch2::ErrorCode (*factoryFunction)( ::Eldritch2::Allocator&, Foundation::World&, void* );
			void*	userParameter;
		};

	// - DATA MEMBERS ------------------------------------

		::Eldritch2::ChildAllocator											_allocator;

		Foundation::FileAppendingLogger										_logger;

		Foundation::SystemInterface&										_systemInterface;
		Scheduler::TaskScheduler&											_scheduler;

		::Eldritch2::IntrusiveForwardList<Foundation::GameEngineService>	_attachedServices;
		::Eldritch2::ResizableArray<WorldViewFactory>						_worldViewFactories;
		size_t																_worldViewAllocationHintInBytes;

		FileSystem::ContentLibrary											_contentLibrary;

		Configuration::ConfigurablePODVariable<LogMessageType>				_logEchoThreshold;
		Configuration::ConfigurablePODVariable<size_t>						_taskArenaPerThreadAllocationSizeInBytes;

		::Eldritch2::IntrusiveForwardList<Foundation::World>				_attachedWorlds;

		ManagementService													_managementService;		

	// - FRIEND CLASS DECLARATION ------------------------

		friend class ::Eldritch2::Foundation::WorldViewFactoryPublishingInitializationVisitor;
		friend class ::Eldritch2::Foundation::GameEngineService;
		friend class ::Eldritch2::Foundation::WorldView;
		friend class ::Eldritch2::Foundation::World;
	};

}	// namespace Foundation
}	// namespace Eldritch2

#ifdef CLEANUP_ERROR
#	if( ET_COMPILER_IS_MSVC )
#		pragma pop_macro( "ERROR" )
#	endif
#	undef CLEANUP_ERROR
#endif

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Foundation/GameEngine.inl>
//------------------------------------------------------------------//