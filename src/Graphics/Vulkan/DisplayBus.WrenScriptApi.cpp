/*==================================================================*\
  DisplayBus.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/DisplayBus.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Scripting;

	ET_IMPLEMENT_WREN_CLASS(DisplayBus) {
		api.CreateClass<DisplayBus>(ET_BUILTIN_WREN_MODULE_NAME(Graphics), "DisplayBusClass", { /* Constructors */ }, { /* Static methods */ }, { /*	Properties */ }, { /*	Methods */ });
	}

}}} // namespace Eldritch2::Graphics::Vulkan
