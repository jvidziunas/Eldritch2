/*==================================================================*\
  DisplayLocator.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/DisplayLocator.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	ET_IMPLEMENT_WREN_CLASS(DisplayLocator, api, moduleName) {
		api.DefineClass<DisplayLocator>("DisplayLocator", moduleName, { /* Static methods */ }, { /* Methods */ });
	}

}}} // namespace Eldritch2::Graphics::Vulkan
