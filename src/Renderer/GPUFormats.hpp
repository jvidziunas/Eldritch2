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

	namespace Renderer	= ::Eldritch2::Renderer;

// ---------------------------------------------------

	enum class GPULayout : ::Eldritch2::uint8 {
		_32_32_32_32	= 0,
		_32_32_32		= 1,
		_32_32			= 2,
		_32				= 3,

		_16_16_16_16	= 4,
		_16_16			= 5,
		_16				= 6,

		_10_10_10_2		= 7,

		_11_11_10		= 8,

		_8_8_8_8		= 9,
		_8_8			= 10,
		_8				= 11,

		_24_8			= 12,

		_9_9_9_5		= 13,

		_5_6_5			= 14,

		_DXT1			= 15,
		_BC1			= _DXT1,
		_DXT3			= 16,
		_BC2			= _DXT3,
		_DXT5			= 17,
		_BC3			= _DXT5,
		_BC4			= 18,
		_BC5			= 19,
		_BC6			= 20,
		_BC7			= 21,
		_CTX1			= 22,

		COUNT
	};

// ---------------------------------------------------

	enum class GPUStorageFormat : ::Eldritch2::uint8 {
		NORMALIZED			= 0,
		sRGB_NORMALIZED,
		SIGNED_NORMALIZED,

		FRACTIONAL			= NORMALIZED,
		sRGB_FRACTIONAL		= sRGB_NORMALIZED,
		SIGNED_FRACTIONAL	= SIGNED_NORMALIZED,

		INTEGER,
		SIGNED_INTEGER,

		FLOAT,
		SIGNED_FLOAT,

		COUNT
	};

// ---------------------------------------------------

	enum class TextureType : ::Eldritch2::uint8 {
		TEXTURE_1D,
		TEXTURE_2D,
		TEXTURE_3D,
		CUBEMAP,

		COUNT,
		MALFORMED = COUNT
	};

// ---------------------------------------------------

	typedef ::Eldritch2::uint8	FilteringFlags;

	enum FilteringFlag : Renderer::FilteringFlags {
		INTERPOLATE_AT_16_BIT_IF_AVAILABLE	= 1u,
		SAMPLE_AS_SRGB						= 2u
	};

// ---------------------------------------------------

	typedef ::Eldritch2::uint8	ExtraLayoutFlags;

	enum ExtraLayoutFlag : Renderer::ExtraLayoutFlags {
		CREATE_IN_EDRAM_IF_AVAILABLE	= 1u,
	};

// ---------------------------------------------------

	struct GPUSamplingDescriptor {
		Renderer::GPULayout			layout;
		Renderer::GPUStorageFormat	storageFormat;
		Renderer::FilteringFlags	filteringFlags;
		Renderer::ExtraLayoutFlags	extraLayoutFlags;
	};

}	// namespace Renderer
}	// namespace Eldritch2