/*==================================================================*\
  XInputService.Controller.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptAPIRegistrationInitializationVisitor.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
#include <Input/XInputService.hpp>
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

	const char* const XInputService::Controller::scriptTypeName = "XInputController";

// ---------------------------------------------------

	XInputService::Controller::Controller( const ::DWORD controllerIndex ) : _controllerIndex( controllerIndex ) {
		if( Controller::INVALID_CONTROLLER_INDEX != controllerIndex ) {
			::XInputGetState( controllerIndex, &_state );
		}
	}

// ---------------------------------------------------

	XInputService::Controller::~Controller() {}

// ---------------------------------------------------

	void XInputService::Controller::ReadInput() {
		if( Controller::INVALID_CONTROLLER_INDEX != _controllerIndex && ERROR_DEVICE_NOT_CONNECTED != ::XInputGetState( _controllerIndex, &_state ) ) {
		}
	}

// ---------------------------------------------------

	ETNoAliasHint void XInputService::Controller::ExposeScriptAPI( ScriptAPIRegistrationInitializationVisitor& typeRegistrar ) {
		FixedStackAllocator<16u>	temporaryAllocator( UTF8L( "XInputService::Controller::ExposeScriptAPI() Temporary Allocator" ) );

		if( const auto registerResult = typeRegistrar.RegisterUserDefinedReferenceType<Controller>( temporaryAllocator ) ) {
			auto&	typeBuilder( *registerResult.object );

			temporaryAllocator.Delete( typeBuilder );
		}
	}

// ---------------------------------------------------

	void XInputService::Controller::Dispose() {}

}	// namespace Input
}	// namespace Eldritch2