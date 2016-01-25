/*==================================================================*\
  EngineService.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Configuration/ConfigurationDatabase.hpp>
#include <Foundation/GameEngineService.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace FileSystem {
		class	ContentProvider;
	}
}

namespace Eldritch2 {
namespace Configuration {

	class EngineService : public Foundation::GameEngineService, private Configuration::ConfigurationDatabase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref EngineService instance.
		EngineService( Foundation::GameEngine& owningEngine, FileSystem::ContentProvider& contentProvider );

		//! Destroys this @ref EngineService instance.
		~EngineService() = default;

	// - DEBUG/LOGGING INFORMATION -----------------------

		const ::Eldritch2::UTF8Char* const	GetName() const override sealed;

	// ---------------------------------------------------

	protected:
		void	AcceptInitializationVisitor( Scripting::ScriptAPIRegistrationInitializationVisitor& typeRegistrar ) override sealed;

	// ---------------------------------------------------

		void	AcceptTaskVisitor( Scheduler::WorkerContext& executingContext, Scheduler::WorkerContext::FinishCounter& finishCounter, const InitializeEngineTaskVisitor ) override sealed;
		void	AcceptTaskVisitor( Scheduler::WorkerContext& executingContext, Scheduler::WorkerContext::FinishCounter& finishCounter, const PreConfigurationLoadedTaskVisitor ) override sealed;

	// ---------------------------------------------------

		void	DumpConfigurationToFile() const;

	// ---------------------------------------------------

	private:
		void	BroadcastConfigurationToEngine();

	// - DATA MEMBERS ------------------------------------

		FileSystem::ContentProvider&	_contentProvider;
	};

}	// namespace Configuration
}	// namespace Eldritch2