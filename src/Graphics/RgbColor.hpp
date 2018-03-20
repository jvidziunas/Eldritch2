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
		RgbColor( float16 red, float16 green, float16 blue );
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

	ETPureFunctionHint RgbColor	GetRgbFromCieXyz( float16 x, float16 y, float16 z );

	ETPureFunctionHint RgbColor	GetRgbFromKelvin( float32 degrees );

}	// namespace Graphics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/RgbColor.inl>
//------------------------------------------------------------------//