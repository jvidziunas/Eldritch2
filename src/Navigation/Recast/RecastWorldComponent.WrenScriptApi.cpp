/*==================================================================*\
  RecastWorldComponent.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Navigation/Recast/RecastWorldComponent.hpp>
#include <Navigation/Recast/NavigationScene.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Navigation {
		namespace Recast {
			ET_DECLARE_WREN_CLASS(NavigationScene);
			ET_DECLARE_WREN_CLASS(BipedalAgent);
		}
	}
}

namespace Eldritch2 {
	namespace Navigation {
		namespace Recast {
			namespace {

				static constexpr int		MaxAgentCount = 4096u;
				static constexpr float32	MaxAgentRadiusInMeters = 5.0f;
				static constexpr float32	TileWidthInMeters = 10.0f;
				static constexpr float32	TileHeightInMeters = 12.0f;
				static constexpr uint32		MaxTilesPerMesh = 4096u;
				static constexpr uint32		MaxPolygonsPerMesh = 16384u;

			}	// anonymous namespace

			using namespace ::Eldritch2::Scripting::Wren;

			void RecastWorldComponent::AcceptVisitor(ApiBuilder& api) {
				ET_REGISTER_WREN_CLASS(NavigationScene, api);
				ET_REGISTER_WREN_CLASS(BipedalAgent, api);

				_scene = api.CreateVariable<NavigationScene>(ET_BUILTIN_WREN_MODULE_NAME(Navigation), "NavigationScene",
															 MaxAgentCount,
															 MaxAgentRadiusInMeters,
															 BuildMeshParameters(
															 Vector(0.0f, 0.0f, 0.0f, 1.0f),
															 TileWidthInMeters,
															 TileHeightInMeters,
															 MaxTilesPerMesh,
															 MaxPolygonsPerMesh
															 )
															 );
			}

		}	// namespace Recast
	}	// namespace Navigation
}	// namespace Eldritch2