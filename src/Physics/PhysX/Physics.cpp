/*==================================================================*\
  Physics.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/AssetViews/PhysicsAsset.hpp>
#include <Physics/PhysX/Physics.hpp>
//------------------------------------------------------------------//
#include <PxArticulationLink.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Physics::PhysX::AssetViews;
using namespace ::Eldritch2::Animation;
using namespace ::physx;

namespace Eldritch2 {
	namespace Physics {
		namespace PhysX {

			Physics::Physics(PhysxPointer<PxArticulation> articulation, Armature& target) : _articulation(eastl::move(articulation)), _target(eastl::addressof(target)) {
				_articulation->userData = this;
			}

		// ---------------------------------------------------

			void Physics::Attach(const PhysicsAsset& asset) {
				const auto	links(static_cast<PxArticulationLink**>(_alloca(sizeof(PxArticulationLink*) * _articulation->getNbLinks())));

				_articulation->getLinks(links, _articulation->getNbLinks());

				for (const auto& shape : asset.GetRigidShapes()) {
					links[shape.linkIndex]->attachShape(*shape.shape);
				}
			}

		// ---------------------------------------------------

			void Physics::Detach(const PhysicsAsset& asset) {
				const auto	links(static_cast<PxArticulationLink**>(_alloca(sizeof(PxArticulationLink*) * _articulation->getNbLinks())));

				_articulation->getLinks(links, _articulation->getNbLinks());

				for (const auto& shape : asset.GetRigidShapes()) {
					links[shape.linkIndex]->detachShape(*shape.shape);
				}
			}

		}	// namespace PhysX
	}	// namespace Physics
}	// namespace Eldritch2