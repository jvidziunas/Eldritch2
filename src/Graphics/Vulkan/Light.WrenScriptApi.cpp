/*==================================================================*\
  Light.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/VulkanGraphicsScene.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//

double	wrenGetSlotDouble( WrenVM* vm, int slot );
void	wrenSetSlotHandle( WrenVM* vm, int slot, WrenHandle* handle );

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {
namespace {

	struct Light : public Graphics::Light {
		Light( Transformation localToWorld, RgbColor color, float16 radius ) : Graphics::Light( localToWorld, color, radius ) {}
	};

}	// anonymous namespace

	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Scripting;

	ET_IMPLEMENT_WREN_CLASS( Light ) {
		api.CreateClass<Light>( ET_BUILTIN_WREN_MODULE_NAME( Graphics ), "Light",
			{/* Constructors */
				DefineConstructor<Light ( VulkanGraphicsScene, Transformation, RgbColor, double )>( "new", [] ( WrenVM* vm ) {
					SetReturn<Light>( vm, GetSlotAs<Transformation>( vm, 2 ), GetSlotAs<RgbColor>( vm, 3 ), float16( wrenGetSlotDouble( vm, 4 ) ) );
				} )
			},
			{/*	Properties */
				DefineProperty<RgbColor>(
					"color", 
				//	Getter
					[] ( WrenVM* vm ) {
						Light&	self( GetSlotAs<Light>( vm, 0 ) );

						wrenSetSlotHandle( vm, 0, AsContext( vm ).FindForeignClass<RgbColor>() );
						SetReturn<RgbColor>( vm, self.color );
					},
				//	Setter
					[] ( WrenVM* vm ) {
						GetSlotAs<Light>( vm, 0 ).color = GetSlotAs<RgbColor>( vm, 1 );
					}
				),
				DefineProperty<Transformation>(
					"localToWorld",
				//	Getter
					[] ( WrenVM* vm ) {
						Light&	self( GetSlotAs<Light>( vm, 0 ) );

						wrenSetSlotHandle( vm, 0, AsContext( vm ).FindForeignClass<Transformation>() );
						SetReturn<Transformation>( vm, self.localToWorld );
					},
				//	Setter
					[] ( WrenVM* vm ) {
						GetSlotAs<Light>( vm, 0 ).localToWorld = GetSlotAs<Transformation>( vm, 1 );
					}
				)
			},
			{/*	Methods */},
			{/*	Static methods */},
			{/*	Operators */}
		);
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2