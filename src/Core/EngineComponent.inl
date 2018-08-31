/*==================================================================*\
  EngineComponent.inl
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

namespace Eldritch2 { namespace Core {

	ETInlineHint ETForceInlineHint EngineComponent::EngineComponent(const ObjectLocator& services) :
		_services(ETAddressOf(services)) {}

	// ---------------------------------------------------

	template <typename ServiceType>
	ETInlineHint ETForceInlineHint ServiceType* EngineComponent::FindService() const {
		return _services->Find<ServiceType>();
	}

}} // namespace Eldritch2::Core
