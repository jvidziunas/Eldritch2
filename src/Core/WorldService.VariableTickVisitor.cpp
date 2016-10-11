/*==================================================================*\
  WorldService.VariableTickVisitor.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/WorldService.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Core {

	WorldService::VariableTickVisitor::VariableTickVisitor( World& world, size_t arenaSizeInBytes ) : world( world ), frameAllocator( world.GetAllocator(), arenaSizeInBytes, AllocationDuration::Temporary, "World Variable Tick Temporary Allocator" ) {}

}	// namespace Core
}	// namespace Eldritch2