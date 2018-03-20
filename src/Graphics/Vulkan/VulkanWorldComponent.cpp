/*==================================================================*\
  VulkanWorldComponent.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/VulkanWorldComponent.hpp>
#include <Graphics/Vulkan/VulkanGraphicsScene.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/Vulkan.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Assets;
	using namespace ::Eldritch2::Core;

	VulkanWorldComponent::VulkanWorldComponent( const World& owner ) : WorldComponent( owner.GetServices() ), _scene( nullptr ) {}

// ---------------------------------------------------

	void VulkanWorldComponent::AcceptVisitor( JobExecutor& executor, const LateInitializationVisitor ) {
		MICROPROFILE_SCOPEI( "World/LateInitialization", "Update Vulkan pipelines", 0x0C11F0 );

		if (_scene == nullptr || Failed( _scene->BindResources( executor, FindService<Vulkan>() ) )) {
			FindService<World>().SetShouldShutDown();
		}
	}

// ---------------------------------------------------

	void VulkanWorldComponent::AcceptVisitor( JobExecutor& executor, const VariableTickVisitor& ) {
		MICROPROFILE_SCOPEI( "World/VariableTick", "Draw Scene", 0x0C11F0 );

		_scene->SubmitGpuCommands( executor, FindService<Vulkan>() );
	}

// ---------------------------------------------------

	void VulkanWorldComponent::AcceptVisitor( JobExecutor& executor, const TearDownVisitor ) {
		if (_scene) {
			_scene->FreeResources( executor, FindService<Vulkan>() );
		}
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2