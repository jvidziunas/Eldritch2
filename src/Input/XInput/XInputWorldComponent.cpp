/*==================================================================*\
  XInputWorldComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Input/XInput/XInputWorldComponent.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Input { namespace XInput {

	using namespace ::Eldritch2::Core;

	XInputWorldComponent::XInputWorldComponent(const World& world) :
		WorldComponent(world.GetServices()) {}

}}} // namespace Eldritch2::Input::XInput
