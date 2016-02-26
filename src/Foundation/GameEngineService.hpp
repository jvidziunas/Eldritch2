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
#include <Scheduler/WorkerContext.hpp>
#include <Utility/MPL/Noncopyable.hpp>
#include <Utility/MPL/CharTypes.hpp>
#include <Utility/CountedResult.hpp>
#include <Utility/Result.hpp>
#include <Logging/Logger.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Configuration {
		class	ConfigurationPublishingInitializationVisitor;
	}

	namespace Foundation {
		class	GameEngine;
		class	WorldView;
		class	World;
	}

	namespace FileSystem {
		class	ResourceViewFactoryPublishingInitializationVisitor;
		class	ContentLibrary;
	}

	namespace Scripting {
		class	ScriptApiRegistrationInitializationVisitor;
	}

	namespace Scheduler {
		class	ThreadScheduler;
		class	Thread;
	}

	class	ErrorCode;
	class	Allocator;
}

namespace Eldritch2 {
namespace Foundation {

	class ETPureAbstractHint GameEngineService : public ::Eldritch2::IntrusiveForwardListBaseHook {
	// - DEBUG/LOGGING INFORMATION -----------------------

	public:
		virtual const ::Eldritch2::UTF8Char* const	GetName() const abstract;

	// ---------------------------------------------------

		virtual ::Eldritch2::ErrorCode	AllocateWorldView( ::Eldritch2::Allocator& allocator, Foundation::World& world );

	// ---------------------------------------------------

		//! Interested service classes should override this method to schedule simple bootstrap tasks with no additional data dependencies.
		/*!	@param[in] executingContext The Scheduler::WorkerContext instance that should execute any internal work.
			@remark The default implementation does nothing.
			@see @ref Scheduler::WorkerContext
			*/
		virtual void	OnEngineInitializationStarted( Scheduler::WorkerContext& executingContext );

		//! Interested service classes should override this method to schedule initialization tasks that require the use of user-configurable variables.
		/*!	@param[in] executingContext The Scheduler::WorkerContext instance that should execute any internal work.
			@remark The default implementation does nothing.
			@see @ref Scheduler::WorkerContext
			*/
		virtual void	OnEngineConfigurationBroadcast( Scheduler::WorkerContext& executingContext );

		//! Interested service classes should override this method in order to participate in script API setup.
		/*!	@param[in] visitor The type registrar that will handle publishing the service's types, methods and variables to script objects.
			@remark The default implementation does nothing.
			@see @ref Scripting::ScriptApiRegistrationInitializationVisitor
			*/
		virtual void	AcceptInitializationVisitor( Scripting::ScriptApiRegistrationInitializationVisitor& visitor );
		//! Interested service classes should override this method in order to participate in configurable variable setup.
		/*!	@remark The default implementation does nothing.
			@see @ref Configuration::ConfigurationPublishingInitializationVisitor
			*/
		virtual void	AcceptInitializationVisitor( Configuration::ConfigurationPublishingInitializationVisitor& visitor );
		//! Interested service classes should override this method in order to participate in resource/world view creation.
		/*!	@remark The default implementation does nothing.
			@see @ref FileSystem::ResourceViewFactoryPublishingInitializationVisitor
			*/
		virtual void	AcceptInitializationVisitor( FileSystem::ResourceViewFactoryPublishingInitializationVisitor& visitor );

		//! Interested service classes should override this method in order to perform any final initialization that interacts with other attached services.
		/*!	@remark The default implementation does nothing.
			*/
		virtual void	OnEngineInitializationCompleted( Scheduler::WorkerContext& executingContext );

	// ---------------------------------------------------

		/*!	@param[in] executingContext The Scheduler::WorkerContext instance that should execute any internal work.
			@remark The default implementation does nothing.
			@see @ref Scheduler::WorkerContext
			*/
		virtual void	OnServiceTickStarted( Scheduler::WorkerContext& executingContext );

		/*!	@param[in] executingContext The Scheduler::WorkerContext instance that should execute any internal work.
			@remark The default implementation does nothing.
			@see @ref Scheduler::WorkerContext
			*/
		virtual void	OnWorldTickStarted( Scheduler::WorkerContext& executingContext );

	// ---------------------------------------------------

		template <typename InitializationVisitor>
		ETInlineHint void	BroadcastInitializationVisitor( InitializationVisitor&& visitor );

		template <void (GameEngineService::*Initialization)( Scheduler::WorkerContext& )>
		ETInlineHint void	InvokeInitializationFunction( Scheduler::WorkerContext& executingContext );
		template <void (GameEngineService::*Initialization)( Scheduler::WorkerContext& )>
		ETInlineHint void	InvokeInitializationFunction( Scheduler::WorkerContext::FinishCounter& finishCounter, Scheduler::WorkerContext& executingContext );

	// ---------------------------------------------------

		template <void (GameEngineService::*TickFunction)( Scheduler::WorkerContext& )>
		ETInlineHint void	InvokeTickFunction( Scheduler::WorkerContext& executingContext );
		template <void (GameEngineService::*TickFunction)(Scheduler::WorkerContext&)>
		ETInlineHint void	InvokeTickFunction( Scheduler::WorkerContext::FinishCounter& finishCounter, Scheduler::WorkerContext& executingContext );

	// - GAME ENGINE SERVICE SANDBOX METHODS -------------

	protected:
		//! Creates a new game world using an anonymous editor package.
		/*!	@param[in] worldResourceName Null-terminated C string containing the name of the root content package for the world. This will be passed to @ref FileSystem::ContentLibrary::ResolvePackageByName()
			@see @ref FileSystem::ContentPackage, @ref FileSystem::ContentLibrary::ResolvePackageByName()
			*/
		::Eldritch2::CountedResult<Foundation::World>		CreateWorld( const ::Eldritch2::UTF8Char* const worldResourceName );

		//! Retrieves a read-only view of the @ref GameEngine instance that owns this @ref GameEngineService.
		ETInlineHint const Foundation::GameEngine&			GetGameEngine() const;

		//! Retrieves a read-only view of the @ref FileSystem::ContentLibrary instance owned by the @ref GameEngine this @ref GameEngineService is attached to.
		/*!	@see @ref FileSystem::ContentLibrary
			*/
		const FileSystem::ContentLibrary&					GetEngineContentLibrary() const;

		//! Retrieves the Scheduler::ThreadScheduler instance used by the @ref GameEngine this @ref GameEngineService is attached to.
		/*!	@see @ref Scheduler::ThreadScheduler
			*/
		Scheduler::ThreadScheduler&							GetEngineThreadScheduler() const;

		//! Retrieves the 'global' engine allocator.
		/*!	@see @ref Allocator
			*/
		::Eldritch2::Allocator&								GetEngineAllocator();

		//! Instructs the hosting engine's scheduler to create an operating system thread and have it assume ownership/begin execution of the given Thread instance.
		/*!	@see @ref Scheduler::Thread
			*/
		::Eldritch2::ErrorCode								LaunchThread( Scheduler::Thread& thread );

	// - LOGGING -----------------------------------------

		Foundation::Logger&	GetLogger( const Foundation::LogMessageType type = Foundation::LogMessageType::Message ) const;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		// Constructs this @ref GameEngineService instance.
		/*!	Adds the service to the indicated @ref GameEngine's list of attached services.
			@param[in] owningEngine @ref GameEngine responsible for updating the new @ref GameEngineService.
			*/
		GameEngineService( Foundation::GameEngine& owningEngine );

		// Destroys this @ref GameEngineService instance.
		~GameEngineService();

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