/*==================================================================*\
  EngineService.Controller.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptApiRegistrationInitializationVisitor.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
#include <Input/XInput/EngineService.hpp>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
#if defined(_WIN32_WINNT_WIN8) && (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
	ET_LINK_LIBRARY( "XInput.lib" )
#else 
	ET_LINK_LIBRARY( "XInput9_1_0.lib" )
#endif
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Input;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Input {
namespace XInput {

	const char* const EngineService::Controller::scriptTypeName = "XInputController";

// ---------------------------------------------------

	EngineService::Controller::Controller( const ::DWORD controllerIndex ) : _controllerIndex( controllerIndex ) {
		if( Controller::InvalidControllerIndex != controllerIndex ) {
			::XInputGetState( controllerIndex, &_state );
		}
	}

// ---------------------------------------------------

	void EngineService::Controller::ReadInput() {
		if( (Controller::InvalidControllerIndex != _controllerIndex) && (ERROR_DEVICE_NOT_CONNECTED != ::XInputGetState( _controllerIndex, &_state )) ) {
		}
	}

// ---------------------------------------------------

	ETNoAliasHint void EngineService::Controller::ExposeScriptAPI( ScriptApiRegistrationInitializationVisitor& typeRegistrar ) {
		FixedStackAllocator<16u>	temporaryAllocator( UTF8L("EngineService::Controller::ExposeScriptAPI() Temporary Allocator") );

		if( const auto registerResult = typeRegistrar.RegisterUserDefinedReferenceType<Controller>( temporaryAllocator ) ) {
			auto&	typeBuilder( *registerResult.object );

			temporaryAllocator.Delete( typeBuilder );
		}
	}

// ---------------------------------------------------

	void EngineService::Controller::Dispose() {}

}	// namespace XInput
}	// namespace Input
}	// namespace Eldritch2