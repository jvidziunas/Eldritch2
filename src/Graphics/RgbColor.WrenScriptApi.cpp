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
						float16( wrenGetSlotDouble( vm, 1 ) ),
						float16( wrenGetSlotDouble( vm, 2 ) ),
						float16( wrenGetSlotDouble( vm, 3 ) )
					);
				} ),
				DefineConstructor<RgbColor ( double, double, double )>( "fromXyz", [] ( WrenVM* vm ) {
					SetReturn<RgbColor>(
						vm,
						GetRgbFromCieXyz(
							float16( wrenGetSlotDouble( vm, 1 ) ),
							float16( wrenGetSlotDouble( vm, 2 ) ),
							float16( wrenGetSlotDouble( vm, 3 ) )
						)
					);
				} ),
				DefineConstructor<RgbColor ( double )>( "fromDegreesKelvin", [] ( WrenVM* vm ) {
					SetReturn<RgbColor>( vm, GetRgbFromKelvin( wrenGetSlotDouble( vm, 1 ) ) );
				} )
			},
			{/*	Properties */},
			{/*	Methods */},
			{/*	Operators */}
		);
	}

}	// namespace Graphics
}	// namespace Eldritch2