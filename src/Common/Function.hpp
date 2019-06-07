/*==================================================================*\
  Function.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <eastl/fixed_function.h>
#include <eastl/internal/function.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <size_t size, typename Signature>
using FixedFunction = eastl::fixed_function<size, Signature>;
template <typename Signature>
using Function = eastl::function<Signature>;

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Function.inl>
//------------------------------------------------------------------//
