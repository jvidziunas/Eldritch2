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
		static ETInlineHint void CopyComponents(EngineComponent** out, Service& service, AdditionalComponents&... additionalComponents) {
			*out = eastl::addressof(service);
			CopyComponents(out + 1, additionalComponents...);
		}

		// ---

		static ETInlineHint void CopyComponents(EngineComponent** /*out*/) {}

	} // namespace Detail

	ETInlineHint const ObjectLocator& Engine::GetServiceLocator() const {
		return _services;
	}

	// ---------------------------------------------------

	ETInlineHint Logging::Log& Engine::GetLog() const {
		return _log;
	}

	// ---------------------------------------------------

	ETInlineHint bool Engine::ShouldRun() const {
		return _shouldRun.load(std::memory_order_consume);
	}

	// ---------------------------------------------------

	ETInlineHint void Engine::SetShouldShutDown() const {
		_shouldRun.store(false, std::memory_order_release);
	}

	// ---------------------------------------------------

	template <class... Components>
	ETInlineHint int Engine::BootOnCaller(Scheduling::JobExecutor& executor, Components&... clientComponents) {
		ArrayList<EngineComponent*> components(sizeof...(clientComponents), _components.GetAllocator());

		Detail::CopyComponents(components.Begin(), clientComponents...);
		Swap(_components, components);
		return BootOnCaller(executor);
	}

}} // namespace Eldritch2::Core
