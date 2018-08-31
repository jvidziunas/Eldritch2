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

void ETNeverThrowsHint StoreMainArguments(HINSTANCE hInstance, int nCmdShow);

int ETPureFunctionHint ETNeverThrowsHint GetCmdShow();

bool ETNeverThrowsHint CommandLineContains(wchar_t* option);
bool ETNeverThrowsHint CommandLineContains(char* option);

HINSTANCE ETNeverThrowsHint GetModuleByAddress(void* addressOfItemInModule = nullptr);

} // namespace Eldritch2
