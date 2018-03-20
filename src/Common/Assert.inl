/*==================================================================*\
  Assert.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <fmt/format.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <size_t formatSize, typename... Arguments>
	AssertionFailure ReportFailure( const char* condition, const char* file, uint32 line, const char (&format)[formatSize], Arguments&&... arguments ) {
		fmt::memory_buffer  string;

		fmt::format_to( string, fmt::string_view( format, formatSize ), eastl::forward<Arguments>( arguments )... );

		return ReportFailure( condition, file, line, string.data() );
	}

}	// namespace Eldritch2