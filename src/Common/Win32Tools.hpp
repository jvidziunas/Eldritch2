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

#if defined GetCommandLine
#	undef GetCommandLine
#endif

namespace Eldritch2 {

void ETNeverThrowsHint StoreMainArguments(HINSTANCE hInstance, int nCmdShow);

// ---------------------------------------------------

const wchar_t* GetWideCommandLine();

const char* GetCommandLine();

bool ETNeverThrowsHint CommandLineContains(wchar_t* option);
bool ETNeverThrowsHint CommandLineContains(char* option);

// ---------------------------------------------------

HINSTANCE ETNeverThrowsHint GetModuleByAddress(void* addressOfItemInModule = nullptr);

// ---------------------------------------------------

int ETPureFunctionHint ETNeverThrowsHint GetCmdShow();

} // namespace Eldritch2
