/*==================================================================*\
  Bakinator.Win32.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Memory/MallocAllocator.hpp>
#include <Bakinator.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2;

namespace {

	template <size_t bufferSize>
	static bool Utf8FromWideString( Utf8Char (&out)[bufferSize], const PlatformChar* source ) {
		return 0 != WideCharToMultiByte(
			CP_UTF8,
			WC_ERR_INVALID_CHARS,
			source,
			-1,
			out,
			bufferSize,
			NULL,
			NULL
		);
	}

}	// anonymous namespace

int wmain( int argc, PlatformChar** argv ) {
	ArrayList<Utf8Char[MAX_PATH]>	arguments( static_cast<size_t>(argc), MallocAllocator( "Global Allocator" ) );

	for (int i( 0 ); i < argc; ++i) {
		if (!Utf8FromWideString( arguments[i], argv[i] )) {
			return 1;
		}
	}

	return Tools::Bakinator().Run( arguments.Begin(), arguments.End() );
}