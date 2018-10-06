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
#include <Graphics/Vulkan/BatchCoordinator.hpp>
#include <Graphics/Vulkan/GraphicsPipeline.hpp>
#include <Graphics/Vulkan/GpuResourceApi.hpp>
#include <Graphics/GraphicsScene.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class PlayerView {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using DisplayLink = UniquePointer<Viewport, ViewportDisposer>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref PlayerView instance.
		PlayerView(DisplayLink target, Transformation worldToView, Angle verticalFov);
		//! Disable copy construction.
		PlayerView(const PlayerView&) = delete;

		~PlayerView() = default;

		// ---------------------------------------------------

	public:
		const Framebuffer& GetTarget() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		ETConstexpr Transformation ETSimdCall GetWorldToView() const ETNoexceptHint;

		ETConstexpr void ETSimdCall SetWorldToView(Transformation worldToView) ETNoexceptHint;

		ETConstexpr Angle GetVerticalFov() const ETNoexceptHint;

		ETConstexpr void SetVerticalFov(Angle angle) ETNoexceptHint;

		// ---------------------------------------------------

		//! Disable copy assignment.
		PlayerView& operator=(const PlayerView&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		DisplayLink    _target;
		Transformation _worldToView;
		Angle          _verticalFov;
	};

	// ---

	class VulkanGraphicsScene : public GraphicsScene {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref VulkanGraphicsScene instance.
		VulkanGraphicsScene();
		//! Disable copy construction.
		VulkanGraphicsScene(const VulkanGraphicsScene&) = delete;

		~VulkanGraphicsScene() = default;

		// ---------------------------------------------------

	public:
		Range<const PlayerView* const*> GetRootViews() const ETNoexceptHint;

		ETConstexpr const GraphicsPipeline& GetShadowPipeline() const ETNoexceptHint;

		ETConstexpr const GraphicsPipeline& GetLitPipeline() const ETNoexceptHint;

		ETConstexpr const Framebuffer& GetShadowAtlas() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, const GraphicsPipelineBuilder& litPipelineBuilder, const GraphicsPipelineBuilder& shadowPipelineBuilder, VkExtent2D shadowResolution, VkDeviceSize transformArenaSize);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

		//! Disable copy assignment.
		VulkanGraphicsScene& operator=(const VulkanGraphicsScene&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		const GraphicsPipelineBuilder* _shadowPipelineBuilder;
		const GraphicsPipelineBuilder* _litPipelineBuilder;
		ArrayList<PlayerView*>         _rootViews;
		UniformBuffer                  _uniforms;
		GraphicsPipeline               _shadowPipeline;
		Framebuffer                    _shadowAtlas;
		GraphicsPipeline               _litPipeline;
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/VulkanGraphicsScene.inl>
//------------------------------------------------------------------//
