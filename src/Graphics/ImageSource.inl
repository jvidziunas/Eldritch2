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

	template <GpuFormat format, typename Generator>
	ETConstexpr ETInlineHint ETForceInlineHint GeneratedImageSource<format, Generator>::GeneratedImageSource(uint32 width, uint32 height, uint32 depth, Generator generator) ETNoexceptHint : _width(width), _height(height), _depth(depth), _generator(eastl::move(generator)) {}

	// ---------------------------------------------------

	template <GpuFormat format, typename Generator>
	ETInlineHint void GeneratedImageSource<format, Generator>::StreamTexels(const StreamRequest& request) const {
		ET_ASSERT(request.subimageId == 0, "Subimage {} out of bounds for image {} (should be < 1)!", request.subimageId, fmt::ptr(this));

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

	template <GpuFormat format, typename Generator>
	ETInlineHint ImageSource::ImageDescription GeneratedImageSource<format, Generator>::GetDescription() const {
		return ImageDescription {
			format,
			/*mips =*/1u,
			/*slices =*/1u,
			_width,
			_height,
			_depth,
		};
	}

	// ---------------------------------------------------

	template <GpuFormat format, typename Generator>
	ETConstexpr ETInlineHint ETForceInlineHint GeneratedImageSource<format, Generator> MakeGeneratedImage(uint32 width, uint32 height, uint32 depth, Generator generator) ETNoexceptHint {
		return GeneratedImageSource<format, Generator>(width, height, depth, eastl::move(generator));
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint ETPureFunctionHint uint32 GetSubimageIndex(uint32 slice, uint32 mip, uint32 imageMips) ETNoexceptHint {
		return slice * imageMips + mip;
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint float32 GetModelScreenCoverageConstant(float32 modelViewZ, Angle fov, float32 reciprocalResolution) ETNoexceptHint {
		const float32 temp(modelViewZ * FastTangent(fov) * reciprocalResolution);
		return temp * temp;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETPureFunctionHint float32 FastTriangleScreenArea(float32 worldArea, float32 screenCoverage) ETNoexceptHint {
		return worldArea * screenCoverage;
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint float32 GetTriangleTextureLod(float32 triangleScreenArea, float32 triangleUvArea) ETNoexceptHint {
		return 0.5f * Log2(triangleScreenArea / triangleUvArea);
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint uint32 GetMipPyramidLevels(uint32 width, uint32 height, uint32 depth) ETNoexceptHint {
		return 1u + Log2(Max(width, height, depth));
	}

}} // namespace Eldritch2::Graphics
