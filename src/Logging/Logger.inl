/*==================================================================*\
  Logger.inl
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
#include <cppformat/format.h>
//------------------------------------------------------------------//
#include <iostream>
//------------------------------------------------------------------//

namespace std {

	template <typename Traits>
	::std::basic_istream<char, Traits>&	operator >>(::std::basic_istream<char, Traits>& stream, ::Eldritch2::Foundation::LogMessageType& threshold) {
		static const char* const	nameTable[] = { "VerboseWarning", "Warning", "Error", "Never" };
		::Eldritch2::uint32			result { 0 };
		char						value[16u];

		stream.get( value, _countof( value ) );

		for( const char* const name : nameTable ) {
			if( ::Eldritch2::EqualityCompareStringCaseInsensitive( name, value ) ) {
				threshold = static_cast<::Eldritch2::Foundation::LogMessageType>(result);
				break;
			}

			++result;
		}

		return stream;
	}

// ---------------------------------------------------

	template <typename Traits>
	::std::basic_ostream<char, Traits>&	operator <<(::std::basic_ostream<char, Traits>& stream, const ::Eldritch2::Foundation::LogMessageType threshold) {
		static const char* const	nameTable[] = { "VerboseWarning", "Warning", "Error", "Never" };

		return stream << nameTable[static_cast<size_t>(threshold)];
	}

}	// namespace std

namespace Eldritch2 {
namespace Foundation {

template <typename... Arguments>
void Logger::operator()( const ::Eldritch2::UTF8Char* str, Arguments&&... arguments ) {
	::Eldritch2::UTF8Char	formattedString[256u];
	fmt::ArrayWriter		formatter( formattedString, _countof(formattedString) );

	formatter.write( str, ::std::forward<Arguments>( arguments )... );

	Write( formatter.c_str(), formatter.size() );
}

}	// namespace Foundation
}	// namespace Eldritch2