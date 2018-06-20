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
#define VMA_ASSERT(expression) ET_ASSERT(expression, #expression)
#define VMA_MIN(x, y) Eldritch2::Min((x), (y))
#define VMA_MAX(x, y) Eldritch2::Max((x), (y))
#define VMA_SWAP(x, y) Eldritch2::Swap((x), (y))
#define VMA_SORT(begin, end, comp) eastl::sort(begin, end, comp)
#define VMA_IMPLEMENTATION
#include <eastl/sort.h>
#include <vk_mem_alloc.h>
//------------------------------------------------------------------//
