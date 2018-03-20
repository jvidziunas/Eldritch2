#include "Animation.isph"

//	Local-space bind pose.
struct AnimationKey {
	int16<4>		translation;
	int8<4>			orientation;
	unsigned int16	targetBone;
};

//	Local-space pose.
struct GpuTransform {
	int16<4>	translation;
	int16<4>	orientation;		
};

//	Local-space pose.
struct GpuTransformWithVelocity {
	int16<4>		translation;
	int16<4>		linearVelocity;
	int16<4>		orientation;	
	int16<4>		angularVelocity;
};

export void AnimateSkeleton( uniform GpuTransform out[], uniform const AnimationKey* const uniform tracks[], uniform size_t trackCount ) {
	foreach (track = 0 ... trackCount) {
		GpuTransform result = { 
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 }
		};

		out[tracks[track]->targetBone] = result;
	}
}

export void AnimateSkeletonWithVelocity( uniform GpuTransformWithVelocity out[], uniform const AnimationKey* const uniform tracks[], uniform size_t trackCount ) {
	foreach (track = 0 ... trackCount) {
		GpuTransformWithVelocity result = { 
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 }
		};

		out[tracks[track]->targetBone] = result;
	}
}