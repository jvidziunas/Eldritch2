/*==================================================================*\
  Physics.PhysicsClip.cpp
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

	PhysicsClip::PhysicsClip(const PxAggregate& aggregate) :
		_aggregate(ETAddressOf(aggregate)) {}

	// ---------------------------------------------------

	void PhysicsClip::FetchKnots(KnotCache& knots, BoneIndex maximumBone, uint64 time) {
		PxActor** const actors(ETStackAlloc(PxActor*, maximumBone));
		_aggregate->getActors(actors, maximumBone);

		const Transformation globalToLocal(AsTransformation(static_cast<PxRigidActor*>(actors[0])->getGlobalPose().getInverse()));
		const auto           knot(knots.Get(_firstKnot));
		for (BoneIndex bone(0u); bone < maximumBone; ++bone) {
			const Transformation poseToLocal(AsTransformation(static_cast<PxRigidActor*>(actors[bone])->getGlobalPose()) * globalToLocal);
			const size_t         firstTrack(bone * TracksPerBone);

			eastl::fill_n(eastl::get<KnotCache::StartTimes>(knot) + firstTrack, TracksPerBone, 0.0f);
			eastl::fill_n(eastl::get<KnotCache::ReciprocalDurations>(knot) + firstTrack, TracksPerBone, 0.0f);
			eastl::fill_n(eastl::get<KnotCache::StartValues>(knot) + firstTrack, TracksPerBone, 0.0f);
			eastl::fill_n(eastl::get<KnotCache::EndValues>(knot) + firstTrack, TracksPerBone, 0.0f);
			eastl::fill_n(eastl::get<KnotCache::StartTangents>(knot) + firstTrack, TracksPerBone, 0.0f);
			eastl::fill_n(eastl::get<KnotCache::EndTangents>(knot) + firstTrack, TracksPerBone, 0.0f);
		}
	}

	// ---------------------------------------------------

	void PhysicsClip::Attach(KnotCache& knots) {
		const size_t     channelCount(_aggregate->getNbActors() * TracksPerBone);
		BoneIndex* const channels(ETStackAlloc(BoneIndex, channelCount));

		eastl::generate_n(channels, channelCount, [index = 0u]() mutable { return index++; });
		_firstKnot = knots.Append(channelCount, channels);
	}

}}} // namespace Eldritch2::Physics::PhysX
