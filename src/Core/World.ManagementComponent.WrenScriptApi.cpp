/*==================================================================*\
  World.ManagementComponent.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Core {

	using namespace ::Eldritch2::Scripting::Wren;

	void World::ManagementComponent::AcceptVisitor( ApiBuilder& api ) {
		struct WorldBridge {
			WorldBridge( World& world ) : world( &world ) {}
			~WorldBridge() = default;

			World* const	world;
		};

		api.CreateClass<WorldBridge>( ET_BUILTIN_WREN_MODULE_NAME( Core ), "_World",
			{/* Constructors */},
			{/*	Properties */
				DefineProperty<double>( "timeScalar",
				//	Getter
					[] ( WrenVM* vm ) {
						wrenSetSlotDouble( vm, 0, GetSlotAs<WorldBridge>( vm, 0 ).world->GetTimeScalar() );
					},
				//	Setter
					[] ( WrenVM* vm ) {
						GetSlotAs<WorldBridge>( vm, 0 ).world->SetTimeScalar( static_cast<float32>( wrenGetSlotDouble( vm, 1 ) ) );
					}
				)
			},
			{/*	Methods */
				DefineMethod<void ( const Utf8Char* )>( "requirePackage", [] ( WrenVM* vm ) {
					const ErrorCode	importResult( GetSlotAs<WorldBridge>( vm, 0 ).world->Import( wrenGetSlotString( vm, 1 ) ) );
					ET_ABORT_WREN_IF( Failed( importResult ), AsCString( importResult ) );
				} )
			},
			{/*	Operators */}
		);

		api.CreateVariable<WorldBridge>( ET_BUILTIN_WREN_MODULE_NAME( Core ), "World", FindService<World>() );
	}

}	// namespace Core
}	// namespace Eldritch2