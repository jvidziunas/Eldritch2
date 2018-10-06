/*==================================================================*\
  RgbColor.hpp
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

	class RgbColor {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref RgbColor instance.
		ETConstexpr RgbColor(const RgbColor&) ETNoexceptHint = default;
		//!	Constructs this @ref RgbColor instance.
		RgbColor(float32 red, float32 green, float32 blue);

		~RgbColor() = default;

		// ---------------------------------------------------

	public:
		float16 GetRed() const ETNoexceptHint;

		float16 GetGreen() const ETNoexceptHint;

		float16 GetBlue() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		float32 GetRec709Luminance() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		RgbColor& operator=(const RgbColor&) = default;

		// - DATA MEMBERS ------------------------------------

	private:
		float16 _channels[3];
	};

	// ---

	ETPureFunctionHint RgbColor GetRgbFromCieXyz(float32 x, float32 y, float32 z);

	ETPureFunctionHint RgbColor GetRgbFromEv(uint8 red, uint8 green, uint8 blue, float32 exposureStops = 1.0f);

	ETPureFunctionHint RgbColor GetRgbFromKelvin(float32 degrees);

}} // namespace Eldritch2::Graphics

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/RgbColor.inl>
//------------------------------------------------------------------//
