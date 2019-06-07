/*==================================================================*\
  Fiber.Win32.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2019 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Windows.h>
//------------------------------------------------------------------//

namespace {

	static const DWORD FiberUserdataIndex = FlsAlloc(nullptr);

} // anonymous namespace

namespace Eldritch2 {

	PlatformFiber CreatePlatformFiber(size_t stackReservation, void* parameter, void (ETStdCall* entryPoint)(void*) ETNoexceptHint) ETNoexceptHint {
		return CreateFiberEx(/*dwStackCommitSize =*/0u, stackReservation, FIBER_FLAG_FLOAT_SWITCH, entryPoint, parameter);
	}

	// ---------------------------------------------------

	void DestroyPlatformFiber(PlatformFiber fiber) ETNoexceptHint {
		DeleteFiber(fiber);
	}

	// ---------------------------------------------------

	PlatformFiber GetActiveFiber() ETNoexceptHint {
		return GetCurrentFiber();
	}

	// ---------------------------------------------------

	void ActivateFiber(PlatformFiber fiber) ETNoexceptHint {
		SwitchToFiber(fiber);
	}

	// ---------------------------------------------------

	void SetFiberUserData(void* userData) ETNoexceptHint {
		FlsSetValue(FiberUserdataIndex, userData);
	}

	// ---------------------------------------------------

	void* GetFiberUserData() ETNoexceptHint {
		return FlsGetValue(FiberUserdataIndex);
	}

}	// namespace Eldritch2
