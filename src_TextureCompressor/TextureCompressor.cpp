/*==================================================================*\
  TextureCompressor.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

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

namespace Eldritch2 { namespace Tools {

	using namespace ::Eldritch2::Logging;

	// ---------------------------------------------------

	TextureCompressor::TextureCompressor() ETNoexceptHint : _sourceImages(MallocAllocator("Source Texture Collection Allocator")),
															_threadCount(1u),
															_imageQuality(75u),
															_mipLevels(~0u),
															_alphaToCoverageSamplesPerPixel(0u),
															_correctAlphaTestDistribution(false),
															_isColorData(true) {}

	// ---------------------------------------------------

	void TextureCompressor::RegisterOptions(OptionRegistrar& options) {
		options.Register(SL("--threadCount"), SL("-t"), OptionRegistrar::MakePodSetter(_threadCount));
		options.Register(SL("--quality"), SL("-q"), OptionRegistrar::MakePodSetter(_imageQuality));
		options.Register(SL("--mipLevels") SL("-m"), OptionRegistrar::MakePodSetter(_mipLevels));
		options.Register(SL("--a2cSpp"), SL("-s"), OptionRegistrar::MakePodSetter(_alphaToCoverageSamplesPerPixel));
		options.Register(SL("--correctAlphaTestDistribution"), SL("-a"), OptionRegistrar::MakePodSetter(_correctAlphaTestDistribution));
		options.Register(SL("--colorData"), SL("-c"), OptionRegistrar::MakePodSetter(_isColorData));

		options.RegisterInputFileHandler([this](Log& log, PlatformStringSpan path) -> Result {
			Path       imagePath(MallocAllocator("Texture Path Allocator"), KnownDirectory::Relative, path);
			FileReader reader;

			ET_ABORT_UNLESS(reader.Open(imagePath), log.Write(Error, "Error opening image {}: {}" ET_NEWLINE, path, FUNC_RESULT));
			_sourceImages.EmplaceBack(Move(imagePath), Move(reader));

			return Result::Success;
		});
	}

	// ---------------------------------------------------

	Result TextureCompressor::ProcessImage(Log& log, ImageList::Reference sourceImage) {
		using CrunchSizeType = crn_uint32;
		using CrunchMip      = crn_uint32;

		crn_comp_params   compressor;
		compressor.m_num_helper_threads = Maximum(_threadCount, 1u) - 1u;
		compressor.set_flag(cCRNCompFlagPerceptual, _isColorData);

		crn_mipmap_params mips;
		// Compression phase is always considered pass-through, we generate/load mipmaps ourselves as requested by user.
		mips.m_mode = cCRNMipModeUseSourceMips;
		for (CrunchMip mip(0u); mip <= _mipLevels; ++mip) {
			compressor.m_pImages[0][mip] = nullptr;
		}

		CrunchSizeType imageByteSize(0u);
		const auto     image(crn_compress(compressor, mips, imageByteSize));
		ET_AT_SCOPE_EXIT(crn_free_block(image));
		ET_ABORT_UNLESS(image ? Result::Success : Result::OutOfMemory, log.Write(Error, "Error compressing source image {}" ET_NEWLINE, Get<Path&>(sourceImage)));

		const CrunchSizeType tablesByteSize(crnd::crnd_get_segmented_file_size(image, imageByteSize));
		ET_ABORT_UNLESS(crnd::crnd_create_segmented_file(image, imageByteSize, nullptr, 0u) ? Result::Success : Result::Unspecified);

		return Result::Success;
	}

	// ---------------------------------------------------

	Result TextureCompressor::Process(Log& log) {
		for (ImageList::Reference image : _sourceImages) {
			ET_ABORT_UNLESS(ProcessImage(log, image));
		}

		return Result::Success;
	}

}} // namespace Eldritch2::Tools
