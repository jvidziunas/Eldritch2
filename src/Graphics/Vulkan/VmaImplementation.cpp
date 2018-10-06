/*==================================================================*\
  VmaImplementation.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/VmaMutex.hpp>
//------------------------------------------------------------------//
#define VMA_ASSERT(expression) ET_ASSERT(expression, #expression)
#define VMA_MIN(x, y) ::Eldritch2::Min((x), (y))
#define VMA_MAX(x, y) ::Eldritch2::Max((x), (y))
#define VMA_SWAP(x, y) ::Eldritch2::Swap((x), (y))
#define VMA_SORT(begin, end, comp) ::Eldritch2::Sort(begin, end, comp)
#define VMA_ALIGN_OF(type) ETAlignOf(type)
#define VMA_MUTEX ::Eldritch2::Graphics::Vulkan::VmaMutex
#define VMA_ATOMIC_UINT32 ::Eldritch2::Atomic<::Eldritch2::uint32>
#define VMA_IMPLEMENTATION
/* (6011) Possible null pointer dereference.
 * (6031) `snprintf` return value ignored.
 * (6326) Comparison of constant with constant.
 * (6386) Possible buffer write overrun.
 * (6387) Memcpy target may be null. */
ET_PUSH_MSVC_WARNING_STATE(disable : 6011 6031 6326 6386 6387)
#include <vk_mem_alloc.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//
