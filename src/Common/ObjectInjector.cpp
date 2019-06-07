/*==================================================================*\
  ObjectLocator.cpp
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

//------------------------------------------------------------------//

namespace Eldritch2 {

ObjectInjector::ObjectInjector() ETNoexceptHint : _serviceByType(MallocAllocator("Object Locator Service Allocator")) {}

// ---------------------------------------------------

ObjectInjector& ObjectInjector::PublishService(CppType type, void* service) {
	const auto result(_serviceByType.Emplace(type, service));
	ETAssert(result.second, "Duplicate service registration!");

	return *this;
}

// ---------------------------------------------------

void* ObjectInjector::Find(CppType serviceType) const ETNoexceptHint {
	const auto candidate(_serviceByType.Find(serviceType));
	ETAssert(candidate != _serviceByType.End(), "Unknown service!");

	return candidate->second;
}

} // namespace Eldritch2
