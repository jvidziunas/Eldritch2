/*==================================================================*\
  EngineService.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Foundation/GameEngineService.hpp>
#include <Scripting/ReferenceCountable.hpp>
//------------------------------------------------------------------//
#ifdef ET_PLATFORM_WINDOWS
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>
#endif
#include <XInput.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Input {
namespace XInput {

	class EngineService : public Foundation::GameEngineService {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class Controller : public Scripting::ReferenceCountable {
		// - TYPE PUBLISHING ---------------------------------

		public:
			enum : ::DWORD {
				InvalidControllerIndex = static_cast<::DWORD>(-1)
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			//!	Constructs this @ref Controller instance.
			Controller( const ::DWORD controllerIndex = Controller::InvalidControllerIndex );

			~Controller() = default;

		// ---------------------------------------------------

			void	ReadInput();

		// ---------------------------------------------------

			static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptApiRegistrationInitializationVisitor& typeRegistrar );

			void						Dispose() override sealed;

		// - DATA MEMBERS ------------------------------------

			static const char* const	scriptTypeName;

		private:
			::DWORD			_controllerIndex;
			::XINPUT_STATE	_state;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref EngineService instance.
		EngineService( Foundation::GameEngine& owningEngine );

		//!	Destroys this @ref EngineService instance.
		~EngineService() = default;

	// ---------------------------------------------------

		const ::Eldritch2::UTF8Char* const	GetName() const override sealed;

	// ---------------------------------------------------

	protected:
		void	AcceptInitializationVisitor( Scripting::ScriptApiRegistrationInitializationVisitor& typeRegistrar ) override sealed;

	// ---------------------------------------------------

		void	OnServiceTickStarted( Scheduler::WorkerContext& executingContext ) override sealed;

	// - DATA MEMBERS ------------------------------------
	
	private:
		Controller	_controllers[XUSER_MAX_COUNT];
	};

}	// namespace XInput
}	// namespace Input
}	// namespace Eldritch2