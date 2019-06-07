/*==================================================================*\
  VulkanGraphicsScene.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/VulkanGraphicsScene.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Scripting::Wren;

	// ---------------------------------------------------

	ET_IMPLEMENT_WREN_CLASS(VulkanGraphicsScene, api, moduleName) {
		api.DefineClass<VulkanGraphicsScene>("GraphicsSceneClass", moduleName, { /* Static methods */ }, { /* Methods */ });
	}

}}} // namespace Eldritch2::Graphics::Vulkan
