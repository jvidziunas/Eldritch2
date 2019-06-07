/*==================================================================*\
  AnimateSkeleton.ispc
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2019 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include "../Eldritch2Math.isph"
//------------------------------------------------------------------//

float AsAlpha(uniform uint16 time, uint16 duration) {
	unsigned int factor = time - duration;
	return floatbits(factor);
}

export void BulkHermite(uniform uint32 trackCount,
						uniform uint16 time,
						uniform float values[],
						const uniform uint16 channels[],
						const uniform uint16 times[],
						const uniform uint16 durations[],
						const uniform half startValues[],
						const uniform half endValues[],
						const uniform half startTangents[],
						const uniform half endTangents[]) {
	foreach (track = 0 ... trackCount) {
		float splines = HermiteSpline(half_to_float_fast(startValues[track]),
										half_to_float_fast(startTangents[track]),
										half_to_float_fast(endValues[track]),
										half_to_float_fast(endTangents[track]),
										AsAlpha(time, durations[track]));
		uint32 channel = channels[track];
		foreach_active(mask) {
			values[extract(channel, mask)] = extract(splines, mask);
		}
	}
}

export void BulkHermiteWithDerivative(uniform uint32 trackCount,
										uniform uint16 time,
										uniform float values[],
										uniform float derivatives[],
										const uniform uint16 channels[],
										const uniform uint16 times[],
										const uniform uint16 durations[],
										const uniform half startValues[],
										const uniform half endValues[],
										const uniform half startTangents[],
										const uniform half endTangents[]) {
	foreach (track = 0 ... trackCount) {
		float splines = HermiteSpline(half_to_float_fast(startValues[track]),
										half_to_float_fast(startTangents[track]),
										half_to_float_fast(endValues[track]),
										half_to_float_fast(endTangents[track]),
										AsAlpha(time, durations[track])); 
		uint32 channel = channels[track];
		foreach_active(mask) {
			uniform uint32 laneChannel = extract(channel, mask);
			uniform float laneValue = extract(splines, mask);

			values[laneChannel] = laneValue;
			derivatives[laneChannel] = laneValue;
		}
	}
}

export void LinearBlend(uniform uint32 valueCount,
						uniform float finalValues[],
						const uniform float sourceValues[],
						uniform float sourceFactor,
						const uniform float targetValues[],
						uniform float targetFactor) {
	foreach (value = 0 ... valueCount) {
		finalValues[value] = sourceValues[value] * sourceFactor +
							 targetValues[value] * targetFactor;
	}
}

export void CopyPalette(uniform uint32 valueCount, uniform float values[], const uniform float sourceValues[]) {
	foreach (value = 0 ... valueCount) {
		values[value] = sourceValues[value];
	}
}

export void TransposeTransforms(uniform uint32 transformCount, uniform float transforms[][4], const uniform float sourceValues[]) {
	foreach (transform = 0 ... transformCount) {
		float x = sourceValues[transform];
		float y = sourceValues[transform + 1];
		float z = sourceValues[transform + 2];
		// No vector w component, dealing with points.

		float i = sourceValues[transform + 3];
		float j = sourceValues[transform + 4];
		float k = sourceValues[transform + 5];
		float w = sourceValues[transform + 6];

		foreach_active(mask) {
			uniform uint32 offset = extract(transform, mask);

			soa_to_aos4(x, y, z, 0.f, transforms[offset]);
			soa_to_aos4(i, j, k, w, transforms[offset + 1]);
		}
	}
}