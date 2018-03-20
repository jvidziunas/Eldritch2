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
namespace {

	static const char* const	nameTable[] = { "VerboseWarning", "Warning", "Error", "Never" };

}	// anonymous namespace

	template <typename Stream>
	ETInlineHint Stream& operator >>( Stream& stream, Logging::MessageType& threshold ) {
	//	TODO: This should really use a hash map.
		auto 	temp( static_cast<eastl::underlying_type<Logging::MessageType>::type>(Logging::MessageType::Message) );
		char	value[16u];

		stream.get( value, _countof( value ) );

		const auto	location( FindIf( eastl::begin( nameTable ), eastl::end( nameTable ), [&] ( const char* name ) {
			return StringsEqualCaseInsensitive( value, name );
		} ) );

		threshold = static_cast<Logging::MessageType>( temp - eastl::distance( eastl::begin( nameTable ), location ) );

		return stream;
	}

// ---------------------------------------------------

	template <typename Stream>
	ETInlineHint Stream& operator <<( Stream& stream, Logging::MessageType threshold ) {
		stream << nameTable[static_cast<size_t>( threshold )];

		return stream;
	}

// ---------------------------------------------------

	ETInlineHint Log::Log() : _muteThreshold( Logging::MessageType::VerboseWarning ) {}

// ---------------------------------------------------

	template <size_t formatSize, typename... Arguments>
	ETInlineHint void Log::Write( Logging::MessageType type, const Utf8Char (&formatString)[formatSize], Arguments&&... arguments ) {
		if (type < GetMuteThreshold()) {
			return;
		}

		fmt::memory_buffer	string;
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