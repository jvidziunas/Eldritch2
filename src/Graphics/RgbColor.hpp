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

namespace Eldritch2 {
namespace Graphics {

	class RgbColor {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref RgbColor instance.
		RgbColor( float32 red, float32 green, float32 blue );
	//!	Constructs this @ref RgbColor instance.
		RgbColor( const RgbColor& ) = default;

		~RgbColor() = default;

	// ---------------------------------------------------

	public:
		float16	GetRed() const;

		float16	GetGreen() const;

		float16	GetBlue() const;

	// ---------------------------------------------------

	public:
		float32	GetRec709Luminance() const;

	// ---------------------------------------------------

	public:
		RgbColor&	operator=( const RgbColor& ) = default;

	// - DATA MEMBERS ------------------------------------

	private:
		float16	_channels[3];
	};

// ---

	ETPureFunctionHint RgbColor	GetRgbFromCieXyz( float32 x, float32 y, float32 z );

	ETPureFunctionHint RgbColor	GetRgbFromEv( uint8 red, uint8 green, uint8 blue, float16 exposureStops = float16(1.0f) );

	ETPureFunctionHint RgbColor	GetRgbFromKelvin( float32 degrees );

}	// namespace Graphics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/RgbColor.inl>
//------------------------------------------------------------------//