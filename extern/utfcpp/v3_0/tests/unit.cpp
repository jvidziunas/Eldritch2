#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE UTF8_CPP_UNIT
#include <boost/test/unit_test.hpp>

#include "../src/utf8.h"
using namespace std;

BOOST_AUTO_TEST_CASE(append)
{
	string s;
    BOOST_CHECK_NO_THROW (utf8::append(U'\U00000448', s));
	BOOST_CHECK (s.length() == 2 && s[0] == '\xd1' && s[1] == '\x88');

	s.erase();
	BOOST_CHECK_NO_THROW(utf8::append(U'\U000065e5', s));
	BOOST_CHECK (s.length() == 3 && s[0] == '\xe6' && s[1] == '\x97' && s[2] == '\xa5');
}


