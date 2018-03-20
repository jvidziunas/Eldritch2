/*==================================================================*\
  DialogueSet.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/AssetViews/DialogueSetAsset.hpp>
#include <Scripting/Wren/AssetReference.hpp>
#include <Scripting/Wren/DialogueSet.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	using namespace ::Eldritch2::Scripting::Wren::AssetViews;

	ET_IMPLEMENT_WREN_CLASS( DialogueSet ) {
		api.CreateClass<DialogueSet>( ET_BUILTIN_WREN_MODULE_NAME( Core ), "DialogueSet",
			{/* Constructors */
				DefineConstructor<DialogueSet ( AssetReference<DialogueSetAsset> )>( "new", [] ( WrenVM* vm ) {
					SetReturn<DialogueSet>( vm, GetSlotAs<AssetReference<DialogueSetAsset>>( vm, 1 ) );
				} )
			},
			{/*	Properties */},
			{/*	Methods */
				DefineMethod<void ( WrenHandle*, WrenHandle* )>( "match", [] ( WrenVM* vm ) {
					WrenHandle* const	target( wrenGetSlotHandle( vm, 1 ) );
					WrenHandle* const	facts( wrenGetSlotHandle( vm, 2 ) );

					GetSlotAs<DialogueSet>( vm, 0 ).Match( facts );

					wrenReleaseHandle( vm, facts );
					wrenReleaseHandle( vm, target );
				} )
			},
			{/*	Operators */}
		);
	}

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2