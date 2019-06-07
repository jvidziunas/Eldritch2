/*==================================================================*\
  VulkanEngineComponent.AssetApi.cpp
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
#include <Graphics/Vulkan/AssetViews/GraphicsPipelineAsset.hpp>
#include <Graphics/Vulkan/AssetViews/SpirVShaderSetAsset.hpp>
#include <Graphics/Vulkan/AssetViews/BasisImageAsset.hpp>
#include <Animation/AssetViews/AnimationTreeAsset.hpp>
#include <Animation/AssetViews/KeyframeClipAsset.hpp>
#include <Graphics/Vulkan/VulkanEngineComponent.hpp>
#include <Graphics/Vulkan/AssetViews/MeshAsset.hpp>
#include <Core/ContentDatabase.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Graphics::Vulkan::AssetViews;
	using namespace ::Eldritch2::Animation::AssetViews;
	using namespace ::Eldritch2::Core;

	// ---------------------------------------------------

	void VulkanEngineComponent::PublishApi(AssetApiBuilder& api) {
		api.DefineType(GraphicsPipelineAsset::GetExtension(), api.MakeFactory<GraphicsPipelineAsset>());
		api.DefineType(SpirVShaderSetAsset::GetExtension(), api.MakeFactory<SpirVShaderSetAsset>());
		api.DefineType(AnimationTreeAsset::GetExtension(), api.MakeFactory<AnimationTreeAsset>());
		api.DefineType(KeyframeClipAsset::GetExtension(), api.MakeFactory<KeyframeClipAsset>());
		api.DefineType(BasisImageAsset::GetExtension(), api.MakeFactory<BasisImageAsset>());
		api.DefineType(MeshAsset::GetExtension(), api.MakeFactory<MeshAsset>());
	}

}}} // namespace Eldritch2::Graphics::Vulkan
