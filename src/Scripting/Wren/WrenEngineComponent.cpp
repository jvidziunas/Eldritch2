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

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;
	using namespace ::Eldritch2::Core;

	WrenEngineComponent::WrenEngineComponent( const Blackboard& services ) : EngineComponent( services ) {}

// ---------------------------------------------------

	Result<UniquePointer<WorldComponent>> WrenEngineComponent::CreateWorldComponent( Allocator& allocator, const World& world ) {
		UniquePointer<WorldComponent> wrenComponent( CreateUnique<WrenWorldComponent>( allocator, world ) );
		if (!wrenComponent) {
			return Error::OutOfMemory;
		}

		return eastl::move( wrenComponent );
	}

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2