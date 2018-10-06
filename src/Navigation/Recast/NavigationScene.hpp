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

namespace Eldritch2 { namespace Navigation { namespace Recast {

	class NavigationScene : public dtNavMesh, public dtCrowd {
		// - TYPE PUBLISHING ---------------------------------

	public:
		enum : int { InvalidAgentId = -1 };

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		NavigationScene(const NavigationScene&) = delete;
		//!	Constructs this @ref NavigationScene instance.
		NavigationScene() ETNoexceptHint;

		~NavigationScene() = default;

		// ---------------------------------------------------

	public:
		void Simulate(MicrosecondTime duration);

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(const dtNavMeshParams& meshParameters, int maxAgents, float32 agentRadius);

		void FreeResources();

		// ---------------------------------------------------

		//!	Disable copy assignment.
		NavigationScene& operator=(const NavigationScene&) = delete;
	};

}}} // namespace Eldritch2::Navigation::Recast
