/*==================================================================*\
  Quaternion.WrenScriptApi.cpp
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
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	ET_IMPLEMENT_WREN_CLASS( Quaternion ) {
		api.CreateClass<Quaternion>( ET_BUILTIN_WREN_MODULE_NAME( Math ), "Quaternion",
			{/* Constructors */
				DefineConstructor<Quaternion ( double, double, double, double )>( "new", [] ( WrenVM* vm ) {
					SetReturn<Quaternion>(
						vm,
						static_cast<float32>( wrenGetSlotDouble( vm, 1 ) ),
						static_cast<float32>( wrenGetSlotDouble( vm, 2 ) ),
						static_cast<float32>( wrenGetSlotDouble( vm, 3 ) ),
						static_cast<float32>( wrenGetSlotDouble( vm, 4 ) )
					);
				} ),
				DefineConstructor<Quaternion ( Vector, Vector )>( "forBasis", [] ( WrenVM* vm ) {
					SetReturn<Quaternion>( vm, AsBasis( GetSlotAs<Vector>( vm, 1 ), GetSlotAs<Vector>( vm, 2 ) ) );
				} ),
				DefineConstructor<Quaternion ()>( "identity", [] ( WrenVM * vm ) {
					SetReturn<Quaternion>( vm, 1.0f, 0.0f, 0.0f, 0.0f );
				} )
			},
			{/*	Properties */
				DefineGetter<Vector>( "forward", [] ( WrenVM* vm ) {
					const Quaternion&	self( GetSlotAs<Quaternion>( vm, 0 ) );

					wrenSetSlotHandle( vm, 0, AsContext( vm ).FindForeignClass<Vector>() );
					SetReturn<Vector>( vm, self.GetForward() );
				} ),
				DefineGetter<Vector>( "right", [] ( WrenVM* vm ) {
					const Quaternion&	self( GetSlotAs<Quaternion>( vm, 0 ) );

					wrenSetSlotHandle( vm, 0, AsContext( vm ).FindForeignClass<Vector>() );
					SetReturn<Vector>( vm, self.GetRight() );
				} ),
				DefineGetter<Vector>( "up", [] ( WrenVM* vm ) {
					const Quaternion&	self( GetSlotAs<Quaternion>( vm, 0 ) );

					wrenSetSlotHandle( vm, 0, AsContext( vm ).FindForeignClass<Vector>() );
					SetReturn<Vector>( vm, self.GetUp() );
				} ),
				DefineGetter<const Utf8Char*>( "toString", [] ( WrenVM* vm ) {
					fmt::memory_buffer	string;
					float32				coefficients[4];

					GetSlotAs<Quaternion>( vm, 0 ).ExtractCoefficients( coefficients );

					fmt::format_to( string, "<i={}, j={}, k={}, w={}>", coefficients[3], coefficients[2], coefficients[1], coefficients[0] );

					wrenSetSlotBytes( vm, 0, string.data(), string.size() );
				} )
			},
			{/*	Methods */},
			{/*	Static methods */
				DefineMethod<double ( Quaternion, Quaternion )>( "dot", [] ( WrenVM* vm ) {
					wrenSetSlotDouble( vm, 0, DotProduct( GetSlotAs<Quaternion>( vm, 1 ), GetSlotAs<Quaternion>( vm, 2 ) ) );
				} )
			},
			{/*	Operators */}
		);
	}

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2