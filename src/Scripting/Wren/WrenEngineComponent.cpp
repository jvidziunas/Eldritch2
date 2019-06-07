/*==================================================================*\
  WrenEngineComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/WrenEngineComponent.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {

	using namespace ::Eldritch2::Core;

	// ---------------------------------------------------

	WrenEngineComponent::WrenEngineComponent(const ObjectInjector& services) ETNoexceptHint : EngineComponent(services) {}

}}} // namespace Eldritch2::Scripting::Wren
