//
// src\Animation\AnimateSkeleton_generated.h
// (Header automatically generated by the ispc compiler.)
// DO NOT EDIT THIS FILE.
//

#ifndef ISPC_SRC_ANIMATION_ANIMATESKELETON_GENERATED_H
#define ISPC_SRC_ANIMATION_ANIMATESKELETON_GENERATED_H

#include <stdint.h>



#ifdef __cplusplus
namespace ispc { /* namespace */
#endif // __cplusplus

#ifndef __ISPC_ALIGN__
#if defined(__clang__) || !defined(_MSC_VER)
// Clang, GCC, ICC
#define __ISPC_ALIGN__(s) __attribute__((aligned(s)))
#define __ISPC_ALIGNED_STRUCT__(s) struct __ISPC_ALIGN__(s)
#else
// Visual Studio
#define __ISPC_ALIGN__(s) __declspec(align(s))
#define __ISPC_ALIGNED_STRUCT__(s) __ISPC_ALIGN__(s) struct
#endif
#endif


///////////////////////////////////////////////////////////////////////////
// Functions exported from ispc code
///////////////////////////////////////////////////////////////////////////
#if defined(__cplusplus) && (! defined(__ISPC_NO_EXTERN_C) || !__ISPC_NO_EXTERN_C )
extern "C" {
#endif // __cplusplus
    extern void BulkHermite(uint32_t trackCount, float time, float * values, const uint16_t * channels, const float * startTimes, const float * reciprocalDurations, const uint16_t * startValues, const uint16_t * endValues, const uint16_t * startTangents, const uint16_t * endTangents);
    extern void BulkHermiteWithDerivative(uint32_t trackCount, float time, float * values, float * derivatives, const uint16_t * channels, const float * startTimes, const float * reciprocalDurations, const uint16_t * startValues, const uint16_t * endValues, const uint16_t * startTangents, const uint16_t * endTangents);
    extern void CopyPalette(uint32_t valueCount, float * values, const float * sourceValues);
    extern void LinearBlend(uint32_t valueCount, float * finalValues, const float * sourceValues, float sourceFactor, const float * targetValues, float targetFactor);
    extern void TransposeTransforms(uint32_t transformCount, float transforms[][4], const float * sourceValues);
#if defined(__cplusplus) && (! defined(__ISPC_NO_EXTERN_C) || !__ISPC_NO_EXTERN_C )
} /* end extern C */
#endif // __cplusplus


#ifdef __cplusplus
} /* namespace */
#endif // __cplusplus

#endif // ISPC_SRC_ANIMATION_ANIMATESKELETON_GENERATED_H
