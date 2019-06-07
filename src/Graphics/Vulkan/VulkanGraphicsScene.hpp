/*==================================================================*\
  VulkanGraphicsScene.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GraphicsPipeline.hpp>
#include <Graphics/GraphicsScene.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	enum class ViewportId : Eldritch2::uintptr;

	// ---

	class RootView {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref RootView instance.
		RootView(ViewportId target, Transformation worldToView, Angle verticalFov) ETNoexceptHint;
		//! Disable copy construction.
		RootView(const RootView&) = delete;

		~RootView() = default;

		// ---------------------------------------------------

	public:
		ETConstexpr Transformation ETSimdCall GetWorldToView() const ETNoexceptHint;

		ETConstexpr void ETSimdCall SetWorldToView(Transformation worldToView) ETNoexceptHint;

		ETConstexpr Angle GetVerticalFov() const ETNoexceptHint;

		ETConstexpr void SetVerticalFov(Angle angle) ETNoexceptHint;

		// ---------------------------------------------------

		//! Disable copy assignment.
		RootView& operator=(const RootView&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		ViewportId     _target;
		Transformation _worldToView;
		Angle          _verticalFov;
	};

	// ---

	class VulkanGraphicsScene : public GraphicsScene {
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
		PipelineBuilder _shadowMapPipelineBuilder;
		GraphicsPipeline        _shadowMapPipeline;
		Framebuffer             _shadowMapAtlas;
		PipelineBuilder _litPipelineBuilder;
		GraphicsPipeline        _litPipeline;
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/VulkanGraphicsScene.inl>
//------------------------------------------------------------------//
