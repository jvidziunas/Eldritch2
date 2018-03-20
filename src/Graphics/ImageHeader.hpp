/*==================================================================*\
  ImageHeader.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/GpuFormats.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {

	struct ImageHeader {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum ImageClass : uint8 {
			Texture2D,
			Texture3D,
			Cubemap
		};

	// ---

		struct SamplingDescriptor {
			GpuBitLayout		layout;
			GpuStorageFormat	storageFormat;
			GpuFilteringFlags	filteringFlags;
		};

	// ---

		struct Header2D {
			uint32		topLevelWidthInPixels;
			uint32		topLevelHeightInPixels;
			uint32		arraySizeInSlices;
		};

	// ---

		struct Header3D {
			uint32		topLevelWidthInPixels;
			uint32		topLevelHeightInPixels;
			uint32		topLevelDepthInPixels;
		};

	// ---

		struct HeaderCubemap {
			uint32		edgeSizeInPixels;
			uint32		arraySizeInCubemaps;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	//!	Constructs this @ref ImageHeader instance.
		ImageHeader( const SamplingDescriptor& samplingDescriptor, const HeaderCubemap& header );
	//!	Constructs this @ref ImageHeader instance.
		ImageHeader( const SamplingDescriptor& samplingDescriptor, const Header2D& header );
	//!	Constructs this @ref ImageHeader instance.
		ImageHeader( const SamplingDescriptor& samplingDescriptor, const Header3D& header );
	//!	Constructs this @ref ImageHeader instance.
		ImageHeader() = default;

		~ImageHeader() = default;

	// ---------------------------------------------------
		
		SamplingDescriptor	samplingDescriptor;
		ImageClass			imageClass;
		union {
			Header2D		as2D;
			Header3D		as3D;
			HeaderCubemap	asCubemap;
		}					image;
	};

}	// namespace Renderer
}	// namespace Eldritch2