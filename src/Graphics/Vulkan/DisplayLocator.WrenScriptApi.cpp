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

	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Scripting;

	ET_IMPLEMENT_WREN_CLASS(DisplayLocator) { // clang-format off
		api.CreateClass<DisplayLocator>(ET_BUILTIN_WREN_MODULE_NAME(Graphics), "DisplayLocator",
			{ /* Constructors */ },
			{ /* Static methods */ },
			{ /* Properties */ },
			{ /* Methods */ }
		);
	} // clang-format on

}}} // namespace Eldritch2::Graphics::Vulkan
