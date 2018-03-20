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

namespace Eldritch2 {
namespace Graphics {

	class ETPureAbstractHint ImageSource {
	// - TYPE PUBLISHING ---------------------------------

	public:
		struct Dimensions {
			uint32	widthInTexels;
			uint32	heightInTexels;
			uint32	depthInTexels;
			uint16	sliceCount;
			uint16	mips;
		};

	// ---
		
		struct StreamRequest {
			uint32	scanlineStrideInBytes;
			uint32	sliceStrideInBytes;
			uint16	mip;
			uint16	arraySlice;

			void*	target;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ImageSource instance.
		ImageSource( const ImageSource& ) = default;
	//!	Constructs this @ref ImageSource instance.
		ImageSource() = default;

		~ImageSource() = default;

	// ---------------------------------------------------

	public:
		virtual Dimensions	GetDimensions() const abstract;

		virtual void		StreamTexels( const StreamRequest& request ) const abstract;
	};

}	// namespace Graphics
}	// namespace Eldritch2