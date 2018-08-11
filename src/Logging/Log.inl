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

	ETInlineHint Log::Log() :
		_muteThreshold(Logging::MessageType::VerboseWarning) {}

	// ---------------------------------------------------

	template <typename... Arguments>
	ETInlineHint void Log::Write(Logging::MessageType type, StringView<Utf8Char> format, Arguments&&... arguments) {
		static const Utf8Char prefixes[][8] = {
			"[WARN] ",
			"[WARN] ",
			"[ERR ] ",
			"[INFO] "
		};

		if (type < GetMuteThreshold()) {
			return;
		}

		fmt::memory_buffer output;
		const auto&        prefix(prefixes[static_cast<size_t>(type)]);

		output.append(eastl::begin(prefix), eastl::end(prefix) - 1);
		fmt::format_to(output, format, eastl::forward<Arguments>(arguments)...);
		Write(output.data(), output.size());
	}

	// ---------------------------------------------------

	ETInlineHint MessageType Log::GetMuteThreshold() const {
		return _muteThreshold.load(std::memory_order_acquire);
	}

	// ---------------------------------------------------

	ETInlineHint void Log::SetWriteThreshold(MessageType threshold) {
		_muteThreshold.store(threshold, std::memory_order_release);
	}

}} // namespace Eldritch2::Logging
