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
#include <Common/ScopeGuard.hpp>
#include <Common/Math.hpp>
//------------------------------------------------------------------//
#include <crn_decomp.h>
#include <crnlib.h>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY("crnlib.lib")
//------------------------------------------------------------------//

#define ET_FAIL_UNLESS2(cond) \
	{                         \
		const auto _(cond);   \
		if (_ != 0) {         \
			return _;         \
		}                     \
	}

namespace Eldritch2 {
namespace Tools {

	TextureCompressor::TextureCompressor() :
		_sourcePaths(MallocAllocator("Source Texture Collection Allocator")),
		_threadCount(1u),
		_imageQuality(75u),
		_mipLevels(~0u),
		_alphaToCoverageSamplesPerPixel(0u),
		_correctAlphaTestDistribution(false),
		_isColorData(true) {
	}

	// ---------------------------------------------------

	void TextureCompressor::RegisterOptions(OptionRegistrar& options) {
		options.Register<uint32>("--threadCount", "-t", _threadCount);
		options.Register<uint32>("--quality", "-q", _imageQuality);
		options.Register<uint32>("--mipLevels", "-m", _mipLevels);
		options.Register<uint32>("--a2cSpp", "-s", _alphaToCoverageSamplesPerPixel);
		options.Register<bool>("--correctAlphaTestDistribution", "-a", _correctAlphaTestDistribution);
		options.Register<bool>("--colorData", "-c", _isColorData);

		options.RegisterInputFileHandler([this](Range<const Utf8Char*> path) -> int {
			if (path.IsEmpty()) {
				return -1;
			}

			_sourcePaths.Insert({ path.Begin(), path.End(), MallocAllocator("Source Texture Path Allocator") });

			return 0;
		});
	}

	// ---------------------------------------------------

	int TextureCompressor::ProcessImage(const Utf8Char* path) {
		crn_comp_params   compressorParameters;
		crn_mipmap_params mipmapParameters;

		compressorParameters.set_flag(cCRNCompFlagPerceptual, _isColorData);
		compressorParameters.m_num_helper_threads = Max(_threadCount, 1u) - 1u;

		// Compression phase is always considered pass-through, we generate/load mips ourselves as requested by user.
		mipmapParameters.m_mode = cCRNMipModeUseSourceMips;

		for (uint32 level(0u); level <= _mipLevels; ++level) {
			compressorParameters.m_pImages[0][level] = nullptr;
		}

		crn_uint32 combinedSize(0u);
		const auto data(crn_compress(compressorParameters, mipmapParameters, combinedSize));
		ET_AT_SCOPE_EXIT(crn_free_block(data));
		ET_FAIL_UNLESS2(data ? 0 : -1);

		const crn_uint32 segmentedSize(crnd::crnd_get_segmented_file_size(data, combinedSize));
		ET_FAIL_UNLESS2(crnd::crnd_create_segmented_file(data, combinedSize, nullptr, 0u) ? 0 : -1);

		return 0;
	}

	// ---------------------------------------------------

	int TextureCompressor::Process() {
		for (const String<>& path : _sourcePaths) {
			ET_FAIL_UNLESS2(ProcessImage(path));
		}

		return 0;
	}

}} // namespace Eldritch2::Tools
