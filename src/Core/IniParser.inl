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
	namespace {

		static ETPureFunctionHint const Utf8Char* StripBom(const Utf8Char* const begin, const Utf8Char* const end) {
			if (end - begin >= 3u && StringsEqual(begin, "\0xEF\0xBB\0xBF", 3u)) {
				return begin + 3u;
			}

			return begin;
		}

		// ---------------------------------------------------

		static ETPureFunctionHint const Utf8Char* FindCharacterOrComment(const Utf8Char* begin, const Utf8Char* const end, const Utf8Char character) {
			ET_PUSH_COMPILER_WARNING_STATE()
			//	(6330) MSVC complains about a harmless type promotion for std::isspace().
			ET_SET_MSVC_WARNING_STATE(disable : 6330)
			for (int wasWhitespace(0); (begin != end) && (*begin != character) && !(wasWhitespace && *begin == ';');) {
				wasWhitespace = std::isspace(*begin++);
			}

			return begin;
			ET_POP_COMPILER_WARNING_STATE()
		}

	} // anonymous namespace

	template <typename TrinaryConsumer, typename AltTrinaryConsumer>
	void ApplyIni(StringView<Utf8Char> source, TrinaryConsumer keyHandler, AltTrinaryConsumer unknownKeyHandler) {
		//	The following is adapted from Ben Hoyt's inih library. Original source available at https://github.com/benhoyt/inih
		//	Consume the BOM, if one exists.
		const Utf8Char* line(StripBom(source.Begin(), source.End()));
		const Utf8Char* lineEnd;
		String<>        group, name;

		//	Scan through each line in the range.
		for (const Utf8Char* readEnd(Find(line, source.End(), '\n')); source.Covers(line); (line = readEnd), readEnd = Find(line, source.End(), '\n')) {
			//	Strip out leading and trailing whitespace.
			line    = TrimLeadingWhitespace(line, readEnd);
			lineEnd = TrimTrailingWhitespace(line, readEnd);

			if ((line == lineEnd) || (*line == ';')) {
				//	Comment/empty/whitespace-only line. Skip and move on.
				continue;
			}

			//	Section/group designator.
			if (*line == '[') {
				//	Advance the line begin by one element to consume the open bracket token.
				++line;

				//	While a comment may be on the same line as a section declaration, it is not allowed to exist within the brackets...
				const auto sectionEnd(FindCharacterOrComment(line, lineEnd, ']'));
				//	... which we confirm.
				if ((sectionEnd != lineEnd) && (*sectionEnd == ']')) {
					group.Assign(TrimLeadingWhitespace(line, sectionEnd), TrimTrailingWhitespace(line, sectionEnd));
				}

				continue;
			}

			//	<name> = <value> pair
			//	Find the separation position between the name and value.
			const auto delimiter(FindCharacterOrComment(line, lineEnd, '='));
			//	Ensure that any comments come after the assignment.
			if ((delimiter == lineEnd) || (*delimiter != '=')) {
				continue;
			}

			name.Assign(TrimLeadingWhitespace(line, delimiter), TrimTrailingWhitespace(line, delimiter));
			if (name.IsEmpty()) {
				continue;
			}

			const Utf8Char* value(TrimLeadingWhitespace(delimiter + 1, lineEnd));
			const Utf8Char* valueEnd(FindCharacterOrComment(delimiter + 1, lineEnd, '\n'));

			/*	If there is a comment after the assignment, make sure we strip it out. Note that the value string being the empty string
			*	(i.e. value == valueEnd) IS ALLOWED according to this implementation. */
			if ((value != valueEnd) && *valueEnd == ';') {
				valueEnd = TrimTrailingWhitespace(value, valueEnd - 1);
			}

			//	Assignment found, try to add to the database or invoke unknown key handler if no matching property is found.
			if (keyHandler(group, name, { value, valueEnd })) {
				continue;
			}

			unknownKeyHandler(group, name, { value, valueEnd });
		}
	}

}} // namespace Eldritch2::Core
