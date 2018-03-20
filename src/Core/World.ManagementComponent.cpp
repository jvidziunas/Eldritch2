/*==================================================================*\
  World.ManagementComponent.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/World.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Core {

	World::ManagementComponent::ManagementComponent(
		const World& owner
	) : WorldComponent( owner.GetServices() ) {}

}	// namespace Core
}	// namespace Eldritch2