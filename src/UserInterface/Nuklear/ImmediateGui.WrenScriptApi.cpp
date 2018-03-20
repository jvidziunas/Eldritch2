/*==================================================================*\
  ImmediateGui.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/ImmediateGui.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/Marshal.hpp>
#include <Common/Mpl/FloatTypes.hpp>
//------------------------------------------------------------------//
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace Wren {
namespace {

	static ETInlineHint nk_flags GetSlotFlags( WrenVM* vm, int slot ) {
		return static_cast<nk_flags>(wrenGetSlotDouble( vm, slot ));
	}

}	// anonymous namespace

	using namespace ::Eldritch2::UserInterface::Nuklear;
	using namespace ::Eldritch2::Scripting;

	ET_IMPLEMENT_WREN_CLASS( ImmediateGui ) {
		api.DefineClass<ImmediateGui>(
			WREN_BUILTIN_MODULE_PREFIX"ImmediateGui",
			"ImmediateGui",
			{/* Constructors */},
			{/*	Properties */},
			{/*	Methods */
				DefineMethod<void ( const Utf8Char*, nk_flags, WrenHandle* )>( "doWindow", [] ( WrenVM* vm ) {
					const auto	flags( GetSlotFlags( vm, 2 ) );
					const auto	body( wrenGetSlotHandle( vm, 3 ) );

					GetReceiverAs<ImmediateGui>( vm ).DoWindow( wrenGetSlotString( vm, 1 ), flags, {}, [vm, body] ( Panel& panel ) {
						wrenSetSlotHandle( vm, 0, body );
						wrenCall( vm, body );
					} );

					wrenReleaseHandle( vm, body );
				} ),
				DefineMethod<void ( const Utf8Char*, nk_flags, WrenHandle* )>( "doStaticPopup", [] ( WrenVM* vm ) {
					const auto	flags( GetSlotFlags( vm, 2 ) );
					const auto	body( wrenGetSlotHandle( vm, 3 ) );

					GetReceiverAs<ImmediateGui>( vm ).DoStaticPopup( wrenGetSlotString( vm, 1 ), flags, {}, [vm, body] ( Panel& panel ) {
						wrenSetSlotHandle( vm, 0, body );
						wrenCall( vm, body );
					} );

					wrenReleaseHandle( vm, body );
				} ),
				DefineMethod<void ( const Utf8Char*, nk_flags, WrenHandle* )>( "doDynamicPopup", [] ( WrenVM* vm ) {
					const auto	flags( GetSlotFlags( vm, 2 ) );
					const auto	body( wrenGetSlotHandle( vm, 3 ) );

					GetReceiverAs<ImmediateGui>( vm ).DoDynamicPopup( wrenGetSlotString( vm, 1 ), flags, {}, [vm, body] ( Panel& panel ) {
						wrenSetSlotHandle( vm, 0, body );
						wrenCall( vm, body );
					} );

					wrenReleaseHandle( vm, body );
				} ),
				DefineMethod<void ( float32, WrenHandle* )>( "doTooltip", [] ( WrenVM* vm ) {
					const auto	body( wrenGetSlotHandle( vm, 2 ) );

					GetReceiverAs<ImmediateGui>( vm ).DoTooltip( wrenGetSlotDouble( vm, 1 ), [vm, body] ( Panel& panel ) {
						wrenSetSlotHandle( vm, 0, body );
						wrenCall( vm, body );
					} );

					wrenReleaseHandle( vm, body );
				} )
			},
			{/*	Operators */}
		);
	}

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2