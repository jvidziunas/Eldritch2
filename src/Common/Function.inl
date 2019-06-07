/*==================================================================*\
  Function.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2019 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

template <size_t size, typename Signature>
ETForceInlineHint void Swap(FixedFunction<size, Signature>& lhs, FixedFunction<size, Signature>& rhs) {
	eastl::swap(lhs, rhs);
}

// ---------------------------------------------------

template <typename Signature>
ETForceInlineHint void Swap(Function<Signature>& lhs, Function<Signature>& rhs) {
	eastl::swap(lhs, rhs);
}

} // namespace Eldritch2
