
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE commented-config-file

#include <boost/test/unit_test.hpp>

#define protected public
#define private   public
#include "CommentedConfigFile.h"


string strip( string line )
{
    static CommentedConfigFile subject;

    subject.strip_trailing_whitespace( line );
    return line;
}


BOOST_AUTO_TEST_CASE( strip_trailing_whitespace )
{
    BOOST_CHECK_EQUAL( strip( "foo bar  "    ) , string( "foo bar"   ) );
    BOOST_CHECK_EQUAL( strip( "foo bar"      ) , string( "foo bar"   ) );
    BOOST_CHECK_EQUAL( strip( "  foo bar  "  ) , string( "  foo bar" ) );
    BOOST_CHECK_EQUAL( strip( "foo bar\t"    ) , string( "foo bar"   ) );
    BOOST_CHECK_EQUAL( strip( "foo bar \t "  ) , string( "foo bar"   ) );
    BOOST_CHECK_EQUAL( strip( "   "          ) , string( ""          ) );
    BOOST_CHECK_EQUAL( strip( ""             ) , string( ""          ) );
}
