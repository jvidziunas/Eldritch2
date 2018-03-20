/*==================================================================*\
  RenderPipeline.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/AssetViews/RenderPipelineAsset.hpp>
#include <Graphics/Vulkan/RenderPipeline.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	using namespace ::Eldritch2::Graphics::Vulkan::AssetViews;

	RenderPipeline::Stage::Stage( const GraphicsScene::GeometryConcept* source ) : source( source ) {}

// ---------------------------------------------------

	RenderPipeline::RenderPipeline() : _stages( MallocAllocator( "Vulkan Render Pipeline Stage List Allocator" ) ) {}

// ---------------------------------------------------

	RenderPipeline::~RenderPipeline() {
		ET_ASSERT( _renderPass == nullptr,	"Leaking Vulkan render pass!" );
	}

// ---------------------------------------------------

	VkResult RenderPipeline::BindResources( Gpu& gpu, const GraphicsScene& scene, const RenderPipelineAsset& asset ) {
		using ::Eldritch2::Swap;

		ArrayList<Stage> stages( _stages.GetAllocator() );
		for (const RenderPipelineAsset::Stage& stage : asset.GetStages()) {
			stages.EmplaceBack( eastl::addressof( scene.GetConcept( GeometryType::Meshes ) ) );
		}

	//	Commit resources to the pipeline.
		Swap( _stages, stages );

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	void RenderPipeline::FreeResources( Gpu& gpu ) {
		_stages.Clear( ReleaseMemorySemantics() );
	}

// ---------------------------------------------------

	void Swap( RenderPipeline& pipeline0, RenderPipeline& pipeline1 ) {
		using ::Eldritch2::Swap;

		Swap( pipeline0._stages, pipeline1._stages );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2