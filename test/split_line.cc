
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE commented-config-file

#include <boost/test/unit_test.hpp>

#define protected public
#define private   public
#include "CommentedConfigFile.h"


void test_split( const string & line,
                 const string & expected_content,
                 const string & expected_comment )
{
    static CommentedConfigFile subject;

    string content;
    string comment;

    subject.split_off_comment( line, content, comment );

    BOOST_CHECK_EQUAL( content, expected_content );
    BOOST_CHECK_EQUAL( comment, expected_comment );
}


BOOST_AUTO_TEST_CASE( split_line )
{
    test_split( "content # comment"    , "content", "# comment" );
    test_split( "content   # comment  ", "content", "# comment" );
    test_split( "content   #"          , "content", "#"         );
    test_split( "content  "            , "content", ""          );
    test_split( "    # comment\t"      , ""       , "# comment" );
    test_split( ""                     , ""       , ""          );
}

