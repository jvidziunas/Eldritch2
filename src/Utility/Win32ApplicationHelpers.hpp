/*==================================================================*\
  Win32ApplicationHelpers.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Compiler.hpp>
//------------------------------------------------------------------//

struct HINSTANCE__;
typedef HINSTANCE__*	HINSTANCE;

#if defined( GetCommandLine )
#	undef GetCommandLine
#endif

namespace Eldritch2 {
namespace Utility {

	namespace Utility = ::Eldritch2::Utility;

// ---------------------------------------------------

	void ETNoThrowHint	StoreMainArguments( ::HINSTANCE hInstance, int nCmdShow );

// ---------------------------------------------------

	bool ETNoThrowHint	CommandLineContains( char* option );
	bool ETNoThrowHint	CommandLineContains( wchar_t* option );

// ---------------------------------------------------

	::HINSTANCE ETNoThrowHint	GetInstance();

// ---------------------------------------------------
	
	const char*		GetCommandLine();

	const wchar_t*	GetWideCommandLine();

// ---------------------------------------------------

	int ETNoAliasHint ETNoThrowHint GetCmdShow();

}	// namespace Utility
}	// namespace Eldritch2