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
#include <Graphics/PipelineBuilder.hpp>
#include <Core/Asset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan { namespace AssetViews {

	class GraphicsPipelineAsset : public PipelineBuilder, public Core::Asset {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		GraphicsPipelineAsset(const GraphicsPipelineAsset&) = delete;
		//!	Constructs this @ref GraphicsPipelineAsset instance.
		/*!	@param[in] path UTF-8-encoded string view containing the path to the asset
				the @ref GraphicsPipelineAsset is describing. */
		GraphicsPipelineAsset(StringSpan path) ETNoexceptHint;

		~GraphicsPipelineAsset() override = default;

		// ---------------------------------------------------

	public:
		Result BindResources(Logging::Log& log, const Core::AssetBuilder& asset) override;

		void FreeResources() ETNoexceptHint override;

		// ---------------------------------------------------

	public:
		static ETPureFunctionHint StringSpan GetExtension() ETNoexceptHint;

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
