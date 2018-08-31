/*==================================================================*\
  VulkanWorldComponent.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/CommandList.hpp>
#include <Core/WorldComponent.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {
	class VulkanGraphicsScene;
	class DisplayLocator;
}}} // namespace Eldritch2::Graphics::Vulkan

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

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
		bool IsConsumed(Gpu& gpu) const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		VkResult RecordCommands(Gpu& gpu, const VulkanGraphicsScene& scene);

		VkResult SubmitCommands(Gpu& gpu);

		// ---------------------------------------------------

	public:
		VkResult BindResources(Gpu& gpu);

		void FreeResources(Gpu& gpu);

		// - DATA MEMBERS ------------------------------------

	public:
		VkFence                  _drawsConsumed;
		SoArrayList<VkSemaphore> _waits;
		ArrayList<VkSemaphore>   _signals;
		CommandList              _drawCommands;

		// ---------------------------------------------------

		friend void Swap(Frame&, Frame&);
	};

	// ---

	class VulkanWorldComponent : public Core::WorldComponent {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		VulkanWorldComponent(const VulkanWorldComponent&) = delete;
		//!	Constructs this @ref VulkanWorldComponent instance.
		VulkanWorldComponent(const ObjectLocator& services);

		~VulkanWorldComponent() = default;

		// ---------------------------------------------------

	public:
		void OnVariableRateTick(Scheduling::JobExecutor& executor, MicrosecondTime tickDuration, float32 residualFraction) override;

		// ---------------------------------------------------

	public:
		void BindResources(Scheduling::JobExecutor& executor) override;

		void FreeResources(Scheduling::JobExecutor& executor) override;

		// ---------------------------------------------------

	public:
		void DefineScriptApi(Scripting::Wren::ApiBuilder& api) override;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		VulkanWorldComponent& operator=(const VulkanWorldComponent&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		VulkanGraphicsScene* _scene;
		DisplayLocator*      _displays;
		Frame                _frames[2];
	};

}}} // namespace Eldritch2::Graphics::Vulkan
