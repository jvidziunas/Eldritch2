/*==================================================================*\
  Win32Tools.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#ifndef NOMINMAX
#	define NOMINMAX
#endif
#include <Windows.h>
#include <shlwapi.h>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY("Shlwapi.lib")
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace {

	static int cmdShow = 0;

} // anonymous namespace

void StoreMainArguments(HINSTANCE /*hInstance*/, int nCmdShow) ETNoexceptHint {
	cmdShow = nCmdShow;
}

// ---------------------------------------------------

bool ETPureFunctionHint CommandLineContains(char* const option) ETNoexceptHint {
	return Find(GetCommandLineA(), option) != nullptr;
}

// ---------------------------------------------------

bool ETPureFunctionHint CommandLineContains(wchar_t* const option) ETNoexceptHint {
	return Find(GetCommandLineW(), option) != nullptr;
}

// ---------------------------------------------------

HINSTANCE ETPureFunctionHint GetModuleByAddress(void* addressOfItemInModule) ETNoexceptHint {
	if (!addressOfItemInModule) {
		addressOfItemInModule = LPVOID(ETAddressOf(GetModuleByAddress));
	}

	HINSTANCE instance(nullptr);
	GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, LPWSTR(addressOfItemInModule), ETAddressOf(instance));
	return instance;
}

// ---------------------------------------------------

int ETPureFunctionHint GetCmdShow() ETNoexceptHint {
	return cmdShow;
}

} // namespace Eldritch2
