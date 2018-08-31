/*==================================================================*\
  VulkanEngineComponent.AssetApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  �2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/AssetViews/GraphicsPipelineAsset.hpp>
#include <Graphics/Vulkan/AssetViews/SpirVShaderSetAsset.hpp>
#include <Animation/AssetViews/AnimationTreeAsset.hpp>
#include <Animation/AssetViews/KeyframeClipAsset.hpp>
#include <Graphics/Vulkan/VulkanEngineComponent.hpp>
#include <Graphics/Vulkan/AssetViews/ImageAsset.hpp>
#include <Graphics/Vulkan/AssetViews/MeshAsset.hpp>
#include <Assets/AssetDatabase.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Graphics::Vulkan::AssetViews;
	using namespace ::Eldritch2::Animation::AssetViews;
	using namespace ::Eldritch2::Assets;

	void VulkanEngineComponent::PublishAssetTypes(AssetApiBuilder& types) {
		ET_PROFILE_SCOPE("Engine/Initialization", "Asset factory registration", 0xBBBBBB);
		types.DefineType(GraphicsPipelineAsset::GetExtension(), AssetApiBuilder::DefaultFactory<GraphicsPipelineAsset>());
		types.DefineType(SpirVShaderSetAsset::GetExtension(), AssetApiBuilder::DefaultFactory<SpirVShaderSetAsset>());
		types.DefineType(KeyframeClipAsset::GetExtension(), AssetApiBuilder::DefaultFactory<KeyframeClipAsset>());
		types.DefineType(ImageAsset::GetExtension(), AssetApiBuilder::DefaultFactory<ImageAsset>());
		types.DefineType(MeshAsset::GetExtension(), AssetApiBuilder::DefaultFactory<MeshAsset>());
	}

}}} // namespace Eldritch2::Graphics::Vulkan
