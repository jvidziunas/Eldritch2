/*==================================================================*\
  VulkanEngineComponent.AssetApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
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
		types.DefineType(".e2graphicspipeline", AssetApiBuilder::DefaultFactory<GraphicsPipelineAsset>());
		types.DefineType(".spirv.e2shaderset", AssetApiBuilder::DefaultFactory<SpirVShaderSetAsset>());
		types.DefineType(".e2anim", AssetApiBuilder::DefaultFactory<KeyframeClipAsset>());
		types.DefineType(".e2image", AssetApiBuilder::DefaultFactory<ImageAsset>());
		types.DefineType(".e2mesh", AssetApiBuilder::DefaultFactory<MeshAsset>());
	}

}}} // namespace Eldritch2::Graphics::Vulkan
