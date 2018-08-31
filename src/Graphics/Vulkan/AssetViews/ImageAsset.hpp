/*==================================================================*\
  ImageAsset.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/CrunchImageSource.hpp>
#include <Assets/Asset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan { namespace AssetViews {

	class ImageAsset : public CrunchImageSource, public Assets::Asset {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		ImageAsset(const ImageAsset&) = delete;
		//!	Constructs this @ref ImageAsset instance.
		/*!	@param[in] path String view containing the file system path to the asset the @ref ImageAsset describes. */
		ImageAsset(StringView path);

		~ImageAsset() override = default;

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(const Builder& builder) override;

		void FreeResources() override;

		// ---------------------------------------------------

	public:
		static ETPureFunctionHint StringView GetExtension() ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		ImageAsset& operator=(const ImageAsset&) = delete;
	};

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews
