/*==================================================================*\
  Win32Tools.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/Compiler.hpp>
//------------------------------------------------------------------//

using HINSTANCE = struct HINSTANCE__*;

namespace Eldritch2 {

void StoreMainArguments(HINSTANCE hInstance, int nCmdShow) ETNoexceptHint;

int ETPureFunctionHint GetCmdShow() ETNoexceptHint;

bool ETPureFunctionHint CommandLineContains(wchar_t* option) ETNoexceptHint;
bool ETPureFunctionHint CommandLineContains(char* option) ETNoexceptHint;

HINSTANCE ETPureFunctionHint GetModuleByAddress(void* addressOfItemInModule = nullptr) ETNoexceptHint;

} // namespace Eldritch2
