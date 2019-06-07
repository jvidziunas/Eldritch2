/*==================================================================*\
  ImageSource.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	template <GpuFormat Format, typename Generator>
	ETConstexpr ETForceInlineHint GeneratedImageSource<Format, Generator>::GeneratedImageSource(uint32 width, uint32 height, uint32 depth, Generator generator) ETNoexceptHint : _width(width), _height(height), _depth(depth), _generator(Move(generator)) {}

	// ---------------------------------------------------

	template <GpuFormat Format, typename Generator>
	ETInlineHint ETForceInlineHint void GeneratedImageSource<Format, Generator>::StreamTexels(const StreamRequest& request) const ETNoexceptHint {
		ETAssert(request.firstSubimage == 0, "Subimage {} out of bounds for image {} (should be < 1)!", request.firstSubimage, fmt::ptr(this));

		char* slice(request.target);
		for (uint32 w(0u); w < _depth; slice += request.sliceStrideInBytes, ++w) {
			char* line(slice);
			for (uint32 v(0u); v < _height; line += request.scanlineStrideInBytes) {
				for (uint32 u(0u); u < _width; ++u) {
					_generator(u, v, w);
				}
			}
		}
	}

	// ---------------------------------------------------

	template <GpuFormat Format, typename Generator>
	ETInlineHint ETForceInlineHint ImageDescriptor GeneratedImageSource<Format, Generator>::DescribeSelf() const ETNoexceptHint {
		return ImageDescriptor {
			format,
			/*smallestMip =*/1u,
			/*slices =*/1u,
			_width,
			_height,
			_depth,
		};
	}

	// ---------------------------------------------------

	template <GpuFormat Format, typename Generator>
	ETConstexpr ETForceInlineHint GeneratedImageSource<Format, Generator> MakeGeneratedImage(uint32 width, uint32 height, uint32 depth, Generator generator) ETNoexceptHint {
		return GeneratedImageSource<Format, Generator>(width, height, depth, Move(generator));
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint ETPureFunctionHint uint32 AsSubimage(uint32 slice, uint32 mip, uint32 imageMips) ETNoexceptHint {
		return slice * imageMips + mip;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint ETPureFunctionHint float32 GetTriangleTextureLod(float32 triangleScreenArea, float32 triangleUvArea) ETNoexceptHint {
		return 0.5f * Log2(triangleScreenArea / triangleUvArea);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint ETPureFunctionHint uint32 GetMipPyramidLevels(uint32 width, uint32 height, uint32 depth) ETNoexceptHint {
		return 1u + Log2(Maximum(width, height, depth));
	}

}} // namespace Eldritch2::Graphics
