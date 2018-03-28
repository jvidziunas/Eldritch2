/*==================================================================*\
  OutputCoordinator.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/OutputCoordinator.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Scripting;

	ET_IMPLEMENT_WREN_CLASS( OutputCoordinator ) {
		api.CreateClass<OutputCoordinator>( ET_BUILTIN_WREN_MODULE_NAME( Graphics ), "OutputCoordinator",
			{/* Constructors */},
			{/*	Properties */},
			{/*	Methods */},
			{/*	Operators */}
		);
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2