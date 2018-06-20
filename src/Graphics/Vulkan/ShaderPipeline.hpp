/*==================================================================*\
  ShaderPipeline.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <vulkan/vulkan_core.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {
	namespace AssetViews {
		class SpirVShaderSetAsset;
	}

	class GraphicsPipeline;
	class Gpu;
}}} // namespace Eldritch2::Graphics::Vulkan

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class ShaderPipeline {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		ShaderPipeline(const ShaderPipeline&) = delete;
		//!	Constructs this @ref ShaderPipeline instance.
		ShaderPipeline(ShaderPipeline&&);
		//!	Constructs this @ref ShaderPipeline instance.
		ShaderPipeline();

		~ShaderPipeline();

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, GraphicsPipeline& context, const AssetViews::SpirVShaderSetAsset& asset);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		ShaderPipeline& operator=(const ShaderPipeline&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		VkPipelineLayout      _layout;
		ArrayList<VkPipeline> _pipelinesByPass;

		// ---------------------------------------------------

		friend void Swap(ShaderPipeline&, ShaderPipeline&);
	};

}}} // namespace Eldritch2::Graphics::Vulkan
