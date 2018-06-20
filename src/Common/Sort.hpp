/*==================================================================*\
  Sort.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  �2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/Compiler.hpp>
//------------------------------------------------------------------//
#include <emmintrin.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	void	Sse41MergeSort(__m128i* list0, __m128i* list0End, __m128i* list1, __m128i* out);

}	// namespace Eldritch2