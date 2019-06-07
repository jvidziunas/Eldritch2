/*==================================================================*\
  ConsoleLog.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/String.hpp>
#include <Common/Mpl/CharTypes.hpp>
#include <Common/Atomic.hpp>
//------------------------------------------------------------------//
#include <Tools/ConsoleLog.hpp>
//------------------------------------------------------------------//
#include <iostream>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Logging {

	void ConsoleLog::Write(const Utf8Char* const string, size_t octetLength) {
		TranscodeString<wchar_t>(string, string + octetLength, std::ostreambuf_iterator<wchar_t>(std::wcout));
	}

}} // namespace Eldritch2::Logging
