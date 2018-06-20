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

	ETInlineHint EngineComponent::EngineComponent(const Blackboard& services) :
		_services(&services) {}

	// ---------------------------------------------------

	template <typename ServiceType>
	ETInlineHint ServiceType& EngineComponent::FindService() const {
		return _services->Find<ServiceType>();
	}

}} // namespace Eldritch2::Core
