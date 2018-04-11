/*==================================================================*\
  Vector.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/Marshal.hpp>
//------------------------------------------------------------------//
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	ET_IMPLEMENT_WREN_CLASS( Vector ) {
		api.CreateClass<Vector>( ET_BUILTIN_WREN_MODULE_NAME( Math ), "Vector",
			{/* Constructors */
				DefineConstructor<Vector ( double, double, double )>( "new", [] ( WrenVM* vm ) {
					SetReturn<Vector>(
						vm,
						static_cast<float32>(wrenGetSlotDouble( vm, 1 )),
						static_cast<float32>(wrenGetSlotDouble( vm, 2 )),
						static_cast<float32>(wrenGetSlotDouble( vm, 3 )),
						0.0f
					);
				} ),
				DefineConstructor<Vector ( double )>( "splat", [] ( WrenVM* vm ) {
					SetReturn<Vector>(
						vm,
						static_cast<float32>(wrenGetSlotDouble( vm, 1 )),
						static_cast<float32>(wrenGetSlotDouble( vm, 1 )),
						static_cast<float32>(wrenGetSlotDouble( vm, 1 )),
						0.0f
					);
				} ),
				DefineConstructor<Vector ()>( "forward", [] ( WrenVM* vm ) {
					SetReturn<Vector>( vm, 0.0f, 0.0f, 1.0f, 0.0f );
				} ),
				DefineConstructor<Vector ()>( "up", [] ( WrenVM* vm ) {
					SetReturn<Vector>( vm, 0.0f, 1.0f, 0.0f, 0.0f );
				} ),
				DefineConstructor<Vector ()>( "right", [] ( WrenVM* vm ) {
					SetReturn<Vector>( vm, 1.0f, 0.0f, 0.0f, 0.0f );
				} )
			},
			{/*	Properties */
				DefineGetter<double>( "lengthSquared", [] ( WrenVM* vm ) {
					wrenSetSlotDouble( vm, 0, SquaredLength( GetSlotAs<Vector>( vm, 0 ) ) );
				} ),
				DefineGetter<double>( "length", [] ( WrenVM* vm ) {
					wrenSetSlotDouble( vm, 0, Length( GetSlotAs<Vector>( vm, 0 ) ) );
				} ),
				DefineGetter<Vector>( "abs", [] ( WrenVM* vm ) {
					const Vector&	self( GetSlotAs<Vector>( vm, 0 ) );

					wrenSetSlotHandle( vm, 0, AsContext( vm ).FindForeignClass<Vector>() );
					SetReturn<Vector>( vm, AbsoluteValue( self ) );
				} ),
				DefineGetter<const Utf8Char*>( "toString", [] ( WrenVM* vm ) {
					fmt::memory_buffer	string;
					float32				coefficients[4];

					GetSlotAs<Vector>( vm, 0 ).ExtractCoefficients( coefficients );

					fmt::format_to( string, "<x={}, y={}, z={}>", coefficients[3], coefficients[2], coefficients[1] );
					
					wrenSetSlotBytes( vm, 0, string.data(), string.size() );
				} )
			},
			{/*	Methods */},
			{/*	Static methods */
				DefineMethod<double ( Vector, Vector )>( "dot", [] ( WrenVM* vm ) {
					wrenSetSlotDouble( vm, 0, DotProduct( GetSlotAs<Vector>( vm, 1 ), GetSlotAs<Vector>( vm, 2 ) ) );
				} ),
				DefineMethod<Vector ( Vector, Vector )>( "cross", [] ( WrenVM* vm ) {
					SetReturn<Vector>( vm, CrossProduct( GetSlotAs<Vector>( vm, 1 ), GetSlotAs<Vector>( vm, 2 ) ) );
				} ),
			},
			{/*	Operators */
				DefineMethod<Vector ( Vector )>( "+", [] ( WrenVM* vm ) {
					const Vector&	self( GetSlotAs<Vector>( vm, 0 ) );

					wrenSetSlotHandle( vm, 0, AsContext( vm ).FindForeignClass<Vector>() );
					SetReturn<Vector>( vm, self + GetSlotAs<Vector>( vm, 1 ) );
				} ),
				DefineMethod<Vector ( Vector )>( "-", [] ( WrenVM* vm ) {
					const Vector&	self( GetSlotAs<Vector>( vm, 0 ) );

					wrenSetSlotHandle( vm, 0, AsContext( vm ).FindForeignClass<Vector>() );
					SetReturn<Vector>( vm, self - GetSlotAs<Vector>( vm, 1 ) );
				} ),
				DefineMethod<Vector ( double )>( "*", [] ( WrenVM* vm ) {
					const Vector&	self( GetSlotAs<Vector>( vm, 0 ) );

					wrenSetSlotHandle( vm, 0, AsContext( vm ).FindForeignClass<Vector>() );
					SetReturn<Vector>( vm, self * static_cast<float32>(wrenGetSlotDouble( vm, 1 )) );
				} ),
				DefineMethod<Vector ( double )>( "/", [] ( WrenVM* vm ) {
					const Vector&	self( GetSlotAs<Vector>( vm, 0 ) );

					wrenSetSlotHandle( vm, 0, AsContext( vm ).FindForeignClass<Vector>() );
					SetReturn<Vector>( vm, self / static_cast<float32>(wrenGetSlotDouble( vm, 1 )) );
				} )
			}
		);
	}

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2