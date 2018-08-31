/*==================================================================*\
  PhysicsScene.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/PhysxMarshals.hpp>
#include <Physics/PhysX/PhysicsScene.hpp>
//------------------------------------------------------------------//
#include <extensions/PxDefaultSimulationFilterShader.h>
#include <PxPhysics.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {

	using namespace ::physx;

	PhysicsScene::PhysicsScene(
		PhysxPointer<PxScene>             scene,
		PhysxPointer<PxControllerManager> controllerManager,
		PhysxPointer<PxMaterial>          characterMaterial,
		PhysxPointer<PxMaterial>          triggerMaterial) :
		_scene(eastl::move(scene)),
		_controllerManager(eastl::move(controllerManager)),
		_characterMaterial(eastl::move(characterMaterial)),
		_triggerMaterial(eastl::move(triggerMaterial)) {
		_scene->setContactModifyCallback(this);
	}

	// ---------------------------------------------------

	ErrorCode PhysicsScene::StartSimulation(PxReal duration) {
		_controllerManager->computeInteractions(duration);
		_scene->simulate(duration, /*completionTask =*/nullptr, _scratchMemory, PxU32(ETCountOf(_scratchMemory)));

		return Error::None;
	}

	// ---------------------------------------------------

	ErrorCode PhysicsScene::FinishSimulation() {
		const PxContactPairHeader* contacts;
		PxU32                      contactsCount, errorState;
		if (_scene->fetchResultsStart(contacts, contactsCount, true)) {
		}

		_scene->fetchResultsFinish(ETAddressOf(errorState));
		return errorState == 0u ? Error::None : Error::Unspecified;
	}

	// ---------------------------------------------------

	void PhysicsScene::onContactModify(PxContactModifyPair* const /*pairs*/, PxU32 /*count*/) {}

	// ---------------------------------------------------

	PxFilterFlags PhysicsScene::FilterShader(PxFilterObjectAttributes lhsAttributes, PxFilterData lhsFilter, PxFilterObjectAttributes rhsAttributes, PxFilterData rhsFilter, PxPairFlags& flags, const void* constants, PxU32 constantsSize) {
		return PxDefaultSimulationFilterShader(lhsAttributes, lhsFilter, rhsAttributes, rhsFilter, flags, constants, constantsSize);
	}

	// ---------------------------------------------------

	PhysxPointer<PxMaterial> MakeCharacterMaterial() {
		return PhysxPointer<PxMaterial> { PxGetPhysics().createMaterial(0.0f, 0.0f, 0.0f) };
	}

	// ---------------------------------------------------

	PhysxPointer<PxMaterial> MakeTriggerMaterial() {
		return PhysxPointer<PxMaterial> { PxGetPhysics().createMaterial(0.0f, 0.0f, 0.0f) };
	}

}}} // namespace Eldritch2::Physics::PhysX
