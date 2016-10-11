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
#define FMT_EXCEPTIONS 0
#include <cppformat/fmt/format.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Logging {
namespace {

	static const char* const	nameTable[] = { "VerboseWarning", "Warning", "Error", "Never" };

}	// anonymous namespace

	template <typename Stream>
	Stream& operator >>( Stream& stream, Logging::MessageSeverity& threshold ) {
		using MessageSeverityArithmeticType = typename eastl::underlying_type<Logging::MessageSeverity>::type;

	// ---

	//	TODO: This should really use a hash map or something.
		MessageSeverityArithmeticType 	temp( static_cast<MessageSeverityArithmeticType>(Logging::MessageSeverity::Message) );
		char							value[16u];

		stream.get( value, _countof( value ) );

		const auto	location( eastl::find( eastl::begin( nameTable ), eastl::end( nameTable ), value, [] ( const char* string0, const char* string1 ) {
			return Eldritch2::EqualityCompareStringCaseInsensitive( string0, string1 );
		} ) );

		threshold = static_cast<Logging::MessageSeverity>( temp - eastl::distance( eastl::begin( nameTable ), location ) );

		return stream;
	}

// ---------------------------------------------------

	template <typename Stream>
	Stream& operator <<( Stream& stream, Logging::MessageSeverity threshold ) {
		stream << nameTable[static_cast<size_t>( threshold )];

		return stream;
	}

// ---------------------------------------------------

	ETInlineHint Log::Log() : _muteThreshold( Logging::MessageSeverity::VerboseWarning ) {}

// ---------------------------------------------------

	template <typename... Arguments>
	ETInlineHint void Log::operator()( Logging::MessageSeverity severity, const Eldritch2::Utf8Char* str, Arguments&&... arguments ) {
		if( severity < GetMuteThreshold() ) {
			return;
		}

		Eldritch2::Utf8Char	formattedString[256u];
		fmt::ArrayWriter	formatter( formattedString );

		formatter.write( str, eastl::forward<Arguments>( arguments )... );

		Write( formatter.c_str(), formatter.size());
	}

// ---------------------------------------------------

	ETInlineHint Logging::MessageSeverity Log::GetMuteThreshold() const {
		return _muteThreshold.load( std::memory_order_acquire );
	}

// ---------------------------------------------------

	ETInlineHint void Log::SetMuteThreshold( Logging::MessageSeverity muteThreshold ) {
		_muteThreshold.store( muteThreshold, std::memory_order_release );
	}

}	// namespace Logging
}	// namespace Eldritch2