/*==================================================================*\
  VulkanGraphicsScene.PlayerView.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/VulkanGraphicsScene.hpp>
#include <Graphics/Vulkan/OutputCoordinator.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/Context.hpp>
//------------------------------------------------------------------//
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Scripting;

	ET_IMPLEMENT_WREN_CLASS( PlayerView ) {
		api.CreateClass<PlayerView>( ET_BUILTIN_WREN_MODULE_NAME( Graphics ), "PlayerView",
			{/* Constructors */
				DefineConstructor<PlayerView ( OutputCoordinator, Transformation, double )>( "new", [] ( WrenVM* vm ) {
					SetReturn<PlayerView>( vm,
						GetSlotAs<OutputCoordinator>( vm, 1 ).GetWindowByName( "" ),
						GetSlotAs<Transformation>( vm, 2 ),
						Angle( wrenGetSlotDouble( vm, 3 ) )
					);
				} )
			},
			{/*	Properties */
				DefineProperty<Transformation>(
					"localToWorld",
				//	Getter
					[] ( WrenVM* vm ) {
						const PlayerView&	self( GetSlotAs<PlayerView>( vm, 0 ) );

						wrenSetSlotHandle( vm, 0, AsContext( vm ).GetForeignClass<Transformation>() );
						SetReturn<Transformation>( vm, self.GetLocalToWorld() );
					},
				//	Setter
					[] ( WrenVM* vm ) {
						GetSlotAs<PlayerView>( vm, 0 ).SetLocalToWorld( GetSlotAs<Transformation>( vm, 1 ) );
					}
				),
				DefineGetter<Transformation>( "worldToLocal", [] ( WrenVM* vm ) {
					const PlayerView&	self( GetSlotAs<PlayerView>( vm, 0 ) );

					wrenSetSlotHandle( vm, 0, AsContext( vm ).GetForeignClass<Transformation>() );
					SetReturn<Transformation>( vm, self.GetLocalToWorld().GetInverse() );
				} ),
				DefineProperty<float32>(
					"verticalFovDegrees",
				//	Getter
					[] ( WrenVM* vm ) {
						wrenSetSlotDouble( vm, 0, DegreesFromAngle( GetSlotAs<PlayerView>( vm, 0 ).GetVerticalFov() ) );
					},
				//	Setter
					[] ( WrenVM* vm ) {
						GetSlotAs<PlayerView>( vm, 0 ).SetVerticalFov( AngleFromDegrees( wrenGetSlotDouble( vm, 1 ) ) );
					}
				),
			},
			{/*	Methods */
				DefineStaticMethod<double ( double )>( "getVerticalFov", [] ( WrenVM* vm ) {
					wrenSetSlotDouble( vm, 0, (16.0 / 9.0) * wrenGetSlotDouble( vm, 1 ) );
				} )
			},
			{/*	Operators */}
		);
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2