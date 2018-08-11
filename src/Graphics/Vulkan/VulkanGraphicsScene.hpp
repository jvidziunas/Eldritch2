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
#include <Graphics/Vulkan/CommandList.hpp>
#include <Graphics/Vulkan/Display.hpp>
#include <Graphics/GraphicsScene.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {
	class FrameTransferBus;
}}} // namespace Eldritch2::Graphics::Vulkan

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class PlayerView : public DisplaySource {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref PlayerView instance.
		PlayerView(Angle verticalFov);
		//! Disable copy construction.
		PlayerView(const PlayerView&) = delete;

		~PlayerView() = default;

		// ---------------------------------------------------

	public:
		Transformation ETSimdCall GetWorldToView() const;

		void ETSimdCall SetWorldToView(Transformation worldToView);

		Angle GetVerticalFov() const;

		void SetVerticalFov(Angle angle);

		// ---------------------------------------------------

	public:
		VkResult UpdateResources(Gpu& gpu);

		void FreeResources(Gpu& gpu);

		// ---------------------------------------------------

		//! Disable copy assignment.
		PlayerView& operator=(const PlayerView&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		Transformation _worldToView;
		Angle          _verticalFov;
		Framebuffer    _framebuffer;
	};

	// ---

	class VulkanGraphicsScene : public GraphicsScene {
		// - TYPE PUBLISHING ---------------------------------

	public:
		enum : uint32 { MaxQueuedFrames = 2u };
		enum : VkDeviceSize {
			DefaultTransformArenaSize = 16u * 1024u * 1024u, /*  16MB */
		};

		// ---

	public:
		class Frame {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------
		public:
			//!	Disable copy construction.
			Frame(const Frame&) = delete;
			//!	Constructs this @ref Frame instance.
			Frame();

			~Frame() = default;

			// ---------------------------------------------------

		public:
			bool CheckCommandsConsumed(Gpu& gpu) const;

			// ---------------------------------------------------

		public:
			VkResult SubmitCommands(Gpu& gpu, const VulkanGraphicsScene& scene);

			// ---------------------------------------------------

		public:
			VkResult BindResources(Gpu& gpu);

			void FreeResources(Gpu& gpu);

			// - DATA MEMBERS ------------------------------------

		public:
			CommandList _commands;
			VkFence     _drawsConsumed;
			VkSemaphore _startExecution;
			VkSemaphore _finishExecution;

			// ---------------------------------------------------

			friend void Swap(Frame&, Frame&);
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Disable copy construction.
		VulkanGraphicsScene(const VulkanGraphicsScene&) = delete;
		//! Constructs this @ref VulkanGraphicsScene instance.
		VulkanGraphicsScene();

		~VulkanGraphicsScene() = default;

		// ---------------------------------------------------

	public:
		VkResult SubmitViewIndependentCommands(Scheduling::JobExecutor& executor, Gpu& gpu);

		VkResult SubmitViewDependentCommands(Scheduling::JobExecutor& executor, Gpu& gpu);

		// ---------------------------------------------------

	public:
		VkResult BindResources(Scheduling::JobExecutor& executor, Gpu& gpu, FrameTransferBus& bus, VkDeviceSize transformArenaSize = DefaultTransformArenaSize);

		void FreeResources(Scheduling::JobExecutor& executor, Gpu& gpu, FrameTransferBus& bus);

		// ---------------------------------------------------

		//! Disable copy assignment.
		VulkanGraphicsScene& operator=(const VulkanGraphicsScene&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		ArrayList<PlayerView*> _rootViews;
		VkSemaphore            _resourcesReady;
		UniformBuffer          _transforms;
		BatchCoordinator       _opaqueBatches;
		GraphicsPipeline       _shadowPipeline;
		Framebuffer            _shadowAtlas;
		GraphicsPipeline       _mainPipeline;
		DescriptorTable        _resourceDescriptors;
		uint32                 _frameId;
		Frame                  _queuedFrames[MaxQueuedFrames];
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/VulkanGraphicsScene.inl>
//------------------------------------------------------------------//
