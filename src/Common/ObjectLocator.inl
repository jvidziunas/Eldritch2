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

template <typename Service>
ETInlineHint ETForceInlineHint Service* ObjectLocator::Find() const ETNoexceptHint {
	return static_cast<Service*>(Find(typeid(Service)));
}

// ---------------------------------------------------

template <typename Service>
ETInlineHint ETForceInlineHint ObjectLocator& ObjectLocator::PublishService(Service& service) {
	return PublishService(typeid(Service), ETAddressOf(service));
}

} // namespace Eldritch2
