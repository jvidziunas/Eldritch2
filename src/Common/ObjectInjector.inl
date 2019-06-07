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

//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename... Services>
ETInlineHint ETForceInlineHint ObjectInjector::ObjectInjector(Services&... services) : ObjectInjector() {
	DiscardArguments((this->PublishService<Services>(services), 0)...);
}

// ---------------------------------------------------

template <typename Service>
ETInlineHint ETForceInlineHint Service* ObjectInjector::Find() const ETNoexceptHint {
	return static_cast<Service*>(Find(GetType<Service>()));
}

// ---------------------------------------------------

template <typename Service>
ETInlineHint ETForceInlineHint ObjectInjector& ObjectInjector::PublishService(Service& service) {
	return PublishService(GetType<Service>(), ETAddressOf(service));
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint void ObjectInjector::Clear() ETNoexceptHint {
	_serviceByType.Clear();
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint void Swap(ObjectInjector& lhs, ObjectInjector& rhs) ETNoexceptHint {
	Swap(lhs._serviceByType, rhs._serviceByType);
}

} // namespace Eldritch2
