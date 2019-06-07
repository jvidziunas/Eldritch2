/*==================================================================*\
  Fiber.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2019 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

	using PlatformFiber = void*;

	// ---

	PlatformFiber CreatePlatformFiber(size_t stackReservation, void* parameter, void (ETStdCall* entryPoint)(void*) ETNoexceptHint) ETNoexceptHint;
	void          DestroyPlatformFiber(PlatformFiber fiber) ETNoexceptHint;

	PlatformFiber GetActiveFiber() ETNoexceptHint;
	void          ActivateFiber(PlatformFiber fiber) ETNoexceptHint;

	void SetFiberUserData(void* userData) ETNoexceptHint;
	void* GetFiberUserData() ETNoexceptHint;

}	// namespace Eldritch2
