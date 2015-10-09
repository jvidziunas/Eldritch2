/*==================================================================*\
  GameEngineService.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/IntrusiveForwardListHook.hpp>
#include <Utility/MPL/Noncopyable.hpp>
#include <Utility/MPL/CharTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Configuration {
		class	ConfigurationPublishingInitializationVisitor;
	}

	namespace Foundation {
		class	WorldViewFactoryPublishingInitializationVisitor;
		class	GameEngine;
		class	World;
	}

	namespace FileSystem {
		class	ResourceViewFactoryPublishingInitializationVisitor;
		class	ContentLibrary;
	}

	namespace Scripting {
		class	ScriptAPIRegistrationInitializationVisitor;
	}

	namespace Scheduler {
		class	WorkerContext;
		class	TaskScheduler;
		class	Thread;
		class	Task;
	}

	namespace Utility {
		template <typename ResultObject>
		struct	DisposingResultPair;
	}

	class	ErrorCode;
	class	Allocator;
}

namespace Eldritch2 {
namespace Foundation {

	namespace Configuration	= ::Eldritch2::Configuration;
	namespace Foundation	= ::Eldritch2::Foundation;
	namespace FileSystem	= ::Eldritch2::FileSystem;
	namespace Scripting		= ::Eldritch2::Scripting;
	namespace Scheduler		= ::Eldritch2::Scheduler;
	namespace Utility		= ::Eldritch2::Utility;

// ---------------------------------------------------

	class ETPureAbstractHint GameEngineService : public ::Eldritch2::IntrusiveForwardListBaseHook {
	// - TYPE PUBLISHING ---------------------------------

	public:
		struct PostInitializationVisitor {};
		
		struct InitializeEngineTaskVisitor {};
		struct PreConfigurationLoadedTaskVisitor {};
		struct PostConfigurationLoadedTaskVisitor {};

		struct ServiceTickTaskVisitor {};
		struct WorldTickTaskVisitor {};

	// - DEBUG/LOGGING INFORMATION -----------------------

		virtual const ::Eldritch2::UTF8Char* const	GetName() const abstract;

	// ---------------------------------------------------

	protected:
		//! Interested service classes should override this method in order to participate in script API setup.
		/*!	@param[in] visitor The type registrar that will handle publishing the service's types, methods and variables to script objects.
			@remark The default implementation does nothing.
			@see Scripting::ScriptAPIRegistrationInitializationVisitor
			*/
		virtual void	AcceptInitializationVisitor( Scripting::ScriptAPIRegistrationInitializationVisitor& visitor );
		//! Interested service classes should override this method in order to participate in configurable variable setup.
		/*!	@remark The default implementation does nothing.
			*/
		virtual void	AcceptInitializationVisitor( Configuration::ConfigurationPublishingInitializationVisitor& visitor );
		//! Interested service classes should override this method in order to participate in resource/world view creation.
		/*!	@remark The default implementation does nothing.
			*/
		virtual void	AcceptInitializationVisitor( Foundation::WorldViewFactoryPublishingInitializationVisitor& visitor );
		//! Interested service classes should override this method in order to participate in resource/world view creation.
		/*!	@remark The default implementation does nothing.
			*/
		virtual void	AcceptInitializationVisitor( FileSystem::ResourceViewFactoryPublishingInitializationVisitor& visitor );
		//! Interested service classes should override this method in order to perform any final initialization that interacts with other attached services.
		/*!	@remark The default implementation does nothing.
			*/
		virtual void	AcceptInitializationVisitor( const PostInitializationVisitor );

		//! Interested service classes should override this method to schedule simple bootstrap tasks with no additional data dependencies.
		/*!	@param[in] subtaskAllocator Allocator instance that provides memory for possible Task instances. The managed memory will exist until the end of the initialization process.
			@param[in] visitingTask The Scheduler::Task instance responsible for performing initial service initialization. Services are encouraged to add codependent Task instances to perform additional work.
			@param[in] executingContext The Scheduler::WorkerContext instance currently executing the visiting Task.
			@remark The default implementation does nothing.
			@see @ref Scheduler::Task, @ref Scheduler::WorkerContext
			*/
		virtual void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::Task& visitingTask, Scheduler::WorkerContext& executingContext, const InitializeEngineTaskVisitor );
		//! Interested service classes should override this method to schedule initialization tasks that do not require the use of any user-configurable variables.
		/*!	@param[in] subtaskAllocator @ref Allocator instance that provides memory for possible Task instances. The managed memory will exist until the end of the initialization process.
			@param[in] visitingTask The Scheduler::Task instance responsible for performing initial service initialization. Services are encouraged to add codependent Task instances to perform additional work.
			@param[in] executingContext The Scheduler::WorkerContext instance currently executing the visiting Task.
			@remark The default implementation does nothing.
			@see @ref Scheduler::Task, @ref Scheduler::WorkerContext
			*/
		virtual void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::Task& visitingTask, Scheduler::WorkerContext& executingContext, const PreConfigurationLoadedTaskVisitor );
		//! Interested service classes should override this method to schedule initialization tasks that require the use of user-configurable variables.
		/*!	@param[in] subtaskAllocator Allocator instance that provides memory for possible Task instances. The managed memory will exist until the end of the initialization process.
			@param[in] visitingTask The Scheduler::Task instance responsible for performing initial service initialization. Services are encouraged to add codependent Task instances to perform additional work.
			@param[in] executingContext The Scheduler::WorkerContext instance currently executing the visiting Task.
			@remark The default implementation does nothing.
			@see @ref Scheduler::Task, @ref Scheduler::WorkerContext
			*/
		virtual void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::Task& visitingTask, Scheduler::WorkerContext& executingContext, const PostConfigurationLoadedTaskVisitor );
		/*!	@param[in] subtaskAllocator Allocator instance that provides memory for possible Task instances. The managed memory will exist until the end of the initialization process.
			@param[in] visitingTask The Scheduler::Task instance responsible for performing initial service initialization. Services are encouraged to add codependent Task instances to perform additional work.
			@param[in] executingContext The Scheduler::WorkerContext instance currently executing the visiting Task.
			@remark The default implementation does nothing.
			@see @ref Scheduler::Task, @ref Scheduler::WorkerContext
			*/
		virtual void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::Task& visitingTask, Scheduler::WorkerContext& executingContext, const ServiceTickTaskVisitor );
		/*!	@param[in] subtaskAllocator Allocator instance that provides memory for possible Task instances. The managed memory will exist until the end of the initialization process.
			@param[in] visitingTask The Scheduler::Task instance responsible for performing initial service initialization. Services are encouraged to add codependent Task instances to perform additional work.
			@param[in] executingContext The Scheduler::WorkerContext instance currently executing the visiting Task.
			@remark The default implementation does nothing.
			@see @ref Scheduler::Task, @ref Scheduler::WorkerContext
			*/
		virtual void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::Task& visitingTask, Scheduler::WorkerContext& executingContext, const WorldTickTaskVisitor );

	// ---------------------------------------------------

		template <typename InitializationVisitor>
		void	BroadcastInitializationVisitor( InitializationVisitor&& visitor );

		template <typename TaskVisitor>
		void	BroadcastTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::Task& visitingTask, Scheduler::WorkerContext& executingContext, TaskVisitor&& visitor );

	// - GAME ENGINE SERVICE SANDBOX METHODS -------------

		//! Creates a new game world using an anonymous editor package.
		/*!	@remark Use @ref CreateWorld() for normal game loads, etc.
			@see @ref CreateWorld(), @ref ContentLibrary::CreatePackageForEditorWorld()
			*/
		Utility::DisposingResultPair<Foundation::World>	CreateEditorWorld();

		//! Creates a new game world using an anonymous editor package.
		/*!	@param[in] worldResourceName Null-terminated C string containing the name of the root content package for the world. This will be passed to @ref FileSystem::ContentLibrary::ResolvePackageByName()
			@remark Use @ref CreateEditorWorld() for making new worlds for use with the editor tool.
			@see @ref CreateEditorWorld(), @ref FileSystem::ContentPackage, @ref FileSystem::ContentLibrary::ResolvePackageByName()
			*/
		Utility::DisposingResultPair<Foundation::World>	CreateWorld( const ::Eldritch2::UTF8Char* const worldResourceName );

		//! Retrieves a read-only view of the @ref GameEngine instance that owns this @ref GameEngineService.
		ETInlineHint const Foundation::GameEngine&		GetHostingEngine() const;

		//! Retrieves a read-only view of the @ref FileSystem::ContentLibrary instance owned by the @ref GameEngine this @ref GameEngineService is attached to.
		/*!	@see @ref FileSystem::ContentLibrary
			*/
		const FileSystem::ContentLibrary&				GetEngineContentLibrary() const;

		//! Retrieves the Scheduler::TaskScheduler instance used by the @ref GameEngine this @ref GameEngineService is attached to.
		/*!	@see @ref Scheduler::TaskScheduler
			*/
		Scheduler::TaskScheduler&						GetEngineTaskScheduler() const;

		//! Retrieves the 'global' engine allocator.
		/*!	@see @ref Allocator
			*/
		::Eldritch2::Allocator&							GetEngineAllocator();

		//! Instructs the hosting engine's scheduler to create an operating system thread and have it assume ownership/begin execution of the given Thread instance.
		/*!	@see @ref Scheduler::Thread
			*/
		::Eldritch2::ErrorCode							LaunchThread( Scheduler::Thread& thread );

	// - LOGGING -----------------------------------------

		//! Emits a status update to all attached logging services.
		void	FormatAndLogString( ETFormatStringHint( const ::Eldritch2::UTF8Char* str ), ... ) const;

		//! Emits a warning message to all attached logging services.
		void	FormatAndLogWarning( ETFormatStringHint( const ::Eldritch2::UTF8Char* str ), ... ) const;

		//! Emits a verbose/minor warning to all attached logging services.
		void	FormatAndLogVerboseWarning( ETFormatStringHint( const ::Eldritch2::UTF8Char* str ), ... ) const;

		//! Emits a fatal error message to all attached logging services.
		void	FormatAndLogError( ETFormatStringHint( const ::Eldritch2::UTF8Char* str ), ... ) const;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		// Constructs this @ref GameEngineService instance.
		GameEngineService( Foundation::GameEngine& owningEngine );

		// Destroys this @ref GameEngineService instance.
		~GameEngineService() = default;

	// - DATA MEMBERS ------------------------------------

	protected:
		Foundation::GameEngine&	_owningEngine;
	};

}	// namespace Foundation
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Foundation/GameEngineService.inl>
//------------------------------------------------------------------//