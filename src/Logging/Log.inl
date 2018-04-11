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

namespace Eldritch2 {
namespace Logging {

	ETInlineHint Log::Log() : _muteThreshold( Logging::MessageType::VerboseWarning ) {}

// ---------------------------------------------------

	template <size_t formatSize, typename... Arguments>
	ETInlineHint void Log::Write( Logging::MessageType type, const Utf8Char (&formatString)[formatSize], Arguments&&... arguments ) {
		static const Utf8Char prefixes[][8] = {
			"[WARN] ",
			"[WARN] ",
			"[ERR ] ",
			"[INFO] "
		};

		if (type < GetMuteThreshold()) {
			return;
		}


		fmt::memory_buffer	string;
		const auto&			prefix( prefixes[static_cast<size_t>(type)] );

		string.append( eastl::begin( prefix ), eastl::end( prefix ) - 1 );

		fmt::vformat_to( string, fmt::basic_string_view<Utf8Char>( formatString, formatSize ), fmt::make_args( eastl::forward<Arguments>( arguments )... ) );

		Write( string.data(), string.size() );
	}

// ---------------------------------------------------

	ETInlineHint Logging::MessageType Log::GetMuteThreshold() const {
		return _muteThreshold.load( std::memory_order_acquire );
	}

// ---------------------------------------------------

	ETInlineHint void Log::SetMuteThreshold( Logging::MessageType muteThreshold ) {
		_muteThreshold.store( muteThreshold, std::memory_order_release );
	}

}	// namespace Logging
}	// namespace Eldritch2