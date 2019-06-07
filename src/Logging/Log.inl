/*==================================================================*\
  Log.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Logging {

	ETInlineHint ETForceInlineHint Log::Log(const Log& /*unused*/) ETNoexceptHint : Log() {}

	// --------------------------------------------------

	ETInlineHint ETForceInlineHint Log::Log() ETNoexceptHint : _writeThreshold(VerboseWarning) {}

	// ---------------------------------------------------

	template <typename... Arguments>
	ETInlineHint ETForceInlineHint void Log::Write(Severity severity, StringSpan format, Arguments&&... arguments) {
		static ETConstexpr StringSpan prefixes[] = {
			"[WARN] ",
			"[WARN] ",
			"[ERR ] ",
			"[INFO] "
		};

		if (severity < GetWriteThreshold()) {
			return;
		}

		::fmt::memory_buffer output;
		const StringSpan&    prefix(prefixes[severity]);

		output.append(prefix.Begin(), prefix.End());
		fmt::format_to(output, ::fmt::string_view(format.GetData(), format.GetLength()), Forward<Arguments>(arguments)...);
		Write(output.data(), output.size());
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint Severity Log::GetWriteThreshold(MemoryOrder order) const ETNoexceptHint {
		return _writeThreshold.load(order);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void Log::SetWriteThreshold(Severity threshold, MemoryOrder order) ETNoexceptHint {
		_writeThreshold.store(threshold, order);
	}

}} // namespace Eldritch2::Logging
