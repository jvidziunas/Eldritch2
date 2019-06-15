/*==================================================================*\
  VulkanGraphicsScene.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  �2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GraphicsPipeline.hpp>
#include <Graphics/GraphicsScene.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class VulkanGraphicsScene : public GraphicsScene<ViewRecord, LightRecord, MeshRecord> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Disable copy construction.
		VulkanGraphicsScene(const VulkanGraphicsScene&) = delete;
		//! Constructs this @ref VulkanGraphicsScene instance.
		VulkanGraphicsScene() ETNoexceptHint;

		~VulkanGraphicsScene() = default;

		// ---------------------------------------------------

	public:
		ETConstexpr const Framebuffer& GetShadowAtlas() const ETNoexceptHint;
		ETConstexpr Framebuffer& GetShadowAtlas() ETNoexceptHint;

		ETConstexpr const PipelineBuilder& GetShadowPipelineBuilder() const ETNoexceptHint;

		ETConstexpr const PipelineBuilder& GetLitPipelineBuilder() const ETNoexceptHint;

		ETConstexpr const GraphicsPipeline& GetShadowMapPipeline() const ETNoexceptHint;
		ETConstexpr GraphicsPipeline& GetShadowMapPipeline() ETNoexceptHint;

		ETConstexpr const GraphicsPipeline& GetLitPipeline() const ETNoexceptHint;
		ETConstexpr GraphicsPipeline& GetLitPipeline() ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, const PipelineBuilder& litPipeline, const PipelineBuilder& shadowPipeline, VkExtent2D shadowResolution);

		void FreeResources(Gpu& gpu) ETNoexceptHint;

		// ---------------------------------------------------

		//! Disable copy assignment.
		VulkanGraphicsScene& operator=(const VulkanGraphicsScene&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		PipelineBuilder  _shadowMapPipelineBuilder;
		GraphicsPipeline _shadowMapPipeline;
		Framebuffer      _shadowMapAtlas;
		PipelineBuilder  _litPipelineBuilder;
		GraphicsPipeline _litPipeline;
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/VulkanGraphicsScene.inl>
//------------------------------------------------------------------//
