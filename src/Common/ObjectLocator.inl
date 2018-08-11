/*==================================================================*\
  ObjectLocator.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <eastl/memory.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Service>
ETInlineHint Service& ObjectLocator::Find() const {
	return *static_cast<Service*>(Find(typeid(Service)));
}

// ---------------------------------------------------

template <typename Service>
ETInlineHint ObjectLocator& ObjectLocator::PublishService(Service& service) {
	return PublishService(typeid(Service), eastl::addressof(service));
}

// ---------------------------------------------------

ETInlineHint ObjectLocator& ObjectLocator::operator=(const ObjectLocator& other) {
	_servicesByType = other._servicesByType;
	return *this;
}

// ---------------------------------------------------

ETInlineHint ObjectLocator& ObjectLocator::operator=(ObjectLocator&& other) {
	_servicesByType = eastl::move(other._servicesByType);
	return *this;
}

} // namespace Eldritch2
