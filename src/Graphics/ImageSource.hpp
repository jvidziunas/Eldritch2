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
#include <Graphics/GpuFormats.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	class ETPureAbstractHint ImageSource {
		// - TYPE PUBLISHING ---------------------------------

	public:
		struct SubimageDescription {
			uint32 widthInTexels;
			uint32 heightInTexels;
			uint32 depthInTexels;
		};

		// ---

		struct ImageDescription {
			GpuFormat format;
			uint32    slices;
			uint32    mips;
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
		virtual SubimageDescription GetDescription(uint32 subimageId) const abstract;
		virtual ImageDescription    GetDescription() const abstract;

		virtual void StreamTexels(const StreamRequest& request) const abstract;
	};

	// ---

	ETPureFunctionHint uint32 GetSubimageIndex(uint32 slice, uint32 mip, uint32 imageMips) ETNoexceptHint;

}} // namespace Eldritch2::Graphics

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/ImageSource.inl>
//------------------------------------------------------------------//
