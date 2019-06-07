/*==================================================================*\
  BipedalAgent.cpp
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
#include <Navigation/Recast/NavigationScene.hpp>
#include <Navigation/Recast/BipedalAgent.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Navigation { namespace Recast {

	using namespace Eldritch2::Animation;

	BipedalAgent::BipedalAgent(Armature& armature) ETNoexceptHint : _armature(ETAddressOf(armature)),
																	_scene(nullptr),
																	_id(NavigationScene::InvalidAgentId) {}

	// ---------------------------------------------------

	BipedalAgent::BipedalAgent(BipedalAgent&& agent) ETNoexceptHint : _armature(agent._armature),
																	  _scene(Exchange(agent._scene, nullptr)),
																	  _id(Exchange(agent._id, NavigationScene::InvalidAgentId)) {}

	// ---------------------------------------------------

	BipedalAgent::~BipedalAgent() {
		StopNavigation();
	}

	// ---------------------------------------------------

	bool ETSimdCall BipedalAgent::TryBeginNavigation(NavigationScene& scene, Vector targetPosition) {
		static ETConstexpr float32 AgentRadius                     = 1.0f;
		static ETConstexpr float32 AgentHeight                     = 2.0f;
		static ETConstexpr float32 MaxAcceleration                 = 3.5f;
		static ETConstexpr float32 MaxSpeed                        = 5.0f;
		static ETConstexpr float32 SeparationWeight                = 5.0f;
		static ETConstexpr float32 CollisionTemporalHorizon        = 0.5f;
		static ETConstexpr float32 PathOptimizationTemporalHorizon = 1.0f;
		static ETConstexpr uint8 AvoidanceId                       = 0u;
		static ETConstexpr uint8 QueryFilterId                     = 0u;
		const dtCrowdAgentParams configuration{
			AgentRadius,
			AgentHeight,
			MaxAcceleration,
			MaxSpeed,
			Clamp(MaxSpeed * CollisionTemporalHorizon, 0.1f, 10.0f),
			Clamp(MaxSpeed * PathOptimizationTemporalHorizon, 0.1f, 10.0f),
			SeparationWeight,
			DT_CROWD_ANTICIPATE_TURNS,
			AvoidanceId,
			QueryFilterId,
			this
		};

		ET16ByteAligned float32 coefficients[4];
		StreamCoefficients(coefficients, targetPosition);

		const int id(scene.addAgent(coefficients, ETAddressOf(configuration)));
		if (id == NavigationScene::InvalidAgentId) {
			return false;
		}

		StopNavigation();

		_scene = ETAddressOf(scene);
		_id    = id;

		return true;
	}

	// ---------------------------------------------------

	void BipedalAgent::StopNavigation() {
		if (_scene == nullptr) {
			return;
		}

		Exchange(_scene, nullptr)->removeAgent(Exchange(_id, NavigationScene::InvalidAgentId));
	}

}}} // namespace Eldritch2::Navigation::Recast
