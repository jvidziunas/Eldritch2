/*==================================================================*\
  GPUFormats.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {

	enum class GpuLayout : Eldritch2::uint8 {
		_32_32_32_32,
		_32_32_32,
		_32_32,
		_32,

		_16_16_16_16,
		_16_16,
		_16,

		_10_10_10_2,

		_11_11_10,

		_8_8_8_8,
		_8_8,
		_8,

		_24_8,

		_9_9_9_5,

		_5_6_5,

		Dxt1,
		Bc1				= Dxt1,
		Dxt3,
		Bc2				= Dxt3,
		Dxt5,
		Bc3				= Dxt5,
		Bc4,
		Bc5,
		Bc6,
		Bc7,
		Ctx1,

		Count
	};

// ---------------------------------------------------

	enum class GpuStorageFormat : Eldritch2::uint8 {
		Normalized,
		SrgbNormalized,
		SignedNormalized,

		Integer,
		SignedInteger,

		Float,
		SignedFloat,

		Count
	};

// ---------------------------------------------------

	enum FilteringFlags : Eldritch2::uint8 {
		InterpolateAt16BitIfAvailable	= 1u,
		SampleAsSrgb					= 2u
	};

}	// namespace Renderer
}	// namespace Eldritch2