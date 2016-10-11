/*==================================================================*\
  Win32ApplicationHelpers.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Compiler.hpp>
//------------------------------------------------------------------//

struct HINSTANCE__;
using HINSTANCE	= HINSTANCE__*;

#if defined( GetCommandLine )
#	undef GetCommandLine
#endif

namespace Eldritch2 {
namespace Utility {

	void ETNoThrowHint	StoreMainArguments( HINSTANCE hInstance, int nCmdShow );

// ---------------------------------------------------

	bool ETNoThrowHint	CommandLineContains( wchar_t* option );
	bool ETNoThrowHint	CommandLineContains( char* option );

// ---------------------------------------------------

	HINSTANCE ETNoThrowHint	GetInstance( void* addressOfItemInModule = nullptr );

// ---------------------------------------------------
	
	const wchar_t*	GetWideCommandLine();

	const char*		GetCommandLine();

// ---------------------------------------------------

	int ETPureFunctionHint ETNoThrowHint GetCmdShow();

}	// namespace Utility
}	// namespace Eldritch2