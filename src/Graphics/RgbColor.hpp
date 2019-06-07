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
		ETConstexpr RgbColor(float32 red, float32 green, float32 blue) ETNoexceptHint;
		//!	Constructs this @ref RgbColor instance.
		ETConstexpr RgbColor(const RgbColor&) ETNoexceptHint = default;

		~RgbColor() = default;

		// ---------------------------------------------------

	public:
		ETConstexpr float32 GetRed() const ETNoexceptHint;

		ETConstexpr float32 GetGreen() const ETNoexceptHint;

		ETConstexpr float32 GetBlue() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		ETConstexpr float32 GetRec709Luminance() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		ETConstexpr RgbColor& operator=(const RgbColor&) ETNoexceptHint = default;

		// - DATA MEMBERS ------------------------------------

	private:
		float32 _channels[3];
	};

	// ---

	ETPureFunctionHint RgbColor GetRgbFromCieXyz(float32 x, float32 y, float32 z) ETNoexceptHint;

	ETPureFunctionHint RgbColor GetRgbFromEv(uint8 red, uint8 green, uint8 blue, float32 exposureStops = 1.0f) ETNoexceptHint;

	ETPureFunctionHint RgbColor GetRgbFromKelvin(float32 degrees) ETNoexceptHint;

}} // namespace Eldritch2::Graphics

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/RgbColor.inl>
//------------------------------------------------------------------//
