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
		options.Register<uint32>(L"--threadCount", L"-t", _threadCount);
		options.Register<uint32>(L"--quality", L"-q", _imageQuality);
		options.Register<uint32>(L"--mipLevels", L"-m", _mipLevels);
		options.Register<uint32>(L"--a2cSpp", L"-s", _alphaToCoverageSamplesPerPixel);
		options.Register<bool>(L"--correctAlphaTestDistribution", L"-a", _correctAlphaTestDistribution);
		options.Register<bool>(L"--colorData", L"-c", _isColorData);

		options.RegisterInputFileHandler([this](PlatformStringView path) -> int {
			if (path.IsEmpty()) {
				return -1;
			}

			_sourcePaths.Emplace(MallocAllocator("Texture Path Allocator"), path);
			return 0;
		});
	}

	// ---------------------------------------------------

	ErrorCode TextureCompressor::ProcessImage(const Path& path) {
		crn_mipmap_params mipmapParameters;
		crn_comp_params   compressorParameters;

		compressorParameters.set_flag(cCRNCompFlagPerceptual, _isColorData);
		compressorParameters.m_num_helper_threads = Max(_threadCount, 1u) - 1u;
		// Compression phase is always considered pass-through, we generate/load mipmaps ourselves as requested by user.
		mipmapParameters.m_mode = cCRNMipModeUseSourceMips;

		for (uint32 level(0u); level <= _mipLevels; ++level) {
			compressorParameters.m_pImages[0][level] = nullptr;
		}

		crn_uint32 combinedSize(0u);
		const auto data(crn_compress(compressorParameters, mipmapParameters, combinedSize));
		ET_AT_SCOPE_EXIT(crn_free_block(data));
		ET_ABORT_UNLESS(data ? Error::None : Error::OutOfMemory);

		const crn_uint32 segmentedSize(crnd::crnd_get_segmented_file_size(data, combinedSize));
		ET_ABORT_UNLESS(crnd::crnd_create_segmented_file(data, combinedSize, nullptr, 0u) ? Error::None : Error::Unspecified);

		return Error::None;
	}

	// ---------------------------------------------------

	ErrorCode TextureCompressor::Process() {
		for (const Path& path : _sourcePaths) {
			ET_ABORT_UNLESS(ProcessImage(path));
		}

		return Error::None;
	}

}} // namespace Eldritch2::Tools
