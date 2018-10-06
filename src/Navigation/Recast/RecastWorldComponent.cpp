/*==================================================================*\
  RecastWorldComponent.cpp
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
#include <Core/World.hpp>
//------------------------------------------------------------------//

#define ET_TERMINATE_WORLD_UNLESS(cond)                  \
	if (!(cond)) {                                       \
		FindService<Core::World>()->SetShouldShutDown(); \
	}

namespace Eldritch2 { namespace Navigation { namespace Recast {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Core;

	namespace {

		ETInlineHint ETForceInlineHint ETPureFunctionHint dtNavMeshParams ETSimdCall AsNavMeshParams(Vector origin, Vector tileDimensions, uint32 maxTiles, uint32 maxPolygons) ETNoexceptHint {
			ET16ByteAligned float32 coefficients[4];
			dtNavMeshParams         meshParams;

			StreamCoefficients(coefficients, origin);
			meshParams.orig[0] = coefficients[Vector::X];
			meshParams.orig[1] = coefficients[Vector::Y];
			meshParams.orig[2] = coefficients[Vector::Z];

			StreamCoefficients(coefficients, tileDimensions);
			meshParams.tileWidth  = coefficients[Vector::X];
			meshParams.tileHeight = coefficients[Vector::Y];
			meshParams.maxTiles   = maxTiles;
			meshParams.maxPolys   = maxPolygons;

			return meshParams;
		}

	} // anonymous namespace

	RecastWorldComponent::RecastWorldComponent(const ObjectLocator& services) ETNoexceptHint : WorldComponent(services), _scene(nullptr) {}

	// ---------------------------------------------------

	void RecastWorldComponent::OnFixedRateTickLate(JobExecutor& /*executor*/, MicrosecondTime duration) {
		ET_PROFILE_SCOPE("World/LateTick", "Update Recast navigation", 0xBBBBBB);

		_scene->Simulate(duration);
	}

	// ---------------------------------------------------

	void RecastWorldComponent::BindResources(JobExecutor& /*executor*/) {
		const dtNavMeshParams meshParams(AsNavMeshParams(/*origin =*/Vector::MakeIdentity(), /*tileDimensions =*/Vector(10.0f, 12.0f, 10.0f, 1.0f), /*maxTiles =*/4096u, /*maxPolygons =*/16384u));

		ET_TERMINATE_WORLD_UNLESS(_scene && Succeeded(_scene->BindResources(meshParams, /*maxAgents =*/4096u, /*agentRadius =*/2.0f)));
	}

	// ---------------------------------------------------

	void RecastWorldComponent::FreeResources(JobExecutor& /*executor*/) {
		if (NavigationScene* const scene = eastl::exchange(_scene, nullptr)) {
			scene->FreeResources();
		}
	}

}}} // namespace Eldritch2::Navigation::Recast
