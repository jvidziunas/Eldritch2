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
#include <Renderer/GPUFormats.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {

	struct ImageHeader {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum ImageClass : Eldritch2::uint8 {
			Texture2D,
			Texture3D,
			Cubemap
		};

	// ---

		struct SamplingDescriptor {
			Renderer::GpuLayout			layout;
			Renderer::GpuStorageFormat	storageFormat;
			Renderer::FilteringFlags	filteringFlags;
		};

	// ---

		struct Header2D {
			Eldritch2::uint32		topLevelWidthInPixels;
			Eldritch2::uint32		topLevelHeightInPixels;
			Eldritch2::uint32		arraySizeInSlices;
		};

	// ---

		struct Header3D {
			Eldritch2::uint32		topLevelWidthInPixels;
			Eldritch2::uint32		topLevelHeightInPixels;
			Eldritch2::uint32		topLevelDepthInPixels;
		};

	// ---

		struct HeaderCubemap {
			Eldritch2::uint32		edgeSizeInPixels;
			Eldritch2::uint32		arraySizeInCubemaps;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	//!	Constructs this @ref ImageHeader instance.
		ImageHeader( const SamplingDescriptor& samplingDescriptor, const Header2D& header );
	//!	Constructs this @ref ImageHeader instance.
		ImageHeader( const SamplingDescriptor& samplingDescriptor, const Header3D& header );
	//!	Constructs this @ref ImageHeader instance.
		ImageHeader( const SamplingDescriptor& samplingDescriptor, const HeaderCubemap& header );
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