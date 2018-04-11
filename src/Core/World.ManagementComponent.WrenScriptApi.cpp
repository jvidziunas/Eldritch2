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

		api.CreateClass<WorldBridge>( ET_BUILTIN_WREN_MODULE_NAME( Core ), "World_",
			{/* Constructors */},
			{/*	Properties */},
			{/*	Methods */
				DefineMethod<void ( const Utf8Char* )>( "requirePackage", [] ( WrenVM* vm ) {
					const ErrorCode	importResult( GetSlotAs<WorldBridge>( vm, 0 ).world->Import( wrenGetSlotString( vm, 1 ) ) );
					ET_ABORT_WREN_IF( Failed( importResult ), AsCString( importResult ) );
				} )
			},
			{/* Static methods */},
			{/*	Operators */}
		);

		api.CreateVariable<WorldBridge>( ET_BUILTIN_WREN_MODULE_NAME( Core ), "World", FindService<World>() );
	}

}	// namespace Core
}	// namespace Eldritch2