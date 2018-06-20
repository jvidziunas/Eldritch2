/*==================================================================*\
  ShaderCompiler.Win32.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Memory/MallocAllocator.hpp>
#include <Common/UniquePointer.hpp>
#include <ShaderCompiler.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2;

namespace {

template <size_t size>
static bool Utf8FromWideString(Utf8Char (&out)[size], const PlatformChar* source) {
	return 0 != WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, source, -1, out, size, NULL, NULL);
}

} // anonymous namespace

int wmain(int argc, PlatformChar** argv) {
	MallocAllocator allocator("Global Allocator");
	const auto      convertedArgs(MakeUniqueArray<Utf8Char[MAX_PATH]>(allocator, static_cast<size_t>(argc)));

	for (int i = 0; i < argc; ++i) {
		if (!Utf8FromWideString(convertedArgs[i], argv[i])) {
			return 1;
		}
	}

	return Tools::ShaderCompiler().Run(convertedArgs.Begin(), convertedArgs.End());
}
