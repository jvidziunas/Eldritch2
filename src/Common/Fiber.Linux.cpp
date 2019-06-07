/*==================================================================*\
  Fiber.Linux.cpp
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
#include <Common/Fiber.hpp>
//------------------------------------------------------------------//
#include <ucontext.h>
//------------------------------------------------------------------//

namespace {

	struct Fiber {
		void*      userData;
		ucontext_t context;
	};

	ETThreadLocal Fiber  CurrentFiber;
}

namespace Eldritch2 {

	PlatformFiber CreatePlatformFiber(size_t stackReservation, void* parameter, void (ETStdCall* entryPoint)(void*) ETNoexceptHint) ETNoexceptHint {
		makecontext(CurrentFiber.context, entryPoint, sizeof(parameter) / sizeof(int), parameter);
		return nullptr;
	}

	// ---------------------------------------------------

	void DestroyPlatformFiber(PlatformFiber /*fiber*/) ETNoexceptHint {

	}

	// ---------------------------------------------------

	PlatformFiber GetActiveFiber() ETNoexceptHint {
		return CurrentFiber.context;
	}

	// ---------------------------------------------------

	void ActivateFiber(PlatformFiber fiber) ETNoexceptHint {
		swapcontext();
	}

	// ---------------------------------------------------

	void SetFiberUserData(void* userData) ETNoexceptHint {
		CurrentFiber->userData = userData;
	}

	// ---------------------------------------------------

	void* GetFiberUserData() ETNoexceptHint {
		return CurrentFiber->userData;
	}

}	// namespace Eldritch2
