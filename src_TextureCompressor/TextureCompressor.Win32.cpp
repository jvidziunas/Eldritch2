/*==================================================================*\
  TextureCompressor.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Memory/MallocAllocator.hpp>
#include <Common/UniquePointer.hpp>
#include <TextureCompressor.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Tools;
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
	MallocAllocator	allocator( "Global Allocator" );
	const auto		convertedArgs( MakeUniqueArray<Utf8Char[MAX_PATH]>( allocator, static_cast<size_t>(argc) ) );

	for( int i = 0; i < argc; ++i ) {
		if( !Utf8FromWideString( convertedArgs[i], argv[i] ) ) {
			return 1;
		}
	}

	return TextureCompressor( allocator ).Run( convertedArgs.Begin(), convertedArgs.End() );
}