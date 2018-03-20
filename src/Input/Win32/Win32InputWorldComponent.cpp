/*==================================================================*\
  Win32InputWorldComponent.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Input/Win32/Win32InputWorldComponent.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Input {
namespace Win32 {

	using namespace ::Eldritch2::Core;

	Win32InputWorldComponent::Win32InputWorldComponent( const World& owner ) : WorldComponent( owner.GetServices() ) {}

}	// namespace Win32
}	// namespace Input
}	// namespace Eldritch2