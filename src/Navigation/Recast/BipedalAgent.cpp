/*==================================================================*\
  BipedalAgent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Navigation/Recast/NavigationScene.hpp>
#include <Navigation/Recast/BipedalAgent.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Navigation {
		namespace Recast {

			using namespace Eldritch2::Animation;

			BipedalAgent::BipedalAgent(Armature& armature) : _armature(eastl::addressof(armature)), _scene(nullptr), _id(NavigationScene::InvalidAgentId) {}

		// ---------------------------------------------------

			BipedalAgent::BipedalAgent(BipedalAgent&& agent) : _armature(agent._armature), _scene(eastl::exchange(agent._scene, nullptr)), _id(eastl::exchange(agent._id, NavigationScene::InvalidAgentId)) {}

		// ---------------------------------------------------

			BipedalAgent::~BipedalAgent() {
				StopNavigation();
			}

		// ---------------------------------------------------

			bool ETSimdCall BipedalAgent::TryBeginNavigation(NavigationScene& scene, Vector targetPosition) {
			//	Scalars for adjusting planning characteristics.
				static constexpr float32		AgentRadius = 1.0f;
				static constexpr float32		AgentHeight = 2.0f;
				static constexpr float32		MaxAcceleration = 3.5f;
				static constexpr float32		MaxSpeed = 5.0f;
				static constexpr float32		SeparationWeight = 5.0f;
				static constexpr float32		CollisionTemporalHorizon = 0.5f;
				static constexpr float32		PathOptimizationTemporalHorizon = 1.0f;
				static constexpr unsigned char	AvoidanceId = 0u;
				static constexpr unsigned char	QueryFilterId = 0u;

				const dtCrowdAgentParams configuration{
					AgentRadius,
					AgentHeight,
					MaxAcceleration,
					MaxSpeed,
					Clamp(MaxSpeed * CollisionTemporalHorizon,        0.1f, 10.0f),
					Clamp(MaxSpeed * PathOptimizationTemporalHorizon, 0.1f, 10.0f),
					SeparationWeight,
					DT_CROWD_ANTICIPATE_TURNS,
					AvoidanceId,
					QueryFilterId,
					this
				};

				float32	coefficients[4];

				targetPosition.ExtractCoefficients(coefficients);

				const int id(scene.addAgent(coefficients, &configuration));
				if (id == NavigationScene::InvalidAgentId) {
					return false;
				}

				StopNavigation();

				_scene = eastl::addressof(scene);
				_id = id;

				return true;
			}

		// ---------------------------------------------------

			void BipedalAgent::StopNavigation() {
				if (_scene != nullptr) {
					_scene->removeAgent(eastl::exchange(_id, NavigationScene::InvalidAgentId));
				}
			}

		}	// namespace Recast
	}	// namespace Navigation
}	// namespace Eldritch2