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
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
ET_PUSH_MSVC_WARNING_STATE(disable : 6326)
#include <PxArticulationLink.h>
#include <PxRigidBody.h>
#include <PxPhysics.h>
#include <PxScene.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

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

	using namespace ::Eldritch2::Physics::PhysX::AssetViews;
	using namespace ::Eldritch2::Animation;
	using namespace ::physx;
	using namespace ::ispc;

	Physics::AnimationClip::AnimationClip(PhysxPointer<PxArticulation> articulation, const Armature& target) :
		_articulation(eastl::move(articulation)),
		_target(ETAddressOf(target)) {}

	// ---------------------------------------------------

	void Physics::AnimationClip::FetchKnots(KnotCache& knots, BoneIndex maximumBone, uint64 time) {
		PxArticulationLink** const links(ETStackAlloc(PxArticulationLink*, _articulation->getNbLinks()));
		const Transformation       worldToLocal(_target->GetWorldToLocal());

		_articulation->getLinks(links, _articulation->getNbLinks());
		for (BoneIndex bone(1u); bone < maximumBone; ++bone) {
			const Transformation poseToLocal(AsTransformation(links[bone]->getGlobalPose()) * worldToLocal);
			auto                 knot(knots.Get(_firstKnot));

			eastl::fill_n(eastl::get<KnotCache::StartTimes>(knot), TracksPerBone, 0.0f);
			eastl::fill_n(eastl::get<KnotCache::ReciprocalDurations>(knot), TracksPerBone, 0.0f);
			eastl::fill_n(eastl::get<KnotCache::StartValues>(knot), TracksPerBone, 0.0f);
			eastl::fill_n(eastl::get<KnotCache::EndValues>(knot), TracksPerBone, 0.0f);
			eastl::fill_n(eastl::get<KnotCache::StartTangents>(knot), TracksPerBone, 0.0f);
			eastl::fill_n(eastl::get<KnotCache::EndTangents>(knot), TracksPerBone, 0.0f);
		}
	}

	// ---------------------------------------------------

	void Physics::AnimationClip::Attach(KnotCache& knots) {
		_firstKnot = knots.Append(_articulation->getNbLinks() * TracksPerBone, nullptr);
	}

}}} // namespace Eldritch2::Physics::PhysX
