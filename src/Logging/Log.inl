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

	ETInlineHint ETForceInlineHint Log::Log() ETNoexceptHint : _writeThreshold(VerboseWarning) {}

	// ---------------------------------------------------

	template <typename... Arguments>
	ETInlineHint ETForceInlineHint void Log::Write(Severity severity, StringView format, Arguments&&... arguments) {
		using namespace ::fmt;

		static ETConstexpr StringView prefixes[] = {
			"[WARN] ",
			"[WARN] ",
			"[ERR ] ",
			"[INFO] "
		};

		if (severity < GetWriteThreshold()) {
			return;
		}

		const StringView& prefix(prefixes[severity]);
		memory_buffer     output;

		output.append(prefix.Begin(), prefix.End());
		fmt::format_to(output, string_view(format.GetData(), format.GetLength()), eastl::forward<Arguments>(arguments)...);
		Write(output.data(), output.size());
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint Severity Log::GetWriteThreshold() const ETNoexceptHint {
		return _writeThreshold.load(std::memory_order_acquire);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void Log::SetWriteThreshold(Severity threshold) ETNoexceptHint {
		_writeThreshold.store(threshold, std::memory_order_release);
	}

}} // namespace Eldritch2::Logging
