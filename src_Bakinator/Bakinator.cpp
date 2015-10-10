/*==================================================================*\
  Bakinator.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <BakinatorCookerStrategy.hpp>
#include <Tools/Win32FileStrategyFactory.hpp>
#include <Tools/ContentCooker.hpp>
#include <Utility/Memory/Win32DiskSectorAllocator.hpp>
#include <Utility/Memory/Win32HeapAllocator.hpp>
#include <Utility/ErrorCode.hpp>
#include <tchar.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2;
using namespace ::Eldritch2::Tools;
using namespace ::std;

int _tmain( int argc, TCHAR* argv[] )
{
	Win32DiskSectorAllocator	fileDataAllocator;
	Win32HeapAllocator			allocator;
	Win32FileStrategyFactory	fileStrategyFactory( allocator );
	BakinatorCookerStrategy		cookerStrategy( fileDataAllocator );	

	_putts( SL( "Eldritch Coalesced Resource Format creator utility\r\n---\r\n" ) );

	return CookContent( static_cast<size_t>( argc ), argv, cookerStrategy, fileStrategyFactory ).ToInt();
}