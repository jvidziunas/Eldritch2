#include "../../../source/utf8.h"

#include <sstream>
#include <iterator>

// [ 3414834 ] Testing unchecked::iterator< istreambuf_iterator< char > >
void id_3414834()
{
	typedef std::istreambuf_iterator< char > char_iterator;
	typedef utf8::unchecked::iterator< char_iterator > utf8_iterator;

	std::string input = "The rain in Spain falls mainly on the plain.";
	//std::cout << input << std::endl;
	
	std::string output;
	{
		std::vector< uint32_t > output_characters;
		{
			std::stringstream input_stream( input );
			utf8_iterator const input_terminator;
			for( 
				utf8_iterator input_iterator( input_stream );
				input_iterator != input_terminator;
				++input_iterator
			) {
				output_characters.push_back( *input_iterator );
			}
		}
		
		utf8::utf32to8( output_characters.begin(), output_characters.end(),
			std::back_inserter( output )
		);
	}
	//std::cout << output << std::endl;

	check(output == input);
}

