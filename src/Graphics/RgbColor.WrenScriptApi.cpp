/*==================================================================*\
  RgbColor.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Graphics/RgbColor.hpp>
//------------------------------------------------------------------//
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {

	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Scripting;

	ET_IMPLEMENT_WREN_CLASS( RgbColor ) {
		api.CreateClass<RgbColor>( ET_BUILTIN_WREN_MODULE_NAME( Graphics ), "RgbColor",
			{/* Constructors */
				DefineConstructor<RgbColor ( double, double, double )>( "fromRgb", [] ( WrenVM* vm ) {
					SetReturn<RgbColor>(
						vm,
						float32( wrenGetSlotDouble( vm, 1 ) ),
						float32( wrenGetSlotDouble( vm, 2 ) ),
						float32( wrenGetSlotDouble( vm, 3 ) )
					);
				} ),
				DefineConstructor<RgbColor ( double, double, double )>( "fromXyz", [] ( WrenVM* vm ) {
					SetReturn<RgbColor>(
						vm,
						GetRgbFromCieXyz(
							float32( wrenGetSlotDouble( vm, 1 ) ),
							float32( wrenGetSlotDouble( vm, 2 ) ),
							float32( wrenGetSlotDouble( vm, 3 ) )
						)
					);
				} ),
				DefineConstructor<RgbColor ( double )>( "fromDegreesKelvin", [] ( WrenVM* vm ) {
					SetReturn<RgbColor>( vm, GetRgbFromKelvin( wrenGetSlotDouble( vm, 1 ) ) );
				} )
			},
			{/*	Properties */
				DefineGetter<const Utf8Char*>( "toString", [] ( WrenVM* vm ) {
					const RgbColor& self( GetSlotAs<RgbColor>( vm, 0 ) );

					fmt::memory_buffer	string;
					fmt::format_to( string, "<r={}, g={}, b={}>", self.GetRed(), self.GetGreen(), self.GetBlue() );

					wrenSetSlotString( vm, 0, string.data() );
				} ) 
			},
			{/*	Methods */},
			{/* Static methods */},
			{/*	Operators */}
		);
	}

}	// namespace Graphics
}	// namespace Eldritch2