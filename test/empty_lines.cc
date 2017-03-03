
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE commented-config-file

#include <boost/test/unit_test.hpp>

#define protected public
#define private   public
#include "CommentedConfigFile.h"



BOOST_AUTO_TEST_CASE( empty_lines )
{
    CommentedConfigFile subject;
    
    BOOST_CHECK_EQUAL( subject.is_empty_line( ""       ), true );
    BOOST_CHECK_EQUAL( subject.is_empty_line( "   "    ), true );
    BOOST_CHECK_EQUAL( subject.is_empty_line( "\t\t"   ), true );
    BOOST_CHECK_EQUAL( subject.is_empty_line( " \t \t" ), true );

    BOOST_CHECK_EQUAL( subject.is_empty_line( " \t \t#" ), false );
    BOOST_CHECK_EQUAL( subject.is_empty_line( " x"      ), false );
    BOOST_CHECK_EQUAL( subject.is_empty_line( "nothing" ), false );
}

