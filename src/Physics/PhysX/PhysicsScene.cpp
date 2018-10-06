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
/*	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does.
 *	We can't fix this, but we can at least disable the warning. */
ET_PUSH_MSVC_WARNING_STATE(disable : 6326)
#include <extensions/PxDefaultSimulationFilterShader.h>
#include <characterkinematic/PxControllerManager.h>
#include <PxSceneDesc.h>
#include <PxMaterial.h>
#include <PxPhysics.h>
#include <PxScene.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {

	using namespace ::physx;

	namespace {

		PxFilterFlags FilterShader(PxFilterObjectAttributes lhsAttributes, PxFilterData lhsFilter, PxFilterObjectAttributes rhsAttributes, PxFilterData rhsFilter, PxPairFlags& flags, const void* constants, PxU32 constantsSize) {
			return PxDefaultSimulationFilterShader(lhsAttributes, lhsFilter, rhsAttributes, rhsFilter, flags, constants, constantsSize);
		}

	} // anonymous namespace

	PhysicsScene::PhysicsScene() ETNoexceptHint : _scene(nullptr), _controllerManager(nullptr), _knownMaterials { nullptr } {}

	// ---------------------------------------------------

	void PhysicsScene::Simulate(MicrosecondTime duration) {
		_controllerManager->computeInteractions(AsSeconds(duration));
		_scene->simulate(AsSeconds(duration), /*completionTask =*/nullptr, _scratchMemory, PxU32(ETCountOf(_scratchMemory)));
	}

	// ---------------------------------------------------

	ErrorCode PhysicsScene::FinishSimulation() {
		const PxContactPairHeader* pairs;
		PxU32                      pairsCount, errorState;
		if (_scene->fetchResultsStart(pairs, pairsCount, /*block =*/true)) {
			// Dispatch contacts.
		}

		_scene->fetchResultsFinish(ETAddressOf(errorState));
		return errorState == 0u ? Error::None : Error::Unspecified;
	}

	// ---------------------------------------------------

	ErrorCode PhysicsScene::BindResources(PxCpuDispatcher& dispatcher) {
		PxSceneDesc sceneProperties(PxGetPhysics().getTolerancesScale());
		sceneProperties.filterShader          = ETAddressOf(FilterShader);
		sceneProperties.cpuDispatcher         = ETAddressOf(dispatcher);
		sceneProperties.contactModifyCallback = this;

		PhysxPointer<PxScene> scene(PxGetPhysics().createScene(sceneProperties));
		ET_ABORT_UNLESS(scene ? Error::None : Error::Unspecified);

		PhysxPointer<PxControllerManager> controllerManager(PxCreateControllerManager(*scene, /*lockingEnabled =*/false));
		ET_ABORT_UNLESS(controllerManager ? Error::None : Error::Unspecified);

		PhysxPointer<PxMaterial> knownMaterials[ETCountOf(_knownMaterials)] = {
			PxGetPhysics().createMaterial(/*staticFriction =*/0.0f, /*dynamicFriction =*/0.0f, /*restitution =*/0.0f),
			PxGetPhysics().createMaterial(/*staticFriction =*/0.0f, /*dynamicFriction =*/0.0f, /*restitution =*/0.0f)
		};

		Swap(_scene, scene);
		Swap(_controllerManager, controllerManager);
		Swap(_knownMaterials, knownMaterials);

		return Error::None;
	}

	// ---------------------------------------------------

	void PhysicsScene::FreeResources() {
		for (PhysxPointer<PxMaterial>& material : _knownMaterials) {
			material.Reset();
		}
		_controllerManager.Reset();
		_scene.Reset();
	}

	// ---------------------------------------------------

	void PhysicsScene::onContactModify(PxContactModifyPair* const /*pairs*/, PxU32 /*count*/) {}

}}} // namespace Eldritch2::Physics::PhysX
