/*==================================================================*\
  DisplayLocator.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/DisplayLocator.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	ET_IMPLEMENT_WREN_CLASS(DisplayLocator) {
		api.DefineClass<DisplayLocator>(ET_BUILTIN_WREN_MODULE_NAME(Graphics), "DisplayLocator", { /* Static methods */ }, { /* Methods */ });
	}

}}} // namespace Eldritch2::Graphics::Vulkan
