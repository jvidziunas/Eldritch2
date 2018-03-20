/*==================================================================*\
  Interface.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <UserInterface/Nuklear/Interface.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace UserInterface {
namespace Nuklear {
namespace {

	static ETInlineHint nk_flags GetSlotFlags( WrenVM* vm, int slot ) {
		return static_cast<nk_flags>(wrenGetSlotDouble( vm, slot ));
	}

}	// anonymous namespace

	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Scripting;

	ET_IMPLEMENT_WREN_CLASS( Interface ) {
		api.CreateClass<Interface>( ET_BUILTIN_WREN_MODULE_NAME( Interface ), "Interface",
			{/* Constructors */},
			{/*	Properties */},
			{/*	Methods */
				DefineMethod<void ( const Utf8Char*, double, WrenHandle* )>( "doWindow", [] ( WrenVM* vm ) {
					const auto	body( wrenGetSlotHandle( vm, 3 ) );

					GetSlotAs<Interface>( vm, 0 ).DoWindow( wrenGetSlotString( vm, 1 ), GetSlotFlags( vm, 2 ), {}, [vm, body] ( Panel& panel ) {
						wrenSetSlotHandle( vm, 0, body );
						wrenCall( vm, body );
					} );

					wrenReleaseHandle( vm, body );
				} ),
				DefineMethod<void ( const Utf8Char*, double, WrenHandle* )>( "doStaticPopup", [] ( WrenVM* vm ) {
					const auto	body( wrenGetSlotHandle( vm, 3 ) );

					GetSlotAs<Interface>( vm, 0 ).DoStaticPopup( wrenGetSlotString( vm, 1 ), GetSlotFlags( vm, 2 ), {}, [vm, body] ( Panel& panel ) {
						wrenSetSlotHandle( vm, 0, body );
						wrenCall( vm, body );
					} );

					wrenReleaseHandle( vm, body );
				} ),
				DefineMethod<void ( const Utf8Char*, double, WrenHandle* )>( "doDynamicPopup", [] ( WrenVM* vm ) {
					const auto	body( wrenGetSlotHandle( vm, 3 ) );

					GetSlotAs<Interface>( vm, 0 ).DoDynamicPopup( wrenGetSlotString( vm, 1 ), GetSlotFlags( vm, 2 ), {}, [vm, body] ( Panel& panel ) {
						wrenSetSlotHandle( vm, 0, body );
						wrenCall( vm, body );
					} );

					wrenReleaseHandle( vm, body );
				} ),
				DefineMethod<void ( double, WrenHandle* )>( "doTooltip", [] ( WrenVM* vm ) {
					const auto	body( wrenGetSlotHandle( vm, 2 ) );

					GetSlotAs<Interface>( vm, 0 ).DoTooltip( wrenGetSlotDouble( vm, 1 ), [vm, body] ( Panel& panel ) {
						wrenSetSlotHandle( vm, 0, body );
						wrenCall( vm, body );
					} );

					wrenReleaseHandle( vm, body );
				} )
			},
			{/*	Operators */}
		);
	}

}	// namespace Nuklear
}	// namespace UserInterface
}	// namespace Eldritch2