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

		struct StreamRequest {
			uint32 scanlineStrideInBytes;
			uint32 sliceStrideInBytes;
			uint32 subimageId;
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

		virtual uint32 GetSliceCount() const abstract;

		virtual uint32 GetMipCount() const abstract;

		virtual void StreamTexels(const StreamRequest& request) const abstract;
	};

	// ---

	ETPureFunctionHint uint32 GetSubimageIndex(uint32 slice, uint32 mip, uint32 totalMips);

}} // namespace Eldritch2::Graphics

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/ImageSource.inl>
//------------------------------------------------------------------//
