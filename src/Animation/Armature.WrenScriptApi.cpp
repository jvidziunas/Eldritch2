/*==================================================================*\
  Armature.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/Context.hpp>
#include <Animation/Armature.hpp>
//------------------------------------------------------------------//
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Animation {

	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Scripting;

	ET_IMPLEMENT_WREN_CLASS( Armature ) {
		api.CreateClass<Armature>( ET_BUILTIN_WREN_MODULE_NAME( Animation ), "Armature",
			{/* Constructors */
				DefineConstructor<void ( Transformation )>( "new", [] ( WrenVM* vm ) {
					SetReturn<Armature>( vm, GetSlotAs<Transformation>( vm, 1 ) );
				} )
			},
			{/*	Properties */
				DefineGetter<Transformation>( "localToWorld", [] ( WrenVM* vm ) {
					Armature&	self( GetSlotAs<Armature>( vm, 0 ) );

					wrenSetSlotHandle( vm, 0, AsContext( vm ).GetForeignClass<Transformation>() );
					SetReturn<Transformation>( vm, self.GetLocalToWorld() );
				} ),
				DefineGetter<Transformation>( "worldToLocal", [] ( WrenVM* vm ) {
					Armature&	self( GetSlotAs<Armature>( vm, 0 ) );

					wrenSetSlotHandle( vm, 0, AsContext( vm ).GetForeignClass<Transformation>() );
					SetReturn<Transformation>( vm, self.GetWorldToLocal() );
				} )
			},
			{/*	Methods */},
			{/*	Operators */}
		);
	}

}	// namespace Animation
}	// namespace Eldritch2