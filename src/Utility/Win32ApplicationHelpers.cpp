/*==================================================================*\
  Win32ApplicationHelpers.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Win32ApplicationHelpers.hpp>
#include <Utility/Memory/StandardLibrary.hpp>
//------------------------------------------------------------------//
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

#if defined( GetCommandLine )
#	undef GetCommandLine
#endif

namespace Eldritch2 {
namespace Utility {
namespace {

	static int	cmdShow = 0;

}	// anonymous namespace

	void StoreMainArguments( HINSTANCE /*hInstance*/, int nCmdShow ) {
		cmdShow = nCmdShow;
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

	HINSTANCE GetInstance( void* addressOfItemInModule ) {
		HINSTANCE	instance( nullptr );

		if( !addressOfItemInModule ) {
			addressOfItemInModule = reinterpret_cast<void*>( &GetInstance );
		}

		GetModuleHandleExW( GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, static_cast<LPWSTR>(addressOfItemInModule), &instance );

		return instance;
	}

// ---------------------------------------------------
	
	const char* GetCommandLine() {
		return GetCommandLineA();
	}

// ---------------------------------------------------

	const wchar_t* GetWideCommandLine() {
		return GetCommandLineW();
	}

// ---------------------------------------------------

	int ETPureFunctionHint ETNoThrowHint GetCmdShow() {
		return cmdShow;
	}

}	// namespace Utility
}	// namespace Eldritch2