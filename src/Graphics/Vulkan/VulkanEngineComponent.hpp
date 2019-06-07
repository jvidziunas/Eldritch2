/*==================================================================*\
  VulkanEngineComponent.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/Vulkan.hpp>
#include <Core/EngineComponent.hpp>
#include <Core/WorldComponent.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {
	class VulkanGraphicsScene;
	class DisplayLocator;
}}} // namespace Eldritch2::Graphics::Vulkan

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class VulkanEngineComponent : public Core::EngineComponent {
		// - TYPE PUBLISHING ---------------------------------

	public:
		class FrameBarrier {
			// - TYPE PUBLISHING ---------------------------------

		public:
			using WaitList = SoaList<VkSemaphore, VkPipelineStageFlags>;

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Disable copy construction.
			FrameBarrier(const FrameBarrier&) = delete;
			//!	Constructs this @ref FrameBarrier instance.
			FrameBarrier() ETNoexceptHint;

			~FrameBarrier() = default;

			// ---------------------------------------------------

		public:
			bool IsConsumed(Gpu& gpu) const ETNoexceptHint;

			// ---------------------------------------------------

		public:
			VkResult BindResources(Gpu& gpu);

			void FreeResources(Gpu& gpu) ETNoexceptHint;

			// ---------------------------------------------------

			//!	Disable copy assignment.
			FrameBarrier& operator=(const FrameBarrier&) = delete;

			// - DATA MEMBERS ------------------------------------

		public:
			VkFence  _consumed;
			WaitList _waits;

			// ---------------------------------------------------

			friend void Swap(FrameBarrier&, FrameBarrier&) ETNoexceptHint;
		};

		// ---

	public:
		class WorldComponent : public Core::WorldComponent {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref VulkanWorldComponent instance.
			WorldComponent(const ObjectInjector& services) ETNoexceptHint;
			//!	Disable copy construction.
			WorldComponent(const WorldComponent&) = delete;

			~WorldComponent() = default;

			// ---------------------------------------------------

		public:
			void OnVariableRateTick(Scheduling::JobExecutor& executor, MicrosecondTime duration, float32 residualFraction) ETNoexceptHint override;

			void OnFixedRateTickLate(Scheduling::JobExecutor& executor, MicrosecondTime duration) ETNoexceptHint override;

			// ---------------------------------------------------

		public:
			void BindResources(Scheduling::JobExecutor& executor) ETNoexceptHint override;

			void FreeResources(Scheduling::JobExecutor& executor) ETNoexceptHint override;

			// ---------------------------------------------------

		public:
			void PublishApi(Scripting::Wren::ApiBuilder&) override;

			using Core::WorldComponent::PublishApi;

			// ---------------------------------------------------

			//!	Disable copy assignment.
			WorldComponent& operator=(const WorldComponent&) = delete;

			// - DATA MEMBERS ------------------------------------

		private:
			DisplayLocator*      _displays;
			VulkanGraphicsScene* _scene;
			FrameBarrier         _frames[2];
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref VulkanEngineComponent instance.
		VulkanEngineComponent(const ObjectInjector& services) ETNoexceptHint;
		//!	Disable copy construction.
		VulkanEngineComponent(const VulkanEngineComponent&) = delete;
		//!	Constructs this @ref VulkanEngineComponent instance.
		VulkanEngineComponent(VulkanEngineComponent&&) ETNoexceptHint = default;

		~VulkanEngineComponent() = default;

		// - ENGINE SERVICE SANDBOX METHODS ------------------

	public:
		void PublishApi(Core::PropertyApiBuilder& api) override;

		void PublishApi(Core::AssetApiBuilder& api) override;

		void PublishApi(ObjectInjector& api) override;

		void BindConfigurableResources(Scheduling::JobExecutor& executor) ETNoexceptHint override;

		// - ENGINE SERVICE SANDBOX METHODS ------------------

	public:
		void TickEarly(Scheduling::JobExecutor& executor) ETNoexceptHint override;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		VulkanEngineComponent& operator=(const VulkanEngineComponent&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		String          _preferredGpu;
		HashSet<String> _layers;
		HashSet<String> _deviceLayers;
		Vulkan          _vulkan;
	};

}}} // namespace Eldritch2::Graphics::Vulkan
