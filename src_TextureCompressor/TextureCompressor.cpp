/*==================================================================*\
  TextureCompressor.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#define BOOST_ERROR_CODE_HEADER_ONLY
#include <boost/system/error_code.hpp>
#include <TextureCompressorCookerStrategy.hpp>
#include <Tools/Win32FileStrategyFactory.hpp>
#include <Tools/ContentCooker.hpp>
#include <Utility/Memory/Win32DiskSectorAllocator.hpp>
#include <Utility/Memory/Win32HeapAllocator.hpp>
#include <Utility/ErrorCode.hpp>
#include <tchar.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2;
using namespace ::Eldritch2::Tools;

int _tmain( int argc, TCHAR* argv[] )
{
	Win32DiskSectorAllocator	fileAllocator;
	Win32HeapAllocator			allocator;
	Win32FileStrategyFactory	fileStrategyFactory( allocator, fileAllocator );

	InstallGlobalAllocator(	allocator );

	return ContentCooker<TextureCompressorCookerStrategy>().BeginCook( argv,
																	   argv + argc,
																	   fileStrategyFactory,
																	   fileStrategyFactory ).ToInt();
}