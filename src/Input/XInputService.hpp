/*==================================================================*\
  XInputService.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Foundation/GameEngineService.hpp>
#include <Scripting/ReferenceTypeBase.hpp>
//------------------------------------------------------------------//
#ifdef ET_PLATFORM_WINDOWS
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>
#endif
#include <XInput.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Input {

	namespace Foundation	= ::Eldritch2::Foundation;
	namespace Input			= ::Eldritch2::Input;

// ---------------------------------------------------

	class XInputService : public Foundation::GameEngineService {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class Controller : public Scripting::ReferenceTypeBase {
		// - TYPE PUBLISHING ---------------------------------

		public:
			enum : ::DWORD {
				INVALID_CONTROLLER_INDEX = static_cast<::DWORD>(-1)
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			// Constructs this Controller instance.
			Controller( const ::DWORD controllerIndex = Controller::INVALID_CONTROLLER_INDEX );

			// Destroys this Controller instance.
			~Controller();

		// ---------------------------------------------------

			void	ReadInput();

		// ---------------------------------------------------

			static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& typeRegistrar );

			void						Dispose() override sealed;

		// - DATA MEMBERS ------------------------------------

			static const char* const	scriptTypeName;

		private:
			::DWORD			_controllerIndex;
			::XINPUT_STATE	_state;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		// Constructs this XInputService instance.
		XInputService( Foundation::GameEngine& owningEngine );

		// Destroys this XInputService instance.
		~XInputService();

	// ---------------------------------------------------

		const ::Eldritch2::UTF8Char* const	GetName() const override sealed;

	// ---------------------------------------------------

	protected:
		void	AcceptInitializationVisitor( Scripting::ScriptAPIRegistrationInitializationVisitor& typeRegistrar ) override sealed;

		void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::Task& visitingTask, Scheduler::WorkerContext& executingContext, const ServiceTickTaskVisitor ) override sealed;

	// - DATA MEMBERS ------------------------------------
	
	private:
		Controller	_controllers[XUSER_MAX_COUNT];
	};

}	// namespace Input
}	// namespace Eldritch2