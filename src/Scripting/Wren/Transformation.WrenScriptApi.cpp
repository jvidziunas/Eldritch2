/*==================================================================*\
  Transformation.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/Context.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	ET_IMPLEMENT_WREN_CLASS( Transformation ) {
		api.CreateClass<Transformation>( ET_BUILTIN_WREN_MODULE_NAME( Math ), "Transformation",
			{/* Constructors */
				DefineConstructor<Transformation ( Vector, Quaternion )>( "new", [] ( WrenVM* vm ) {
					SetReturn<Transformation>( vm, GetSlotAs<Vector>( vm, 1 ), GetSlotAs<Quaternion>( vm, 2 ) );
				} ),
				DefineConstructor<Transformation ()>( "identity", [] ( WrenVM* vm ) {
					SetReturn<Transformation>( vm, Vector( 0.0f, 0.0f, 0.0f, 0.0f ), Quaternion( 1.0f, 0.0f, 0.0f, 0.0f ) );
				} )
			},
			{/*	Properties */
				DefineProperty<Vector>( "translation",
				//	Getter
					[] ( WrenVM* vm ) {
						const auto&	self( GetSlotAs<Transformation>( vm, 0 ) );

						wrenSetSlotHandle( vm, 0, AsContext( vm ).GetForeignClass<Vector>() );
						SetReturn<Vector>( vm, self.translation );
					},
				//	Setter
					[] ( WrenVM* vm ) {
						GetSlotAs<Transformation>( vm, 0 ).translation = GetSlotAs<Vector>( vm, 1 );
					}
				),
				DefineProperty<Quaternion>( "rotation",
				//	Getter
					[] ( WrenVM* vm ) {
						const auto&	self( GetSlotAs<Transformation>( vm, 0 ) );

						wrenSetSlotHandle( vm, 0, AsContext( vm ).GetForeignClass<Quaternion>() );
						SetReturn<Quaternion>( vm, self.rotation );
					},
				//	Setter
					[] ( WrenVM* vm ) {
						GetSlotAs<Transformation>( vm, 0 ).rotation = GetSlotAs<Quaternion>( vm, 1 );
					}
				)
			},
			{/*	Methods */},
			{/*	Operators */}
		);
	}

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2