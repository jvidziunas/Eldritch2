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
#include <vulkan/vulkan_core.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace AssetViews {
	class ImageAsset;
}}} // namespace Eldritch2::Graphics::AssetViews

namespace Eldritch2 { namespace Graphics { namespace Vulkan { namespace AssetViews {

	class GraphicsPipelineAsset : public Assets::Asset {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref GraphicsPipelineAsset instance.
		/*!	@param[in] filePath Null-terminated, UTF-8-encoded character sequence containing the
		   file system path to the asset this @ref GraphicsPipelineAsset is describing. */
		GraphicsPipelineAsset(const Utf8Char* const filePath);
		//!	Disable copy construction.
		GraphicsPipelineAsset(const GraphicsPipelineAsset&) = delete;

		~GraphicsPipelineAsset() override = default;

		// ---------------------------------------------------

	public:
		const GraphicsPipelineBuilder& GetBuilder() const;

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(const Builder& asset) override;

		void FreeResources() override;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		GraphicsPipelineAsset& operator=(const GraphicsPipelineAsset&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		GraphicsPipelineBuilder _pipelineBuilder;
	};

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/AssetViews/GraphicsPipelineAsset.inl>
//------------------------------------------------------------------//
