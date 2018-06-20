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
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Graphics::Vulkan::AssetViews;
	using namespace ::Eldritch2::Animation::AssetViews;
	using namespace ::Eldritch2::Assets;

	void VulkanEngineComponent::AcceptVisitor(AssetApiBuilder& factories) {
		MICROPROFILE_SCOPEI("Engine/Initialization", "Asset factory registration", 0xBBBBBB);

		factories.DefineType(".e2graphicspipeline", AssetApiBuilder::DefaultFactory<GraphicsPipelineAsset>());
		factories.DefineType(".spirv.e2shaderset", AssetApiBuilder::DefaultFactory<SpirVShaderSetAsset>());
		factories.DefineType(".e2anim", AssetApiBuilder::DefaultFactory<KeyframeClipAsset>());
		factories.DefineType(".e2image", AssetApiBuilder::DefaultFactory<ImageAsset>());
		factories.DefineType(".e2mesh", AssetApiBuilder::DefaultFactory<MeshAsset>());
	}

	// ---------------------------------------------------

	void VulkanEngineComponent::AcceptVisitor(Blackboard& services) {
		services.Publish<Vulkan>(_vulkan);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
