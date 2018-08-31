/*==================================================================*\
  GraphicsPipelineAsset.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GraphicsPipelineBuilder.hpp>
#include <Assets/Asset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan { namespace AssetViews {

	class GraphicsPipelineAsset : public GraphicsPipelineBuilder, public Assets::Asset {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		GraphicsPipelineAsset(const GraphicsPipelineAsset&) = delete;
		//!	Constructs this @ref GraphicsPipelineAsset instance.
		/*!	@param[in] path UTF-8-encoded string view containing the path to the asset
				the @ref GraphicsPipelineAsset is describing. */
		GraphicsPipelineAsset(StringView path);

		~GraphicsPipelineAsset() override = default;

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(const Builder& asset) override;

		void FreeResources() override;

		// ---------------------------------------------------

	public:
		static ETPureFunctionHint StringView GetExtension() ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		GraphicsPipelineAsset& operator=(const GraphicsPipelineAsset&) = delete;
	};

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/AssetViews/GraphicsPipelineAsset.inl>
//------------------------------------------------------------------//
