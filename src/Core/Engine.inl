/*==================================================================*\
  Engine.inl
  ------------------------------------------------------------------
  Purpose:
  Implements an 'engine' object that binds together most subsystems
  into a coherent whole. Also serves as an intermediate layer
  between higher-level OS-dependent application systems and
  low-level generic simulation engines.

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Core {
	namespace Detail {

		template <class Service, class... AdditionalComponents>
		static ETCpp14Constexpr ETInlineHint ETForceInlineHint void CopyComponents(EngineComponent** out, Service& service, AdditionalComponents&... additionalComponents) {
			*out = ETAddressOf(service), CopyComponents(out + 1, additionalComponents...);
		}

		// ---

		static ETConstexpr ETInlineHint ETForceInlineHint void CopyComponents(EngineComponent** /*out*/) {}

	} // namespace Detail

	ETInlineHint ETForceInlineHint const ObjectLocator& Engine::GetServiceLocator() const {
		return _services;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint Logging::Log& Engine::GetLog() const {
		return _log;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool Engine::ShouldRun(MemoryOrder order) const {
		return _shouldRun.load(order);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void Engine::SetShouldShutDown(MemoryOrder order) const {
		_shouldRun.store(false, order);
	}

	// ---------------------------------------------------

	template <class... Components>
	ETInlineHint ETForceInlineHint ErrorCode Engine::BootOnCaller(Scheduling::JobExecutor& executor, Components&... clientComponents) {
		ArrayList<EngineComponent*> components(_components.GetAllocator(), sizeof...(clientComponents));

		Detail::CopyComponents(components.Begin(), clientComponents...);
		Swap(_components, components);
		return BootOnCaller(executor);
	}

}} // namespace Eldritch2::Core
