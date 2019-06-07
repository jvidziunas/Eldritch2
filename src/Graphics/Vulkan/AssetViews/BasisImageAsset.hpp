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
#include <Graphics/BasisImageSource.hpp>
#include <Core/Asset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan { namespace AssetViews {

	class BasisImageAsset : public BasisImageSource, public Core::Asset {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		BasisImageAsset(const BasisImageAsset&) = delete;
		//!	Constructs this @ref BasisImageAsset instance.
		/*!	@param[in] path String span containing the file system path to the asset the @ref BasisImageAsset describes. */
		BasisImageAsset(StringSpan path) ETNoexceptHint;

		~BasisImageAsset() override = default;

		// ---------------------------------------------------

	public:
		Result BindResources(Logging::Log& log, const Core::AssetBuilder& builder) override;

		void FreeResources() ETNoexceptHint override;

		// ---------------------------------------------------

	public:
		static ETPureFunctionHint StringSpan GetExtension() ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		BasisImageAsset& operator=(const BasisImageAsset&) = delete;
	};

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews
