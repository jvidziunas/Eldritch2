/*==================================================================*\
  GraphicsPipeline.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/GraphicsScene.hpp>
//------------------------------------------------------------------//
#include <vulkan/vulkan_core.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Graphics {
		namespace Vulkan {
			namespace AssetViews {
				class	GraphicsPipelineAsset;
			}

			class	Gpu;
		}
	}
}

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	class GraphicsPipeline {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class Stage {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref Stage instance.
			Stage( const GraphicsScene::GeometryConcept* source = nullptr );
		//!	Constructs this @ref Stage instance.
			Stage( const Stage& ) = default;

			~Stage() = default;

		// - DATA MEMBERS ------------------------------------

		public:
			const GraphicsScene::GeometryConcept*	source;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		GraphicsPipeline( const GraphicsPipeline& ) = delete;
	//!	Constructs this @ref RenderPipeline instance.
		GraphicsPipeline();

		~GraphicsPipeline();

	// ---------------------------------------------------

	public:
		VkRenderPass	GetRenderPass();

	// ---------------------------------------------------

	public:
		VkResult	BindResources( Gpu& gpu, const GraphicsScene& scene, const AssetViews::GraphicsPipelineAsset& asset );

		void		FreeResources( Gpu& gpu );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		GraphicsPipeline&	operator=( const GraphicsPipeline& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		ArrayList<Stage>	_stages;
		VkRenderPass		_renderPass;

	// ---------------------------------------------------

		friend void	Swap( GraphicsPipeline&, GraphicsPipeline& );
	};

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/GraphicsPipeline.inl>
//------------------------------------------------------------------//