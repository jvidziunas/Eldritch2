/*==================================================================*\
  ImageAsset.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/AssetViews/BasisImageAsset.hpp>
#include <Logging/Log.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan { namespace AssetViews {

	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;

	// ---------------------------------------------------

	BasisImageAsset::BasisImageAsset(StringSpan path) ETNoexceptHint : Asset(path) {}

	// ---------------------------------------------------

	Result BasisImageAsset::BindResources(Log& log, const AssetBuilder& asset) {
		ET_ABORT_UNLESS(BasisImageSource::BindResources(asset.bytes), log.Write(Error, "Data integrity check failed for {}, aborting load." ET_NEWLINE, GetPath()));

		return Result::Success;
	}

	// ---------------------------------------------------

	void BasisImageAsset::FreeResources() ETNoexceptHint {
		BasisImageSource::FreeResources();
	}

	// ---------------------------------------------------

	ETPureFunctionHint StringSpan BasisImageAsset::GetExtension() ETNoexceptHint {
		return ".basis";
	}

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews
