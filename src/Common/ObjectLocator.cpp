/*==================================================================*\
  ObjectLocator.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/ObjectLocator.hpp>
#include <Common/Assert.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

ObjectLocator::ObjectLocator() :
	_servicesByType(MallocAllocator("Object Locator Service Allocator")) {}

// ---------------------------------------------------

ObjectLocator& ObjectLocator::PublishService(Type type, void* service) {
	const auto result(_servicesByType.Emplace(type, service));
	ET_ASSERT(result.second, "Duplicate service registration!");

	return *this;
}

// ---------------------------------------------------

void* ObjectLocator::Find(Type serviceType) const {
	const auto candidate(_servicesByType.Find(serviceType));
	ET_ASSERT(candidate != _servicesByType.End(), "Unknown service!");

	return candidate->second;
}

} // namespace Eldritch2
