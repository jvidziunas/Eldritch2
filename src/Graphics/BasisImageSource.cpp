/*==================================================================*\
  BasisImageSource.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/BasisImageSource.hpp>
//------------------------------------------------------------------//

//==================================================================//
// THIRD-PARTY SOURCE
//==================================================================//
#include <basis/transcoder/basisu_transcoder.cpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	namespace {

		basist::etc1_global_selector_codebook etc1Codebook(0u, nullptr);

	} // anonymous namespace

	BasisImageSource::BasisImageSource() ETNoexceptHint : _transcoder(ETAddressOf(etc1Codebook)) {}

	// ---------------------------------------------------

	BasisImageSource::~BasisImageSource() {
		FreeResources();
	}

	// ---------------------------------------------------

	ImageDescriptor BasisImageSource::DescribeSelf() const ETNoexceptHint {
		return ImageDescriptor{
			/*format =*/GpuFormat::BC1_Srgb,
			/*smallestMip =*/0u,
			/*slices =*/1u,
			/*size =*/ { /*width =*/1u, /*height =*/1u, /*depth =*/1u, }
		};
	}

	// ---------------------------------------------------

	void BasisImageSource::StreamTexels(const StreamRequest& request) const ETNoexceptHint {
		ForEach(request.targets.Begin(), request.targets.End(), AsCounter(request.firstSubimage), [this](byte* target, uint32 image) ETNoexceptHint {
			_transcoder.transcode_image_level(/*pData =*/nullptr, /*data_size =*/0u, /*image_index =*/0u, image, target, 0u, basist::cTFBC1);
		});
	}

	// ---------------------------------------------------

	Result BasisImageSource::BindResources(Span<const byte*> bytes) {
		ET_ABORT_UNLESS(_transcoder.start_transcoding(bytes.Begin(), bytes.GetSize()) ? Result::Success : Result::InvalidParameter);

		return _transcoder.get_ready_to_transcode() ? Result::Success : Result::InvalidObjectState;
	}

	// ---------------------------------------------------

	void BasisImageSource::FreeResources() ETNoexceptHint {
		// This space intentionally blank.
	}

}} // namespace Eldritch2::Graphics
