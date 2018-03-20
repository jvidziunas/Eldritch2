/*==================================================================*\
  NavigationScene.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <DetourNavMesh.h>
#include <DetourCrowd.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Navigation {
namespace Recast {

	class NavigationScene : public dtNavMesh, public dtCrowd {
	public:
		enum : int { InvalidAgentId = -1 };

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref NavigationScene instance.
		NavigationScene( int maxAgents, float32 maxAgentRadius, const dtNavMeshParams& meshParameters );
	//!	Disable copy construction.
		NavigationScene( const NavigationScene& ) = delete;

		~NavigationScene() = default;

	// ---------------------------------------------------

	public:
		void	Simulate( float32 timeDeltaMicroseconds );
	};

// ---------------------------------------------------

	ETPureFunctionHint dtNavMeshParams ETSimdCall BuildMeshParameters( Vector origin, float32 tileWidth, float32 tileHeight, uint32 maxTiles, uint32 maxPolygons );

}	// namespace Recast
}	// namespace Navigation
}	// namespace Eldritch2