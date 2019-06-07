/*==================================================================*\
  SpirVShaderSetAsset.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/SpirVShaderSet.hpp>
#include <Core/Asset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan { namespace AssetViews {

	class SpirVShaderSetAsset : public SpirVShaderSet, public Core::Asset {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		SpirVShaderSetAsset(const SpirVShaderSetAsset&) = delete;
		//!	Constructs this @ref SpirVShaderSetAsset instance.
		SpirVShaderSetAsset(StringSpan path) ETNoexceptHint;

		~SpirVShaderSetAsset() override = default;

		// ---------------------------------------------------

	public:
		Result BindResources(Logging::Log& log, const Core::AssetBuilder& builder) override;

		void FreeResources() ETNoexceptHint override;

		// ---------------------------------------------------

	public:
		static ETPureFunctionHint StringSpan GetExtension() ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		SpirVShaderSetAsset& operator=(const SpirVShaderSetAsset&) = delete;
	};

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews
