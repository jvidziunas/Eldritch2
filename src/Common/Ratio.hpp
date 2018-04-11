/*==================================================================*\
  Ratio.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/TypeTraits.hpp>
#include <Common/Mpl/IntTypes.hpp>
//------------------------------------------------------------------//
#include <eastl/ratio.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <intmax numerator, intmax denominator>
	struct Ratio {
		static constexpr intmax Numerator   = numerator;
		static constexpr intmax Denominator = denominator;

		using Type = Ratio<Numerator, Denominator>;
	};

// ---

#if 0
	template <class R1, class R2> struct RatioEqual        : public IntegralConstant<bool, Internal::ratio_equal<R1, R2>::Value> {};
	template <class R1, class R2> struct RatioNotEqual     : public IntegralConstant<bool, !RatioEqual<R1, R2>::Value> {};
	template <class R1, class R2> struct RatioLess         : public IntegralConstant<bool, Internal::ratio_less<R1, R2>::Value> {};
	template <class R1, class R2> struct RatioLessEqual    : public IntegralConstant<bool, !RatioLess<R2, R1>::Value> {};
	template <class R1, class R2> struct RatioGreater      : public IntegralConstant<bool, RatioLess<R2, R1>::Value> {};
	template <class R1, class R2> struct RatioGreaterEqual : public IntegralConstant<bool, !RatioLess<R1, R2>::Value> {};
#endif

// ---

//	See https://github.com/OculusVR/Flicks
	using Flicks = Ratio<1, 705600000>;

}	// namespace Eldritch2