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
		struct StreamRequest {
			uint32       firstSubimage;
			uint32       scanlineStrideInBytes;
			Span<byte**> targets;
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ImageSource instance.
		ImageSource(const ImageSource&) = default;
		//!	Constructs this @ref ImageSource instance.
		ImageSource() ETNoexceptHint = default;

		~ImageSource() = default;

		// ---------------------------------------------------

	public:
		virtual void StreamTexels(const StreamRequest& request) const ETNoexceptHint abstract;

		// ---------------------------------------------------

	public:
		virtual ImageDescriptor DescribeSelf() const ETNoexceptHint abstract;
	};

	// ---

	template <GpuFormat Format, typename Generator>
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
		void StreamTexels(const StreamRequest& request) const ETNoexceptHint override;

		// ---------------------------------------------------

	public:
		ImageDescriptor DescribeSelf() const ETNoexceptHint override;

		// - DATA MEMBERS ------------------------------------

	private:
		uint32    _width;
		uint32    _height;
		uint32    _depth;
		Generator _generator;
	};

	// ---

	template <GpuFormat Format, typename Generator>
	ETConstexpr GeneratedImageSource<Format, Generator> MakeGeneratedImage(uint32 width, uint32 height, uint32 depth, Generator generator) ETNoexceptHint;

	ETConstexpr ETPureFunctionHint uint32 AsSubimage(uint32 slice, uint32 mip, uint32 imageMips) ETNoexceptHint;

	ETPureFunctionHint float32 GetTriangleTextureLod(float32 triangleScreenArea, float32 triangleUvArea) ETNoexceptHint;

	ETPureFunctionHint uint32 GetMipPyramidLevels(uint32 width, uint32 height, uint32 depth = 1u) ETNoexceptHint;

}} // namespace Eldritch2::Graphics

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/ImageSource.inl>
//------------------------------------------------------------------//
