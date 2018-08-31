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

	static ETConstexpr StringView Whitespace = " \f\n\r\t\v";

	// ---------------------------------------------------

	template <typename TrinaryConsumer, typename AltTrinaryConsumer>
	void ParseIniAssignment(StringView group, StringView line, TrinaryConsumer& keyHandler, AltTrinaryConsumer& unknownKeyHandler) {
		using Offset = StringView::SizeType;

		const Offset delimiter(line.FindAny("=;"));
		if (delimiter == Offset(-1) || line[delimiter] == ';') { // Ensure that any comments come *after* the assignment.
			return;
		}

		// Split name/value pair across '=' delimiter and strip comment if present.
		StringView name(line.Substring(0u, delimiter));
		StringView value(line.Substring(line.FindNot(Whitespace, delimiter + 1), line.Find(';', delimiter + 1)));
		name  = name.Substring(0u, name.FindNotLast(Whitespace));
		value = value.Substring(0u, value.FindNotLast(Whitespace));

		if (!keyHandler(group, name, value)) {
			unknownKeyHandler(group, name, value);
		}
	}

	// ---------------------------------------------------

	StringView ParseSectionHeader(StringView line) {
		using Offset = StringView::SizeType;

		const Offset delimiter(line.FindAny("];", 1u));
		if (delimiter == Offset(-1) || line[delimiter] == ';') {
			//	Comments may not be placed within the section header, though they may appear after the end of the header.
			return "";
		}
		StringView section(line.Substring(line.FindNot(Whitespace, 1u), delimiter));
		return section.Substring(0u, section.FindNotLast(Whitespace));
	}

	// ---------------------------------------------------

	template <typename TrinaryConsumer, typename AltTrinaryConsumer>
	void ParseIni(StringView source, TrinaryConsumer keyHandler, AltTrinaryConsumer unknownKeyHandler) {
		using Offset = StringView::SizeType;

		StringView section;

		source.RemovePrefix("\0xEF\0xBB\0xBF" /*UTF-8 BOM*/);
		for (Offset position(0u), newline(source.Find('\n', position)); position < source.GetLength(); position = newline, newline = source.Find('\n', position)) {
			StringView line(source.Substring(source.FindNot(Whitespace, position), newline));
			line = line.Substring(0, line.FindNotLast(Whitespace));

			if (line.IsEmpty() || line.Front() == ';') {
				continue; // Comment-only/empty line.
			}

			if (line.Front() == '[') {
				section = ParseSectionHeader(line);
				continue;
			}

			ParseIniAssignment(section, line, keyHandler, unknownKeyHandler);
		}
	}

}} // namespace Eldritch2::Core
