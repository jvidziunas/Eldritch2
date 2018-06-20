/*==================================================================*\
  GpuFormats.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	enum class GpuBitLayout : uint8 {
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
		Bc1 = Dxt1,
		Dxt3,
		Bc2 = Dxt3,
		Dxt5,
		Bc3 = Dxt5,
		Bc4,
		Bc5,
		Bc6,
		Bc7,
		Ctx1,

		Count
	};

	// ---------------------------------------------------

	enum class GpuStorageFormat : uint8 {
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

	enum GpuFilteringFlags : uint8 {
		InterpolateAt16BitIfAvailable = 1u,
		SampleAsSrgb                  = 2u
	};

}} // namespace Eldritch2::Graphics
