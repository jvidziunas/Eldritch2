/*==================================================================*\
  ActionSet.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/ActionSet.hpp>
#include <Input/InputBus.hpp>
//------------------------------------------------------------------//

double	wrenGetSlotDouble( WrenVM* vm, int slot );
void	wrenSetSlotString( WrenVM* vm, int slot, const char* text );
void	wrenAbortFiber( WrenVM* vm, int slot );

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	using namespace ::Eldritch2::Input;

	ET_IMPLEMENT_WREN_CLASS( ActionSet ) {
		api.CreateClass<ActionSet>( ET_BUILTIN_WREN_MODULE_NAME( Input ), "ActionSet",
			{/* Constructors */
				DefineConstructor<ActionSet ( WrenHandle )>( "new", [] ( WrenVM* vm ) {
					SetReturn<ActionSet>( vm ).BindResources( vm, 1 );
				} )
			},
			{/*	Properties */},
			{/*	Methods */
				DefineMethod<void ( InputBus, double )>( "acquireDevice", [] ( WrenVM* vm ) {
					InputDevice* const target( GetSlotAs<InputBus>( vm, 1 ).Find( AsIntBits( wrenGetSlotDouble( vm, 2 ) ) ) );
					ET_ABORT_WREN_UNLESS( target != nullptr, "ID does not match any devices" );
					ET_ABORT_WREN_UNLESS( GetSlotAs<ActionSet>( vm, 0 ).TryAcquire( *target ), "Device already acquired by another client" );
				} )
			},
			{/*	Static methods */},
			{/*	Operators */}
		);
	}

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2