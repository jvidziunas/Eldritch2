/*==================================================================*\
  MeshCooker.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2012 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <util/mpl/Compiler.hpp>
#include <windows.h>
#include <tchar.h>
#include <iostream>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//

//------------------------------------------------------------------//

using namespace Eldritch2;
using namespace std;

int _tmain( int argc, TCHAR* argv[] )
{
	WIN32_FIND_DATA	findData;
	HANDLE			findHandle;
	TCHAR			outputFileName[256] = { SL('\0') };

	if( argc < 2 )
		return -1;

	cout << endl << "Eldritch Tech 2.0 Mesh Tool" << endl << "---" << endl;

	findHandle = ::FindFirstFile( argv[1], &findData );

	if( INVALID_HANDLE_VALUE == findHandle ) 
		return -1;

	do
	{

	} while( 0 != ::FindNextFile( findHandle, &findData ) );

	::FindClose( findHandle );

	return 0;
}