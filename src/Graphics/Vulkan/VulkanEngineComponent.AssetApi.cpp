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
#include <Graphics/Vulkan/AssetViews/SpirVShaderPackageAsset.hpp>
#include <Graphics/Vulkan/AssetViews/GraphicsPipelineAsset.hpp>
#include <Animation/AssetViews/AnimationTreeAsset.hpp>
#include <Animation/AssetViews/KeyframeClipAsset.hpp>
#include <Graphics/Vulkan/VulkanEngineComponent.hpp>
#include <Graphics/Vulkan/AssetViews/ImageAsset.hpp>
#include <Graphics/Vulkan/AssetViews/MeshAsset.hpp>
#include <Assets/AssetDatabase.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	using namespace ::Eldritch2::Graphics::Vulkan::AssetViews;
	using namespace ::Eldritch2::Animation::AssetViews;
	using namespace ::Eldritch2::Assets;

	void VulkanEngineComponent::AcceptVisitor( AssetApiBuilder& factories ) {
		MICROPROFILE_SCOPEI( "Engine/Initialization", "Asset factory registration", 0xBBBBBB );

		factories.DefineType<SpirVShaderPackageAsset>( ".e2spirvshaderpackage" );
		factories.DefineType<GraphicsPipelineAsset>( ".e2renderpipeline" );
		factories.DefineType<KeyframeClipAsset>( ".e2anim" );
		factories.DefineType<ImageAsset>( ".e2image" );
		factories.DefineType<MeshAsset>( ".e2mesh" );
	}

// ---------------------------------------------------

	void VulkanEngineComponent::AcceptVisitor( Blackboard& services ) {
		services.Publish<Vulkan>( _vulkan );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2