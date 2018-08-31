/*==================================================================*\
  WrenEngineComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/WrenEngineComponent.hpp>
#include <Scripting/Wren/WrenWorldComponent.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {

	using namespace ::Eldritch2::Core;

	WrenEngineComponent::WrenEngineComponent(const ObjectLocator& services) :
		EngineComponent(services) {}

	// ---------------------------------------------------

	UniquePointer<WorldComponent> WrenEngineComponent::CreateWorldComponent(Allocator& allocator, const ObjectLocator& services) {
		return MakeUnique<WrenWorldComponent>(allocator, services);
	}

}}} // namespace Eldritch2::Scripting::Wren
