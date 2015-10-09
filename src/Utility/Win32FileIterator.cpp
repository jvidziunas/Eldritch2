/*==================================================================*\
  Win32FileIterator.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2012 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <util/Win32FileIterator.hpp>
#include <util/memory/MemStdLib.hpp>
#include <Shlwapi.h>
#include <algorithm>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "Shlwapi.lib" )
//------------------------------------------------------------------//

#ifdef min
#	undef min
#endif

using namespace ::Eldritch2;
using namespace ::Eldritch2::Util;
using namespace ::std;

namespace Eldritch2
{

	namespace Util
	{

		Win32FileIterator::Win32FileIterator() : _findHandle( INVALID_HANDLE_VALUE )
		{
		}

	// ---------------------------------------------------

		bool Win32FileIterator::BeginSearch( const TCHAR* searchString )
		{
			if( INVALID_HANDLE_VALUE != _findHandle )
			{
				::FindClose( _findHandle );
			}

			TCHAR	pathTemp[MAX_PATH];

			::PathCanonicalize( pathTemp, searchString );
			_sourceFileFolder.assign( pathTemp, _tcscspn( pathTemp, SL("?*") ) );
			size_t newLength = min( _sourceFileFolder.size(), _sourceFileFolder.find_last_of( SL("/\\") )+1 );
			size_t folderFixup = SL('\\') == _sourceFileFolder[0] ? 1 : 0;
			_sourceFileFolder = _sourceFileFolder.substr( folderFixup, newLength - folderFixup );

			_findHandle = ::FindFirstFile( searchString, &_findData );

			if( INVALID_HANDLE_VALUE != _findHandle )
			{
				_fullPath.assign( _sourceFileFolder + _findData.cFileName );
				return true;
			}

			return false;
		}

	// ---------------------------------------------------

		Win32FileIterator::~Win32FileIterator()
		{
			if( INVALID_HANDLE_VALUE != _findHandle )
			{
				::FindClose( _findHandle );
			}
		}

	// ---------------------------------------------------

		const Win32FileIterator::tstring& Win32FileIterator::GetCurrentFileName() const
		{
			return _fullPath;
		}

	// ---------------------------------------------------

		bool Win32FileIterator::Advance()
		{
			if( INVALID_HANDLE_VALUE == _findHandle || !::FindNextFile( _findHandle, &_findData ) )
				return false;

			_fullPath.assign( _sourceFileFolder + _findData.cFileName );
			return true;
		}

	}	// namespace Util

}	// namespace Eldritch2