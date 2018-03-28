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
ET_PUSH_COMPILER_WARNING_STATE()
	ET_SET_MSVC_WARNING_STATE( disable : 4459 4244 )
#	include <fmt/format.h>
ET_POP_COMPILER_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <size_t formatSize, typename... Arguments>
	AssertionFailure ReportFailure( const char* condition, const char* file, uint32 line, const char (&format)[formatSize], Arguments&&... arguments ) {
		fmt::memory_buffer  string;

		fmt::format_to( string, fmt::string_view( format, formatSize ), eastl::forward<Arguments>( arguments )... );

		return ReportFailure( condition, file, line, string.data() );
	}

}	// namespace Eldritch2