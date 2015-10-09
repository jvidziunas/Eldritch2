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
#include <Utility/Containers/Range.hpp>
#include <Utility/Memory/MemStdLib.hpp>
#include <Utility/Algorithms.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Configuration {

	template <typename SettingHandler>
	void ParseINI( const ::Eldritch2::Range<const ::Eldritch2::UTF8Char*>& sourceData, SettingHandler settingHandler ) {
		// The following is adapted from Ben Hoyt's inih library. Original source available at https://github.com/benhoyt/inih

		using namespace ::Eldritch2;
		using namespace ::std;

		struct FunctionHelper {
			static const UTF8Char* StripByteOrderMark( const UTF8Char* const begin, const UTF8Char* const end ) {
				if( end - begin >= 3u && EqualityCompareString( begin, UTF8L("\0xEF\0xBB\0xBF"), 3u ) ) {
					return begin + 3u;
				}

				return begin;
			}

		// ---------------------------------------------------

			static const UTF8Char* StripLeadingWhitespace( const UTF8Char* readLine, const UTF8Char* const readEnd ) {
				for( ; readLine != readEnd && isspace( *readLine ); ++readLine ) {}

				return readLine;
			}

		// ---------------------------------------------------

			static const UTF8Char* StripTrailingWhitespace( const UTF8Char* readEnd, const UTF8Char* const readLine ) {
				for( ; readLine != readEnd && isspace( readEnd[-1] ); --readEnd ) {}

				return readEnd;
			}

		// ---------------------------------------------------

			static const UTF8Char* FindCharacterOrComment( const UTF8Char* readLine, const UTF8Char* const readEnd, const UTF8Char character ) {
				int	wasWhitespace( 0 );

				while( (readLine != readEnd) && (*readLine != character) && !(wasWhitespace && *readLine == UTF8L(';')) ) {
					wasWhitespace = isspace( *readLine++ );
				}

				return readLine;
			}
		};

	// ---

		Range<const UTF8Char*>	section( Range<const UTF8Char*>::EmptySet() );

		// Consume the BOM, if one exists.
		const UTF8Char* lineBegin( FunctionHelper::StripByteOrderMark( sourceData.first, sourceData.onePastLast ) );

		// Scan through each line in the range.
		for( const UTF8Char* readEnd( Utility::FindElement( lineBegin, sourceData.onePastLast, UTF8L('\n') ) ); sourceData.Contains( lineBegin ); (lineBegin = readEnd), readEnd = Utility::FindElement( lineBegin, sourceData.onePastLast, UTF8L('\n') ) ) {
			// Strip out leading and trailing whitespace.
			const UTF8Char* const	lineEnd( FunctionHelper::StripTrailingWhitespace( readEnd, (lineBegin = FunctionHelper::StripLeadingWhitespace( lineBegin, readEnd )) ) );

			if( lineBegin == lineEnd ) {
				// Empty/whitespace-only line.

			} else if( *lineBegin == UTF8L(';') ) {
				// Comment line. Skip and move on.

			} else if( *lineBegin == UTF8L('[') ) {
				// Section designator.

				// Advance the line begin by one element to consume the open bracket token.
				++lineBegin;

				// While a comment may be on the same line as a section declaration, it is not allowed to exist within the brackets...
				const UTF8Char* const	sectionEnd( FunctionHelper::FindCharacterOrComment( lineBegin, lineEnd, UTF8L(']') ) );

				// ... which we confirm.
				if( (sectionEnd != lineEnd) && (*sectionEnd == UTF8L(']')) ) {
					section = Range<const UTF8Char*>( FunctionHelper::StripLeadingWhitespace( lineBegin, sectionEnd ), FunctionHelper::StripTrailingWhitespace( sectionEnd, lineBegin ) );
				}
			} else {
				// <name> = <value> pair

				// Find the separation position between the name and value
				const UTF8Char* const	separatorPosition( FunctionHelper::FindCharacterOrComment( lineBegin, lineEnd, UTF8L('=') ) );

				// Ensure that any comments come after the assignment.
				if( (separatorPosition != lineEnd) && (*separatorPosition == UTF8L('=')) ) {
					Range<const UTF8Char*>	name( FunctionHelper::StripLeadingWhitespace( lineBegin, separatorPosition ), FunctionHelper::StripTrailingWhitespace( separatorPosition, lineBegin ) );

					if( !name.IsEmpty() ) {
						Range<const UTF8Char*>	value( FunctionHelper::StripLeadingWhitespace( separatorPosition + 1, lineEnd ), FunctionHelper::FindCharacterOrComment( separatorPosition + 1, lineEnd, UTF8L('\n') ) );
					
						// If there is a comment after the assignment, make sure we strip it out
						if( !value.IsEmpty() && *value.onePastLast == UTF8L(';') ) {
							value.onePastLast = FunctionHelper::StripTrailingWhitespace( value.onePastLast - 1, value.first );
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