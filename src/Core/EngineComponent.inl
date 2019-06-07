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

	ETInlineHint ETForceInlineHint EngineComponent::EngineComponent(const ObjectInjector& services) ETNoexceptHint : _services(ETAddressOf(services)) {}

	// ---------------------------------------------------

	template <typename Service>
	ETInlineHint ETForceInlineHint Service* EngineComponent::Inject() const ETNoexceptHint {
		return _services->Find<Service>();
	}

}} // namespace Eldritch2::Core
