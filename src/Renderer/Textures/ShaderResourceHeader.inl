/*==================================================================*\
  ShaderResourceHeader.inl
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
	bool ShaderResourceHeader::SamplingDescriptor::Serialize( Archive& archive ) {
		return archive( layout, storageFormat, filteringFlags );
	}

// ---------------------------------------------------

	template <typename Archive>
	bool ShaderResourceHeader::Header2D::Serialize( Archive& archive ) {
		return archive( topLevelWidthInPixels, topLevelHeightInPixels, arraySizeInSlices );
	}

// ---------------------------------------------------

	template <typename Archive>
	bool ShaderResourceHeader::Header3D::Serialize( Archive& archive ) {
		return archive( topLevelWidthInPixels, topLevelHeightInPixels, topLevelDepthInPixels );
	}

// ---------------------------------------------------

	template <typename Archive>
	bool ShaderResourceHeader::HeaderCubemap::Serialize( Archive& archive ) {
		return archive( edgeSizeInPixels, arraySizeInSlices );
	}

// ---------------------------------------------------

	template <typename Archive>
	bool ShaderResourceHeader::Serialize( Archive& archive ) {
		// Handle basic header information
		if( !archive( _samplingDescriptor, _class ) ) {
			return false;
		}
			
		switch( _class ) {
			case Class::TEXTURE_2D: {
				if( !archive( _headers.header2D ) ) {
					return false;
				}
				break;
			}	// case TextureType::TEXTURE_2D

			case Class::TEXTURE_3D: {
				if( !archive( _headers.header3D ) ) {
					return false;
				}
				break;
			}	// case TextureType::TEXTURE_3D

			case Class::CUBEMAP: {
				if( !archive( _headers.headerCubemap ) ) {
					return false;
				}
				break;
			}	// case TextureType::CUBEMAP
		}

		return true;
	}

// ---------------------------------------------------

	ETInlineHint Renderer::Direct3D11TextureBuilder& ShaderResourceHeader::ConfigureResourceBuilder( Renderer::Direct3D11TextureBuilder& builder ) {
		_samplingDescriptor.ConfigureResourceBuilder( builder );
		
		switch( _class ) {
			case Class::TEXTURE_2D: {
				_headers.header2D.ConfigureResourceBuilder( builder );
				break;
			}	// case TextureType::TEXTURE_2D

			case Class::TEXTURE_3D: {
				_headers.header3D.ConfigureResourceBuilder( builder );
				break;
			}	// case TextureType::TEXTURE_3D

			case Class::CUBEMAP: {
				_headers.headerCubemap.ConfigureResourceBuilder( builder );
				break;
			}	// case TextureType::CUBEMAP
		}

		return builder;
	}

// ---------------------------------------------------

	template <typename Archive>
	bool Serialize( Renderer::ShaderResourceHeader::Class& resourceClass, Archive& archive ) {
		return archive( reinterpret_cast<::std::underlying_type<ShaderResourceHeader::Class>::type&>(resourceClass) );
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

