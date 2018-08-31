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
#include <Graphics/Vulkan/DescriptorTable.hpp>
#include <Graphics/Vulkan/GpuResources.hpp>
#include <Graphics/GraphicsScene.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class PlayerView {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using ViewportReference = UniquePointer<const Viewport, ViewportDisposer>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref PlayerView instance.
		PlayerView(ViewportReference target, Transformation worldToView, Angle verticalFov);
		//! Disable copy construction.
		PlayerView(const PlayerView&) = delete;

		~PlayerView() = default;

		// ---------------------------------------------------

	public:
		const Framebuffer& GetTarget() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		Transformation ETSimdCall GetWorldToView() const ETNoexceptHint;

		void ETSimdCall SetWorldToView(Transformation worldToView) ETNoexceptHint;

		Angle GetVerticalFov() const ETNoexceptHint;

		void SetVerticalFov(Angle angle) ETNoexceptHint;

		// ---------------------------------------------------

		//! Disable copy assignment.
		PlayerView& operator=(const PlayerView&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		ViewportReference _target;
		Transformation    _worldToView;
		Angle             _verticalFov;
	};

	// ---

	class VulkanGraphicsScene : public GraphicsScene {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Disable copy construction.
		VulkanGraphicsScene(const VulkanGraphicsScene&) = delete;
		//! Constructs this @ref VulkanGraphicsScene instance.
		VulkanGraphicsScene();

		~VulkanGraphicsScene() = default;

		// ---------------------------------------------------

	public:
		const ArrayList<PlayerView*> GetRootViews() const ETNoexceptHint;

		const BatchCoordinator& GetOpaqueBatches() const ETNoexceptHint;

		const DescriptorTable& GetShaderResources() const ETNoexceptHint;

		const GraphicsPipeline& GetOpaqueLitPipeline() const ETNoexceptHint;

		const GraphicsPipeline& GetShadowPipeline() const ETNoexceptHint;

		const Framebuffer& GetShadowAtlas() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu, VkExtent2D shadowResolution, VkDeviceSize transformArenaSize);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

		//! Disable copy assignment.
		VulkanGraphicsScene& operator=(const VulkanGraphicsScene&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		ArrayList<PlayerView*> _rootViews;
		UniformBuffer          _transforms;
		BatchCoordinator       _opaqueBatches;
		GraphicsPipeline       _shadowPipeline;
		Framebuffer            _shadowAtlas;
		GraphicsPipeline       _opaqueLitPipeline;
		DescriptorTable        _shaderResources;
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/VulkanGraphicsScene.inl>
//------------------------------------------------------------------//
