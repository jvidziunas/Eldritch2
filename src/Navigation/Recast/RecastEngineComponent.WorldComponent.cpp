/*==================================================================*\
  RecastEngineComponent.WorldComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Navigation/Recast/RecastEngineComponent.hpp>
#include <Navigation/Recast/NavigationScene.hpp>
#include <Core/Profiler.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Navigation { namespace Recast {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Core;

	// ---------------------------------------------------

	namespace {

		ETForceInlineHint ETPureFunctionHint dtNavMeshParams ETSimdCall AsNavMeshParams(Vector origin, Vector tileDimensions, uint32 maxTiles, uint32 maxPolygons) ETNoexceptHint {
			ET16ByteAligned float32 coefficients[4];
			dtNavMeshParams         meshParams;

			StreamCoefficients(coefficients, origin);
			Tie(meshParams.orig[0], meshParams.orig[1], meshParams.orig[2]) = Tie(coefficients[Vector::X], coefficients[Vector::Y], coefficients[Vector::Z]);

			StreamCoefficients(coefficients, tileDimensions);
			Tie(meshParams.tileWidth, meshParams.tileHeight) = Tie(coefficients[Vector::X], coefficients[Vector::Y]);

			meshParams.maxTiles = maxTiles;
			meshParams.maxPolys = maxPolygons;

			return meshParams;
		}

	} // anonymous namespace

	RecastEngineComponent::WorldComponent::WorldComponent(const ObjectInjector& services) ETNoexceptHint : Core::WorldComponent(services), _scene(nullptr) {}

	// ---------------------------------------------------

	void RecastEngineComponent::WorldComponent::OnFixedRateTickLate(JobExecutor& /*executor*/, MicrosecondTime duration) {
		ET_PROFILE_SCOPE("World/LateTick/Recast", "Process navigation", 0xBBBBBB);

		_scene->Simulate(duration);
	}

	// ---------------------------------------------------

	void RecastEngineComponent::WorldComponent::BindResources(JobExecutor& /*executor*/) {
		ET_TERMINATE_WORLD_IF(_scene == nullptr);

		ET_TERMINATE_WORLD_IF_FAILED(_scene->BindResources(/*maxAgents =*/4096u, /*agentRadius =*/2.0f, AsNavMeshParams(/*origin =*/Vector::MakeIdentity(), /*tileDimensions =*/Vector(10.0f, 12.0f, 10.0f, 1.0f), /*maxTiles =*/4096u, /*maxPolygons =*/16384u)));
	}

	// ---------------------------------------------------

	void RecastEngineComponent::WorldComponent::FreeResources(JobExecutor& /*executor*/) {
		if (NavigationScene* const scene = Exchange(_scene, nullptr)) {
			scene->FreeResources();
		}
	}

}}} // namespace Eldritch2::Navigation::Recast
