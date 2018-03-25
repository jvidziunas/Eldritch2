/*==================================================================*\
  RenderPipeline.hpp
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
				class	RenderPipelineAsset;
			}

			class	Gpu;
		}
	}
}

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	class RenderPipeline {
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
		RenderPipeline( const RenderPipeline& ) = delete;
	//!	Constructs this @ref RenderPipeline instance.
		RenderPipeline();

		~RenderPipeline();

	// ---------------------------------------------------

	public:
		VkRenderPass	GetRenderPass();

	// ---------------------------------------------------

	public:
		VkResult	BindResources( Gpu& gpu, const GraphicsScene& scene, const AssetViews::RenderPipelineAsset& asset );

		void		FreeResources( Gpu& gpu );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		RenderPipeline&	operator=( const RenderPipeline& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		ArrayList<Stage>	_stages;
		VkRenderPass		_renderPass;

	// ---------------------------------------------------

		friend void	Swap( RenderPipeline&, RenderPipeline& );
	};

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/RenderPipeline.inl>
//------------------------------------------------------------------//