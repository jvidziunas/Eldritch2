/*==================================================================*\
  PlatformWindowSwapChain.ScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/ScriptComponents/PlatformWindowSwapChain.hpp>
#include <Scripting/ApiRegistrar.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {
namespace ScriptComponents {

	void PlatformWindowSwapChain::RegisterScriptApi( ApiRegistrar& registrar ) {
		registrar.DefineUserAllocatedClass<PlatformWindowSwapChain>(	{/* Factories */},
																		{/* Properties */ },
																		{/* Methods */},
																		{/* Operators */},
																		{/* Fields*/ } );
	}

}	// namespace ScriptComponents
}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2