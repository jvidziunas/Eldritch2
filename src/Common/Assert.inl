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

	void ReportFailure(StringSpan message) ETNoexceptHint;

template <typename... Arguments>
ETInlineHint void ReportFailure(StringSpan file, uint32 line, StringSpan format, Arguments&&... arguments) {
	fmt::basic_memory_buffer<StringSpan::CharacterType> message;
	fmt::format_to(message, fmt::basic_string_view<StringSpan::CharacterType>("{}({}):" ET_NEWLINE), file, line);
	fmt::format_to(message, fmt::basic_string_view<StringSpan::CharacterType>(format.GetData(), format.GetLength()), arguments...);
	message.push_back('\0');

	ReportFailure(StringSpan(message.data(), StringSpan::SizeType(message.size())));
}

} // namespace Eldritch2
