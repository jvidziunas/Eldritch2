/*==================================================================*\
  ImageAsset.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/AssetViews/ImageAsset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan { namespace AssetViews {

	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;

	ImageAsset::ImageAsset(StringView path) :
		Asset(path) {}

	// ---------------------------------------------------

	ErrorCode ImageAsset::BindResources(const Builder& asset) {
		if (Failed(CrunchImageSource::BindResources(asset.Begin(), asset.End()))) {
			asset.WriteLog(Severity::Error, "Data integrity check failed for {}, aborting load." ET_NEWLINE, GetPath());
			return Error::Unspecified;
		}

		return Error::None;
	}

	// ---------------------------------------------------

	void ImageAsset::FreeResources() {
		CrunchImageSource::FreeResources();
	}

	// ---------------------------------------------------

	ETPureFunctionHint StringView ImageAsset::GetExtension() ETNoexceptHint {
		return ".e2image";
	}

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews
