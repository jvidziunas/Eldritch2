/*==================================================================*\
  NavigationScene.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Navigation/Recast/NavigationScene.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Navigation {
namespace Recast {

	NavigationScene::NavigationScene(
		int maxAgents,
		float32 maxAgentRadius,
		const dtNavMeshParams& meshParameters
	) {
		dtNavMesh::init( &meshParameters );
		dtCrowd::init( maxAgents, maxAgentRadius, this );
	}

// ---------------------------------------------------

	void NavigationScene::Simulate( float32 timeDeltaMicroseconds ) {
		dtCrowd::update( timeDeltaMicroseconds, nullptr );
	}

// ---------------------------------------------------

	ETPureFunctionHint dtNavMeshParams ETSimdCall BuildMeshParameters( Vector origin, float32 tileWidth, float32 tileHeight, uint32 maxTiles, uint32 maxPolygons ) {
		dtNavMeshParams	meshParams;
		float32			coefficients[4];

		origin.ExtractCoefficients( coefficients );

		meshParams.orig[0]		= coefficients[0];
		meshParams.orig[1]		= coefficients[1];
		meshParams.orig[2]		= coefficients[2];
		
		meshParams.tileWidth	= tileWidth;
		meshParams.tileHeight	= tileHeight;
		meshParams.maxTiles		= maxTiles;
		meshParams.maxPolys		= maxPolygons;

		return meshParams;
	}
	
}	// namespace Recast
}	// namespace Navigation
}	// namespace Eldritch2