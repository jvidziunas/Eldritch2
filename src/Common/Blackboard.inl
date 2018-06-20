/*==================================================================*\
  Blackboard.inl
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
ETInlineHint Service& Blackboard::Find() const {
	return *static_cast<Service*>(Find(typeid(Service)));
}

// ---------------------------------------------------

template <typename Service>
ETInlineHint Blackboard& Blackboard::Publish(Service& service) {
	return Publish(typeid(Service), eastl::addressof(service));
}

// ---------------------------------------------------

ETInlineHint Blackboard& Blackboard::operator=(const Blackboard& other) {
	_servicesByType = other._servicesByType;
	return *this;
}

// ---------------------------------------------------

ETInlineHint Blackboard& Blackboard::operator=(Blackboard&& other) {
	_servicesByType = eastl::move(other._servicesByType);
	return *this;
}

} // namespace Eldritch2
