/*==================================================================*\
  INIParser.inl
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

	template <typename SettingHandler>
	void ParseINI( const ::Eldritch2::Range<const ::Eldritch2::UTF8Char*>& sourceData, SettingHandler settingHandler ) {
		using namespace ::Eldritch2;
		using namespace ::std;

	// ---

		struct FunctionHelper {
			static const UTF8Char* StripByteOrderMark( const UTF8Char* const begin, const UTF8Char* const end ) {
				if( end - begin >= 3u && EqualityCompareString( begin, UTF8L("\0xEF\0xBB\0xBF"), 3u ) ) {
					return begin + 3u;
				}

				return begin;
			}

		// ---------------------------------------------------

#if( ET_COMPILER_IS_MSVC )
//	MSVC complains needlessly about a harmless type promotion for ::std::isspace().
#	pragma warning( push )
#	pragma warning( disable : 6330 )
#endif
			static const UTF8Char* FindCharacterOrComment( const UTF8Char* readLine, const UTF8Char* const readEnd, const UTF8Char character ) {
				for( int wasWhitespace( 0 ); (readLine != readEnd) && (*readLine != character) && !(wasWhitespace && *readLine == UTF8L( ';' )); ) {
					wasWhitespace = ::std::isspace( *readLine++ );
				}

				return readLine;
			}
		};
#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif

	// ---

		// The following is adapted from Ben Hoyt's inih library. Original source available at https://github.com/benhoyt/inih

		Range<const UTF8Char*>	section( Range<const UTF8Char*>::EmptySet() );

		// Consume the BOM, if one exists.
		const UTF8Char*	lineBegin( FunctionHelper::StripByteOrderMark( sourceData.first, sourceData.onePastLast ) );
		const UTF8Char*	lineEnd;

		// Scan through each line in the range.
		for( const UTF8Char* readEnd( Utility::FindElement( lineBegin, sourceData.onePastLast, UTF8L('\n') ) ); sourceData.Contains( lineBegin ); (lineBegin = readEnd), readEnd = Utility::FindElement( lineBegin, sourceData.onePastLast, UTF8L('\n') ) ) {
			// Strip out leading and trailing whitespace.
			lineBegin	= StripLeadingWhitespace( lineBegin, readEnd );
			lineEnd		= StripTrailingWhitespace( lineBegin, readEnd );

			if( (lineBegin == lineEnd) || *lineBegin == UTF8L(';') ) {
				// Comment/empty/whitespace-only line. Skip and move on.
				continue;
			} else if( *lineBegin == UTF8L('[') ) {
				// Section designator.

				// Advance the line begin by one element to consume the open bracket token.
				++lineBegin;

				// While a comment may be on the same line as a section declaration, it is not allowed to exist within the brackets...
				const UTF8Char* const	sectionEnd( FunctionHelper::FindCharacterOrComment( lineBegin, lineEnd, UTF8L(']') ) );

				// ... which we confirm.
				if( (sectionEnd != lineEnd) && (*sectionEnd == UTF8L(']')) ) {
					section = Range<const UTF8Char*>( StripLeadingWhitespace( lineBegin, sectionEnd ), StripTrailingWhitespace( lineBegin, sectionEnd ) );
				}
			} else {
				// <name> = <value> pair

				// Find the separation position between the name and value
				const UTF8Char* const	separatorPosition( FunctionHelper::FindCharacterOrComment( lineBegin, lineEnd, UTF8L('=') ) );

				// Ensure that any comments come after the assignment.
				if( (separatorPosition != lineEnd) && (*separatorPosition == UTF8L('=')) ) {
					Range<const UTF8Char*>	name( StripLeadingWhitespace( lineBegin, separatorPosition ), StripTrailingWhitespace( lineBegin, separatorPosition ) );

					if( !name.IsEmpty() ) {
						Range<const UTF8Char*>	value( StripLeadingWhitespace( separatorPosition + 1, lineEnd ), FunctionHelper::FindCharacterOrComment( separatorPosition + 1, lineEnd, UTF8L('\n') ) );
					
						// If there is a comment after the assignment, make sure we strip it out
						if( !value.IsEmpty() && *value.onePastLast == UTF8L(';') ) {
							value.onePastLast = StripTrailingWhitespace( value.first, value.onePastLast - 1 );
						}

						// Valid pair found, call handler
						settingHandler( section, name, value );
					}
				}
			}
		}
	}

}	// namespace Configuration
}	// namespace Eldritch2