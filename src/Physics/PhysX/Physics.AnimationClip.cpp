/*==================================================================*\
  Physics.AnimationClip.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/PhysxMarshals.hpp>
#include <Physics/PhysX/Physics.hpp>
#include <Animation/Armature.hpp>
//------------------------------------------------------------------//
ET_PUSH_COMPILER_WARNING_STATE()
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
ET_SET_MSVC_WARNING_STATE(disable : 6326)
#include <PxArticulationLink.h>
#include <PxRigidBody.h>
#include <PxPhysics.h>
#include <PxScene.h>
ET_POP_COMPILER_WARNING_STATE()
//------------------------------------------------------------------//

using namespace ::Eldritch2::Physics::PhysX::AssetViews;
using namespace ::Eldritch2::Animation;
using namespace ::physx;
using namespace ::ispc;

// PhysX quaternion integration:
#if 0
// Integrate the rotation using closed form quaternion integrator
if (w != 0.0f)
{
	w = PxSqrt(w);
	// Perform a post-solver clamping
	// TODO(dsequeira): ignore this for the moment
	//just clamp motionVel to half float-range
	const PxReal maxW = 1e+7f;		//Should be about sqrt(PX_MAX_REAL/2) or smaller
	if (w > maxW)
	{
		angularMotionVel = angularMotionVel.getNormalized() * maxW;
		w = maxW;
	}
	const PxReal v = dt * w * 0.5f;
	PxReal s, q;
	Ps::sincos(v, s, q);
	s /= w;

	const PxVec3 pqr = angularMotionVel * s;
	const PxQuat quatVel(pqr.x, pqr.y, pqr.z, 0);
	PxQuat result = quatVel * solverBodyData.body2World.q;

	result += solverBodyData.body2World.q * q;

	solverBodyData.body2World.q = result.getNormalized();
	PX_ASSERT(solverBodyData.body2World.q.isSane());
	PX_ASSERT(solverBodyData.body2World.q.isFinite());
}
#endif

namespace Eldritch2 { namespace Physics { namespace PhysX {

	using namespace ::Eldritch2::Animation;

	Physics::AnimationClip::AnimationClip(PhysxPointer<PxArticulation> articulation, const Animation::Armature& target) :
		_articulation(eastl::move(articulation)),
		_target(eastl::addressof(target)) {}

	// ---------------------------------------------------

	void Physics::AnimationClip::FetchKnots(KnotCache& knots, uint64 time, BoneIndex maximumBone) {
		enum : uint32 { TracksPerBone = 8u };

		PxArticulationLink** const links(ETStackAlloc(PxArticulationLink*, _articulation->getNbLinks()));
		_articulation->getLinks(links, _articulation->getNbLinks());

		const Transformation worldToLocal(_target->GetWorldToLocal());
		float32              startTimes[TracksPerBone];
		float32              endTimes[TracksPerBone];
		uint16               startValues[TracksPerBone];
		uint16               endValues[TracksPerBone];
		uint16               startTangents[TracksPerBone];
		uint16               endTangents[TracksPerBone];

		for (BoneIndex bone(1u); bone < maximumBone; ++bone) {
			const Transformation poseToLocal(AsTransformation(links[bone]->getGlobalPose()) * worldToLocal);

			knots.PushKnots(_firstKnot + (bone * TracksPerBone), startTimes, endTimes, startValues, endValues, startTangents, endTangents);
		}
	}

}}} // namespace Eldritch2::Physics::PhysX
