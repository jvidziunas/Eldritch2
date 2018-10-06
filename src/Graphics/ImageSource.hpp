/*==================================================================*\
  ImageSource.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/GpuAbi.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	class ETPureAbstractHint ImageSource {
		// - TYPE PUBLISHING ---------------------------------

	public:
		struct ImageDescription {
			GpuFormat format;
			uint32    mips;
			uint32    slices;
			uint32    texelWidth;
			uint32    texelHeight;
			uint32    texelDepth;
		};

		// ---

		struct StreamRequest {
			uint32 subimageId;
			uint32 scanlineStrideInBytes;
			uint32 sliceStrideInBytes;
			void*  target;
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ImageSource instance.
		ImageSource(const ImageSource&) = default;
		//!	Constructs this @ref ImageSource instance.
		ImageSource() = default;

		~ImageSource() = default;

		// ---------------------------------------------------

	public:
		virtual void StreamTexels(const StreamRequest& request) const abstract;

		// ---------------------------------------------------

	public:
		virtual ImageDescription GetDescription() const abstract;
	};

	// ---

	template <GpuFormat format, typename Generator>
	class GeneratedImageSource : public ImageSource {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref GeneratedImageSource instance.
		ETConstexpr GeneratedImageSource(uint32 width, uint32 height, uint32 depth, Generator generator) ETNoexceptHint;
		//! Constructs this @ref GeneratedImageSource instance.
		ETConstexpr GeneratedImageSource(const GeneratedImageSource&) ETNoexceptHint = default;
		//! Constructs this @ref GeneratedImageSource instance.
		ETConstexpr GeneratedImageSource(GeneratedImageSource&&) ETNoexceptHint = default;

		~GeneratedImageSource() = default;

		// ---------------------------------------------------

	public:
		void StreamTexels(const StreamRequest& request) const override;

		// ---------------------------------------------------

	public:
		ImageDescription GetDescription() const override;

		// - DATA MEMBERS ------------------------------------

	private:
		uint32    _width, _height, _depth;
		Generator _generator;
	};

	// ---

	template <GpuFormat format, typename Generator>
	ETConstexpr GeneratedImageSource<format, Generator> MakeGeneratedImage(uint32 width, uint32 height, uint32 depth, Generator generator) ETNoexceptHint;

	ETConstexpr ETPureFunctionHint uint32 GetSubimageIndex(uint32 slice, uint32 mip, uint32 imageMips) ETNoexceptHint;

	ETPureFunctionHint float32 GetModelScreenCoverageConstant(float32 modelViewZ, Angle fov, float32 reciprocalResolution) ETNoexceptHint;

	ETConstexpr ETPureFunctionHint float32 FastTriangleScreenArea(float32 worldArea, float32 screenCoverageConstant) ETNoexceptHint;

	ETPureFunctionHint float32 GetTriangleTextureLod(float32 triangleScreenArea, float32 triangleUvArea) ETNoexceptHint;

	ETPureFunctionHint uint32 GetMipPyramidLevels(uint32 width, uint32 height, uint32 depth = 1u) ETNoexceptHint;

}} // namespace Eldritch2::Graphics

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/ImageSource.inl>
//------------------------------------------------------------------//
