//
// src\Common\Containers\SimdSort_generated.h
// (Header automatically generated by the ispc compiler.)
// DO NOT EDIT THIS FILE.
//

#ifndef ISPC_SRC_COMMON_CONTAINERS_SIMDSORT_GENERATED_H
#define ISPC_SRC_COMMON_CONTAINERS_SIMDSORT_GENERATED_H

#include <stdint.h>



#ifdef __cplusplus
namespace ispc { /* namespace */
#endif // __cplusplus
///////////////////////////////////////////////////////////////////////////
// Vector types with external visibility from ispc code
///////////////////////////////////////////////////////////////////////////

#ifndef __ISPC_VECTOR_int32_t2__
#define __ISPC_VECTOR_int32_t2__
#ifdef _MSC_VER
__declspec( align(32) ) struct int32_t2 { int32_t v[2]; };
#else
struct int32_t2 { int32_t v[2]; } __attribute__ ((aligned(32)));
#endif
#endif

#ifndef __ISPC_VECTOR_int64_t2__
#define __ISPC_VECTOR_int64_t2__
#ifdef _MSC_VER
__declspec( align(32) ) struct int64_t2 { int64_t v[2]; };
#else
struct int64_t2 { int64_t v[2]; } __attribute__ ((aligned(32)));
#endif
#endif



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
    extern void MergeSort32(int32_t2   * records, uint32_t recordCount);
    extern void MergeSort64(int64_t2   * records, uint32_t recordCount);
#if defined(__cplusplus) && (! defined(__ISPC_NO_EXTERN_C) || !__ISPC_NO_EXTERN_C )
} /* end extern C */
#endif // __cplusplus


#ifdef __cplusplus
} /* namespace */
#endif // __cplusplus

#endif // ISPC_SRC_COMMON_CONTAINERS_SIMDSORT_GENERATED_H