/*==================================================================*\
  PhysicsScene.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  �2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/PhysxMarshals.hpp>
#include <Physics/PhysX/PhysicsScene.hpp>
#include <Common/Containers/Range.hpp>
#include <Scheduling/JobExecutor.hpp>
//------------------------------------------------------------------//
#include <extensions/PxDefaultSimulationFilterShader.h>
#include <PxPhysics.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Physics {
		namespace PhysX {

			using namespace ::Eldritch2::Scheduling;
			using namespace ::physx;

			PhysicsScene::PhysicsScene(
				PhysxPointer<PxScene> scene,
				PhysxPointer<PxControllerManager> controllerManager,
				PhysxPointer<PxMaterial> characterMaterial,
				PhysxPointer<PxMaterial> triggerMaterial
			) : _scene(eastl::move(scene)),
				_controllerManager(eastl::move(controllerManager)),
				_characterMaterial(eastl::move(characterMaterial)),
				_triggerMaterial(eastl::move(triggerMaterial)) {
				_scene->setSimulationEventCallback(this);
				_scene->setContactModifyCallback(this);
			}

		// ---------------------------------------------------

			void PhysicsScene::BeginSimulation(JobExecutor& /*executor*/, PxReal duration) {
				_controllerManager->computeInteractions(duration);
				_scene->simulate(duration, nullptr, _scratchMemory, static_cast<PxU32>(_countof(_scratchMemory)));
			}

		// ---------------------------------------------------

			ErrorCode PhysicsScene::JoinSimulation(JobExecutor& executor) {
				executor.AwaitCondition(IsSimulationComplete());

				const PxContactPairHeader*	contacts;
				PxU32						errorState;
				PxU32						count;

				if (_scene->fetchResultsStart(contacts, count, true)) {
					executor.ForEach<16u>(contacts, contacts + count, [](JobExecutor& /*executor*/, const PxContactPairHeader& /*contact*/) {});
				}

				_scene->fetchResultsFinish(&errorState);

				if (errorState != 0u) {
					return Error::Unspecified;
				}

				return Error::None;
			}

		// ---------------------------------------------------

			void PhysicsScene::AbortSimulation(JobExecutor& executor) {
				const PxContactPairHeader*	unused;
				PxU32						unused1;

				executor.AwaitCondition(IsSimulationComplete());

				_scene->fetchResultsStart(unused, unused1, true);
				_scene->fetchResultsFinish();
			}

		// ---------------------------------------------------

			void PhysicsScene::onAdvance(const PxRigidBody*const* /*bodyBuffer*/, const PxTransform* /*poseBuffer*/, const PxU32 /*count*/) {}

		// ---------------------------------------------------

			void PhysicsScene::onConstraintBreak(PxConstraintInfo* /*constraints*/, PxU32 /*count*/) {}

		// ---------------------------------------------------

			void PhysicsScene::onWake(PxActor** /*actors*/, PxU32 /*count*/) {}

		// ---------------------------------------------------

			void PhysicsScene::onSleep(PxActor** /*actors*/, PxU32 /*count*/) {}

		// ---------------------------------------------------

			void PhysicsScene::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* /*pairs*/, PxU32 /*nbPairs*/) {
				if (pairHeader.flags.isSet(PxContactPairHeaderFlag::eREMOVED_ACTOR_0) | pairHeader.flags.isSet(PxContactPairHeaderFlag::eREMOVED_ACTOR_1)) {
					return;
				}
			}

		// ---------------------------------------------------

			void PhysicsScene::onTrigger(PxTriggerPair* pairs, PxU32 count) {
				for (const auto& trigger : Range<PxTriggerPair*>(pairs, pairs + count)) {

				}
			}

		// ---------------------------------------------------

			void PhysicsScene::onContactModify(PxContactModifyPair* const /*pairs*/, PxU32 /*count*/) {}

		// ---------------------------------------------------

			PxFilterFlags PhysicsScene::FilterShader(
				PxFilterObjectAttributes attributes0,
				PxFilterData filterData0,
				PxFilterObjectAttributes attributes1,
				PxFilterData filterData1,
				PxPairFlags& pairFlags,
				const void* constantBlock,
				PxU32 constantBlockSize
			) {
				return PxDefaultSimulationFilterShader(attributes0, filterData0, attributes1, filterData1, pairFlags, constantBlock, constantBlockSize);
			}

		// ---------------------------------------------------

			PhysxPointer<PxMaterial> MakeCharacterMaterial() {
				return PhysxPointer<PxMaterial>{ PxGetPhysics().createMaterial(0.0f, 0.0f, 0.0f) };
			}

		// ---------------------------------------------------

			PhysxPointer<PxMaterial> MakeTriggerMaterial() {
				return PhysxPointer<PxMaterial>{ PxGetPhysics().createMaterial(0.0f, 0.0f, 0.0f) };
			}


		}	// namespace PhysX
	}	// namespace Physics
}	// namespace Eldritch2