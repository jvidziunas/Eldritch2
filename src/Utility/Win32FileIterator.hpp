/*==================================================================*\
  Win32FileIterator.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2012 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Windows.h>
#include <tchar.h>
#include <string>
//------------------------------------------------------------------//

namespace Eldritch2
{

	namespace Util
	{

		class Win32FileIterator
		{
		public:
			typedef std::basic_string<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR> > tstring;

		// ---------------------------------------------------

			 Win32FileIterator();
			~Win32FileIterator();

		// ---------------------------------------------------

			bool	BeginSearch( const TCHAR* searchString );

			bool	Advance();

		// ---------------------------------------------------

			// Returns the fully-qualified/absolute file path for the current file in the search,
			// or an empty string if the search is invalid for some reason.
			const tstring&	GetCurrentFileName() const;

		// ---------------------------------------------------

		private:
			tstring			_sourceFileFolder;
			tstring			_fullPath;
			HANDLE			_findHandle;
			WIN32_FIND_DATA	_findData;
		};

	}	// namespace Util

}	// namespace Eldritch2