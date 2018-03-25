/*==================================================================*\
  VulkanWorldComponent.WrenScriptApi.cpp
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
#include <Graphics/Vulkan/OutputCoordinator.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Graphics/Vulkan/Vulkan.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Animation {
		ET_DECLARE_WREN_CLASS( Armature );
	}

	namespace Graphics {
		namespace Vulkan {
			ET_DECLARE_WREN_CLASS( VulkanGraphicsScene );
			ET_DECLARE_WREN_CLASS( OutputCoordinator );
			ET_DECLARE_WREN_CLASS( StaticMesh );
			ET_DECLARE_WREN_CLASS( PlayerView );
			ET_DECLARE_WREN_CLASS( Light );
			ET_DECLARE_WREN_CLASS( Mesh );
		}

		ET_DECLARE_WREN_CLASS( RgbColor );
	}
}

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Animation;
	using namespace ::Eldritch2::Core;

	void VulkanWorldComponent::AcceptVisitor( ApiBuilder& api ) {
		ET_REGISTER_WREN_CLASS( VulkanGraphicsScene, api );
		ET_REGISTER_WREN_CLASS( OutputCoordinator, api );
		ET_REGISTER_WREN_CLASS( StaticMesh, api );
		ET_REGISTER_WREN_CLASS( PlayerView, api );
		ET_REGISTER_WREN_CLASS( RgbColor, api );
		ET_REGISTER_WREN_CLASS( Armature, api );
		ET_REGISTER_WREN_CLASS( Light, api );
		ET_REGISTER_WREN_CLASS( Mesh, api );

		_scene = api.CreateVariable<VulkanGraphicsScene>( ET_BUILTIN_WREN_MODULE_NAME( Graphics ), "GraphicsScene" );
		api.CreateVariable<OutputCoordinator>( ET_BUILTIN_WREN_MODULE_NAME( Graphics ), "OutputCoordinator", FindService<Vulkan>().GetPrimaryDevice() );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2