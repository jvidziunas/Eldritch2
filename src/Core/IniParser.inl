/*==================================================================*\
  IniParser.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Core {

	template <typename TrinaryConsumer>
	ETInlineHint void ParseIni(StringSpan source, TrinaryConsumer keyHandler) ETNoexceptHint {
		static ETConstexpr StringSpan Whitespace = " \f\n\r\t\v";
		static ETConstexpr Utf8Char Comment      = ';';

		StringSpan group;

		const auto ParseGroupHeader([&](StringSpan line) ETNoexceptHint -> void {
			const StringSpan::SizeType delimiter(line.FindAny("];", 1u));
			if (delimiter == StringSpan::SizeType(-1) || line[delimiter] == Comment) {
				//	Comments may not be placed within the group header, though they may appear after the end of the header.
				return;
			}

			line = line.Slice(0u, delimiter);

			const StringSpan::SizeType groupBegin(line.FindNot(Whitespace, 1u));
			group = line.Slice(groupBegin, line.FindNotLast(Whitespace, groupBegin));
		});

		const auto ParseAssignment([&](StringSpan line) ETNoexceptHint -> void {
			const StringSpan::SizeType delimiter(line.FindAny("=;"));
			if (delimiter == StringSpan::SizeType(-1) || line[delimiter] == Comment) { // Ensure that any comments come *after* the assignment.
				return;
			}

			// Split name/value pair across '=' delimiter and strip comment if present.
			StringSpan name(line.Slice(0u, delimiter));
			StringSpan value(line.Slice(line.FindNot(Whitespace, delimiter + 1u), line.Find(Comment, delimiter + 1u)));

			keyHandler(group, name.Slice(0u, name.FindNotLast(Whitespace)), value.Slice(0u, value.FindNotLast(Whitespace)));
		});

		// Strip BOM if present.
		source.RemovePrefix(/*UTF-8 BOM*/ "\0xEF\0xBB\0xBF");
		for (StringSpan::SizeType sourcePos(0u), newline(source.Find('\n', sourcePos)); sourcePos < source.GetLength(); sourcePos = newline, newline = source.Find('\n', sourcePos)) {
			const StringSpan::SizeType lineBegin(source.FindNot(Whitespace, sourcePos));
			const StringSpan           line(source.Slice(lineBegin, source.FindNotLast(Whitespace, lineBegin)));

			if (line.IsEmpty() || line.Front() == Comment) {
				continue; // Comment-only/empty line.
			}

			if (line.Front() == '[') {
				ParseGroupHeader(line);
				continue;
			}

			ParseAssignment(line);
		}
	}

}} // namespace Eldritch2::Core
