/*==================================================================*\
  TextureResourceHeader.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {

	template <typename Archive>
	bool TextureHeader::SamplingDescriptor::Serialize( Archive& archive ) {
		return archive( layout, storageFormat, filteringFlags );
	}

// ---------------------------------------------------

	template <typename Archive>
	bool TextureHeader::Header2D::Serialize( Archive& archive ) {
		return archive( topLevelWidthInPixels, topLevelHeightInPixels, arraySizeInSlices );
	}

// ---------------------------------------------------

	template <typename Archive>
	bool TextureHeader::Header3D::Serialize( Archive& archive ) {
		return archive( topLevelWidthInPixels, topLevelHeightInPixels, topLevelDepthInPixels );
	}

// ---------------------------------------------------

	template <typename Archive>
	bool TextureHeader::HeaderCubemap::Serialize( Archive& archive ) {
		return archive( edgeSizeInPixels, arraySizeInCubemaps );
	}

// ---------------------------------------------------

	template <typename Archive>
	bool TextureHeader::Serialize( Archive& archive ) {
		// Handle basic header information
		if( !archive( samplingDescriptor, headerClass ) ) {
			return false;
		}
			
		switch( headerClass ) {
			case Class::TEXTURE_2D: {
				if( !archive( headers.as2D ) ) {
					return false;
				}
				break;
			}	// case TextureType::TEXTURE_2D

			case Class::TEXTURE_3D: {
				if( !archive( headers.as3D ) ) {
					return false;
				}
				break;
			}	// case TextureType::TEXTURE_3D

			case Class::CUBEMAP: {
				if( !archive( headers.asCubemap ) ) {
					return false;
				}
				break;
			}	// case TextureType::CUBEMAP
		}

		return true;
	}

// ---------------------------------------------------

	template <typename Archive>
	bool Serialize( Renderer::TextureHeader::Class& resourceClass, Archive& archive ) {
		return archive( reinterpret_cast<::std::underlying_type<TextureHeader::Class>::type&>(resourceClass) );
	}

// ---------------------------------------------------

	template <typename Archive>
	bool Serialize( Renderer::GPULayout& layout, Archive& archive ) {
		return archive( reinterpret_cast<::std::underlying_type<Renderer::GPULayout>::type&>(layout) );
	}

// ---------------------------------------------------

	template <typename Archive>
	bool Serialize( Renderer::GPUStorageFormat& format, Archive& archive ) {
		return archive( reinterpret_cast<::std::underlying_type<Renderer::GPUStorageFormat>::type&>(format) );
	}

// ---------------------------------------------------

	template <typename Archive>
	bool Serialize( Renderer::FilteringFlags& flags, Archive& archive ) {
		return archive( reinterpret_cast<::std::underlying_type<Renderer::FilteringFlags>::type&>(flags) );
	}

}	// namespace Renderer
}	// namespace Eldritch2

