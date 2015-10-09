/*==================================================================*\
  Win32ApplicationHelpers.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Win32ApplicationHelpers.hpp>
#include <Utility/Memory/MemStdLib.hpp>
#include <wchar.h>
#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#	define NOMINMAX
#endif
#include <Windows.h>
#include <shlwapi.h>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "Shlwapi.lib" )
//------------------------------------------------------------------//

using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

#if defined( GetCommandLine )
#	undef GetCommandLine
#endif

namespace {

	static ::HINSTANCE	instance = nullptr;
	static int			cmdShow = 0;

}	// anonymous namespace

namespace Eldritch2 {
namespace Utility {

	void StoreMainArguments( ::HINSTANCE hInstance, int nCmdShow ) {
		instance	= hInstance;
		cmdShow		= nCmdShow;
	}

// ---------------------------------------------------

	bool ETNoThrowHint CommandLineContains( char* const option ) {
		return nullptr != FindFirstInstance( GetCommandLine(), option );
	}

// ---------------------------------------------------

	bool ETNoThrowHint CommandLineContains( wchar_t* const option ) {
		return nullptr != FindFirstInstance( GetWideCommandLine(), option );
	}

// ---------------------------------------------------

	::HINSTANCE GetInstance() {
		return instance;
	}

// ---------------------------------------------------
	
	const char* GetCommandLine() {
		return ::GetCommandLineA();
	}

// ---------------------------------------------------

	const wchar_t* GetWideCommandLine() {
		return ::GetCommandLineW();
	}

// ---------------------------------------------------

	int ETNoAliasHint ETNoThrowHint GetCmdShow() {
		return cmdShow;
	}

}	// namespace Utility
}	// namespace Eldritch2