#include "../Eldritch2Math.isph"

export void BulkHermite(uniform size_t trackCount,
						uniform float time,
						uniform float values[],
						const uniform uint16 channels[],
						const uniform float startTimes[],
						const uniform float reciprocalDurations[],
						const uniform half startValues[],
						const uniform half endValues[],
						const uniform half startTangents[],
						const uniform half endTangents[]) {
	foreach (track = 0 ... trackCount) {
		float value = HermiteSpline(half_to_float_fast(startValues[track]),
									half_to_float_fast(startTangents[track]),
									half_to_float_fast(endValues[track]),
									half_to_float_fast(endTangents[track]),
									clamp(reciprocalDurations[track] * (time - startTimes[track]), 0.0, 1.0));
		uint32 channel = channels[track];
		foreach_active(mask) {
			values[extract(channel, mask)] = extract(value, mask);
		}
	}
}

export void BulkHermiteWithDerivative(uniform size_t trackCount,
										uniform float time,
										uniform float values[],
										uniform float derivatives[],
										const uniform uint16 channels[],
										const uniform float startTimes[],
										const uniform float reciprocalDurations[],
										const uniform half startValues[],
										const uniform half endValues[],
										const uniform half startTangents[],
										const uniform half endTangents[]) {
	foreach (track = 0 ... trackCount) {
		float value = HermiteSpline(half_to_float_fast(startValues[track]),
									half_to_float_fast(startTangents[track]),
									half_to_float_fast(endValues[track]),
									half_to_float_fast(endTangents[track]),
									(time - startTimes[track]) * reciprocalDurations[track]); 
		uint32 channel = channels[track];
		foreach_active(mask) {
			uniform uint32 laneChannel = extract(channel, mask);
			uniform float laneValue = extract(value, mask);

			values[laneChannel] = laneValue;
			derivatives[laneChannel] = laneValue;
		}
	}
}

export void LinearBlend(uniform size_t valueCount,
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

export void CopyPalette(uniform size_t valueCount, uniform float values[], const uniform float sourceValues[]) {
	foreach (value = 0 ... valueCount) {
		values[value] = sourceValues[value];
	}
}

export void TransposeTransforms(uniform size_t transformCount, uniform float transforms[][4], const uniform float sourceValues[]) {
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
			uniform size_t offset = extract(transform, mask);

			soa_to_aos4(x, y, z, 0.f, transforms[offset]);
			soa_to_aos4(i, j, k, w, transforms[offset + 1]);
		}
	}
}