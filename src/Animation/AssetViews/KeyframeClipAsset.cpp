/*==================================================================*\
  KeyframeClipAsset.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/AssetViews/KeyframeClipAsset.hpp>
//------------------------------------------------------------------//
#include <Flatbuffers/Animation_generated.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Animation::FlatBuffers;
using namespace ::Eldritch2::Logging;
using namespace ::Eldritch2::Assets;
using namespace ::flatbuffers;

namespace Eldritch2 {
	namespace Animation {
		namespace AssetViews {

			KeyframeClipAsset::KeyframeClipAsset(const Utf8Char* const filePath) : Asset(filePath) {}

		// ---------------------------------------------------

			ErrorCode KeyframeClipAsset::BindResources(const Builder& asset) {
			//	Ensure we're working with data that can plausibly represent a keyframe animation clip.
				Verifier verifier(reinterpret_cast<const uint8_t*>(asset.Begin()), asset.GetSize());
				if (!VerifyAnimationBuffer(verifier)) {
					asset.WriteLog(MessageType::Error, "Data integrity check failed for {}, aborting load." UTF8_NEWLINE, GetPath());
					return Error::InvalidParameter;
				}

				return Error::None;
			}

		// ---------------------------------------------------

			void KeyframeClipAsset::FreeResources() {}

		}	// namespace AssetViews
	}	// namespace Animation
}	// namespace Eldritch2