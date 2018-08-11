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
#include <Core/WorldComponent.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {
	class VulkanGraphicsScene;
	class DisplayLocator;
}}} // namespace Eldritch2::Graphics::Vulkan

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

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
		void BindResources(Scheduling::JobExecutor& executor) override;

		void FreeResources(Scheduling::JobExecutor& executor) override;

		// ---------------------------------------------------

	public:
		void OnVariableRateTick(Scheduling::JobExecutor& executor, MicrosecondTime tickDuration, float32 residualFraction) override;

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
	};

}}} // namespace Eldritch2::Graphics::Vulkan
