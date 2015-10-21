/*==================================================================*\
  TextureHeader.hpp
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

	namespace Renderer	= ::Eldritch2::Renderer;

// ---------------------------------------------------

	struct TextureHeader {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum Class : ::Eldritch2::uint8 {
			TEXTURE_2D,
			TEXTURE_3D,
			CUBEMAP
		};

	// ---

		struct SamplingDescriptor {
			template <typename Archive>
			bool	Serialize( Archive& archive );

		// - DATA MEMBERS ------------------------------------

			Renderer::GPULayout			layout;
			Renderer::GPUStorageFormat	storageFormat;
			Renderer::FilteringFlags	filteringFlags;
		};

	// ---

		struct Header2D {
			template <typename Archive>
			bool	Serialize( Archive& archive );

		// - DATA MEMBERS ------------------------------------

			::Eldritch2::uint32		topLevelWidthInPixels;
			::Eldritch2::uint32		topLevelHeightInPixels;
			::Eldritch2::uint32		arraySizeInSlices;
		};

	// ---

		struct Header3D {
			template <typename Archive>
			bool	Serialize( Archive& archive );

		// - DATA MEMBERS ------------------------------------

			::Eldritch2::uint32		topLevelWidthInPixels;
			::Eldritch2::uint32		topLevelHeightInPixels;
			::Eldritch2::uint32		topLevelDepthInPixels;
		};

	// ---

		struct HeaderCubemap {
			template <typename Archive>
			bool	Serialize( Archive& archive );

		// - DATA MEMBERS ------------------------------------

			::Eldritch2::uint32		edgeSizeInPixels;
			::Eldritch2::uint32		arraySizeInCubemaps;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref TextureHeader instance.
		TextureHeader( const SamplingDescriptor& samplingDescriptor, const Header2D& header );
		//!	Constructs this @ref TextureHeader instance.
		TextureHeader( const SamplingDescriptor& samplingDescriptor, const Header3D& header );
		//!	Constructs this @ref TextureHeader instance.
		TextureHeader( const SamplingDescriptor& samplingDescriptor, const HeaderCubemap& header );
		//!	Constructs this @ref TextureHeader instance.
		TextureHeader() = default;

	public:
		//!	Destroys this @ref TextureHeader instance.
		~TextureHeader() = default;

	// ---------------------------------------------------

		template <typename Archive>
		bool	Serialize( Archive& archive );

	// ---------------------------------------------------
		
		SamplingDescriptor	samplingDescriptor;
		Class				headerClass;
		union {
			Header2D		as2D;
			Header3D		as3D;
			HeaderCubemap	asCubemap;
		}					headers;
	};

}	// namespace Renderer
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Renderer/Textures/TextureHeader.inl>
//------------------------------------------------------------------//