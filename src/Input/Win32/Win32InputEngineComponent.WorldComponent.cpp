/*==================================================================*\
  Win32InputEngineComponent.WorldComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Input/Win32/Win32InputEngineComponent.hpp>
#include <Input/Win32/DeviceCoordinator.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Input { namespace Win32 {

	using namespace ::Eldritch2::Scheduling;

	// ---------------------------------------------------

	Win32InputEngineComponent::WorldComponent::WorldComponent(const ObjectInjector& services) ETNoexceptHint : Core::WorldComponent(services), _keyboards(nullptr), _mice(nullptr) {}

	// ---------------------------------------------------

	void Win32InputEngineComponent::WorldComponent::BindResources(JobExecutor& /*executor*/) ETNoexceptHint {
		const auto devices(Inject<DeviceCoordinator>());

		ET_TERMINATE_WORLD_IF(_keyboards == nullptr || _mice == nullptr);
		ET_TERMINATE_WORLD_IF_FAILED(_keyboards->BindResources(devices->GetMutex(), devices->GetKeyboards()));
		ET_TERMINATE_WORLD_IF_FAILED(_mice->BindResources(devices->GetMutex(), devices->GetMice()));
	}

	// ---------------------------------------------------

	void Win32InputEngineComponent::WorldComponent::FreeResources(JobExecutor& /*executor*/) ETNoexceptHint {
		if (DeviceLocator* const mice = Exchange(_mice, nullptr)) {
			mice->FreeResources();
		}

		if (DeviceLocator* const keyboards = Exchange(_keyboards, nullptr)) {
			keyboards->FreeResources();
		}
	}

}}} // namespace Eldritch2::Input::Win32
