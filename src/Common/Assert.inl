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
ET_PUSH_MSVC_WARNING_STATE(disable : 4459 4244)
#include <fmt/format.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename... Arguments>
AssertionFailure ReportFailure(StringView condition, StringView file, uint32 line, StringView format, Arguments&&... arguments) {
	fmt::memory_buffer message;
	fmt::format_to(message, fmt::string_view(format.Begin(), format.GetLength()), eastl::forward<Arguments>(arguments)...);

	return ReportFailure(condition, file, line, StringView(message.data(), message.size()));
}

} // namespace Eldritch2
