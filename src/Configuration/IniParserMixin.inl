/*==================================================================*\
  IniParserMixin.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/StandardLibrary.hpp>
#include <Utility/Containers/Range.hpp>
#include <Utility/Algorithms.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Configuration {
namespace {

	static ETPureFunctionHint const Eldritch2::Utf8Char* StripByteOrderMark( const Eldritch2::Utf8Char* const begin, const Eldritch2::Utf8Char* const end ) {
		if( end - begin >= 3u && Eldritch2::EqualityCompareString( begin, "\0xEF\0xBB\0xBF", 3u ) ) {
			return begin + 3u;
		}

		return begin;
	}

#if( ET_COMPILER_IS_MSVC )
	//	MSVC complains needlessly about a harmless type promotion for std::isspace().
#	pragma warning( push )
#	pragma warning( disable : 6330 )
#endif
	static ETPureFunctionHint const Eldritch2::Utf8Char* FindCharacterOrComment( const Eldritch2::Utf8Char* readLine, const Eldritch2::Utf8Char* const readEnd, const Eldritch2::Utf8Char character ) {
		for( int wasWhitespace( 0 ); ( readLine != readEnd ) && ( *readLine != character ) && !( wasWhitespace && *readLine == ';' ); ) {
			wasWhitespace = std::isspace( static_cast<unsigned char>( *readLine++ ) );
		}

		return readLine;
	}
#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif

}	// anonymous namespace

	template <typename Database>
	template <typename... Arguments>
	IniParserMixin<Database>::IniParserMixin( Arguments&&... arguments ) : Database( eastl::forward<Arguments>( arguments )... ) {}

// ---------------------------------------------------

	template <typename Database>
	void IniParserMixin<Database>::ApplyIni( Eldritch2::Range<const Eldritch2::Utf8Char*> sourceData ) {
	//	The following is adapted from Ben Hoyt's inih library. Original source available at https://github.com/benhoyt/inih
		Range<const Eldritch2::Utf8Char*>	section( Range<const Eldritch2::Utf8Char*>::CreateEmptySet() );

	//	Consume the BOM, if one exists.
		const Eldritch2::Utf8Char*	lineBegin( StripByteOrderMark( sourceData.Begin(), sourceData.End() ) );
		const Eldritch2::Utf8Char*	lineEnd;

	//	Scan through each line in the range.
		for( const Eldritch2::Utf8Char* readEnd( FindElement( lineBegin, sourceData.End(), '\n' ) ); sourceData.ContainsIterator( lineBegin ); (lineBegin = readEnd), readEnd = FindElement( lineBegin, sourceData.End(), '\n' ) ) {
		//	Strip out leading and trailing whitespace.
			lineBegin	= StripLeadingWhitespace( lineBegin, readEnd );
			lineEnd		= StripTrailingWhitespace( lineBegin, readEnd );

			if( (lineBegin == lineEnd) || *lineBegin == ';' ) {
			//	Comment/empty/whitespace-only line. Skip and move on.
				continue;
			} else if( *lineBegin == '[' ) {
			//	Section designator.

			//	Advance the line begin by one element to consume the open bracket token.
				++lineBegin;

			//	While a comment may be on the same line as a section declaration, it is not allowed to exist within the brackets...
				const Eldritch2::Utf8Char* const	sectionEnd( FindCharacterOrComment( lineBegin, lineEnd, ']' ) );

			//	... which we confirm.
				if( (sectionEnd != lineEnd) && (*sectionEnd == ']') ) {
					section = Range<const Eldritch2::Utf8Char*>( StripLeadingWhitespace( lineBegin, sectionEnd ), StripTrailingWhitespace( lineBegin, sectionEnd ) );
				}
			} else {
			//	<name> = <value> pair

			//	Find the separation position between the name and value
				const Eldritch2::Utf8Char* const	separatorPosition( FindCharacterOrComment( lineBegin, lineEnd, '=' ) );

			//	Ensure that any comments come after the assignment.
				if( (separatorPosition != lineEnd) && (*separatorPosition == '=') ) {
					Range<const Eldritch2::Utf8Char*>	name( StripLeadingWhitespace( lineBegin, separatorPosition ), StripTrailingWhitespace( lineBegin, separatorPosition ) );

					if( !name.IsEmpty() ) {
						const Eldritch2::Utf8Char*	valueBegin( StripLeadingWhitespace( separatorPosition + 1, lineEnd ) );
						const Eldritch2::Utf8Char*	valueEnd( FindCharacterOrComment( separatorPosition + 1, lineEnd, '\n' ) );
					
					//	If there is a comment after the assignment, make sure we strip it out
						if( (valueBegin != valueEnd) && *valueEnd == ';' ) {
							valueEnd = StripTrailingWhitespace( valueBegin, valueEnd - 1 );
						}

					//	Valid pair found, call handler.
						this->SetProperty( section, name, { valueBegin, valueEnd } );
					}
				}
			}
		}
	}

}	// namespace Configuration
}	// namespace Eldritch2