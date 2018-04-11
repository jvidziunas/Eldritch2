/*==================================================================*\
  GraphicsPipeline.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/AssetViews/GraphicsPipelineAsset.hpp>
#include <Graphics/Vulkan/GraphicsPipeline.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	using namespace ::Eldritch2::Graphics::Vulkan::AssetViews;

	GraphicsPipeline::Stage::Stage( const GraphicsScene::GeometryConcept* source ) : source( source ) {}

// ---------------------------------------------------

	GraphicsPipeline::GraphicsPipeline() : _stages( MallocAllocator( "Vulkan Render Pipeline Stage List Allocator" ) ) {}

// ---------------------------------------------------

	GraphicsPipeline::~GraphicsPipeline() {
		ET_ASSERT( _renderPass == nullptr,	"Leaking Vulkan render pass!" );
	}

// ---------------------------------------------------

	VkResult GraphicsPipeline::BindResources( Gpu& gpu, const GraphicsScene& scene, const GraphicsPipelineAsset& asset ) {
		using ::Eldritch2::Swap;

		ArrayList<Stage> stages( _stages.GetAllocator() );
		for (const GraphicsPipelineAsset::StageAsset& stage : asset.GetStages()) {
			stages.EmplaceBack( eastl::addressof( scene.GetConcept( GeometryType::Meshes ) ) );
		}

	//	Commit resources to the pipeline.
		Swap( _stages, stages );

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	void GraphicsPipeline::FreeResources( Gpu& gpu ) {
		_stages.Clear( ReleaseMemorySemantics() );
	}

// ---------------------------------------------------

	void Swap( GraphicsPipeline& pipeline0, GraphicsPipeline& pipeline1 ) {
		using ::Eldritch2::Swap;

		Swap( pipeline0._stages, pipeline1._stages );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2