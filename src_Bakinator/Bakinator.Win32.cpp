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
#include <Common/Memory.hpp>
#include <Bakinator.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2;

int wmain(int argc, PlatformChar* argv[]) {
	ArrayList<Utf8Char[MAX_PATH]> arguments(MallocAllocator("Global Allocator"), size_t(argc));

	for (int i(0); i < argc; ++i) {
		TranscodeString<Utf8Char>(argv[i], FindTerminator(argv[i]), arguments[i]);
	}

	return AsPosixInt(Tools::Bakinator().Run({arguments.Begin(), arguments.End()});
}
