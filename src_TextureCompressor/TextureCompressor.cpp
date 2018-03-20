/*==================================================================*\
  TextureCompressor.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <TextureCompressor.hpp>
//------------------------------------------------------------------//
#include <crnlib.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Tools {

	TextureCompressor::TextureCompressor(
	) : _isPerceptualData( true ),
		_useMipmaps( true ),
		_quality( 75 ),
		_sourcePaths( MallocAllocator( "Source Texture Collection Allocator" ) ) {
	}

// ---------------------------------------------------

	void TextureCompressor::RegisterOptions( OptionRegistrar& options ) {
		options.Register<uint32>( "--quality",   "-q", _quality );
		options.Register<bool>(   "--colorData", "-c", _isPerceptualData );
		options.Register<bool>(   "--mipmaps",   "-m", _useMipmaps );

		options.RegisterInputFileHandler( [this] ( Range<const Utf8Char*> path ) -> int {
			if (path.IsEmpty()) {
				return -1;
			}

			_sourcePaths.Insert( { path.Begin(), path.End(), MallocAllocator( "Source Texture Path Allocator" ) } );

			return 0;
		} );
	}

// ---------------------------------------------------

	int TextureCompressor::ProcessFile( const Utf8Char* path ) {
		crn_comp_params		compressorParameters;
		crn_mipmap_params	mipGenerationParameters;

		compressorParameters.set_flag( cCRNCompFlagPerceptual, _isPerceptualData );

		mipGenerationParameters.m_gamma_filtering = _isPerceptualData;
		mipGenerationParameters.m_mode = _useMipmaps ? cCRNMipModeGenerateMips : cCRNMipModeNoMips;

		crn_uint32	compressedSize( 0u );
		const auto	data( crn_compress( compressorParameters, mipGenerationParameters, compressedSize ) );
	}

// ---------------------------------------------------

	int TextureCompressor::Process() {
		for (const String<>& path : _sourcePaths) {
			const int	result( ProcessFile( path ) );

			if (result != 0) {
				return result;
			}
		}

		return 0;
	}

}	// namespace Tools
}	// namespace Eldritch2