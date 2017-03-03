
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE commented-config-file

#include <boost/test/unit_test.hpp>

#define protected public
#define private   public
#include "CommentedConfigFile.h"



BOOST_AUTO_TEST_CASE( comment_lines )
{
    CommentedConfigFile subject;
    
    BOOST_CHECK_EQUAL( subject.is_comment_line( "# Text" ), true );
    BOOST_CHECK_EQUAL( subject.is_comment_line( "#"      ), true );
    BOOST_CHECK_EQUAL( subject.is_comment_line( " #"     ), true );
    BOOST_CHECK_EQUAL( subject.is_comment_line( " \t#"   ), true );
    BOOST_CHECK_EQUAL( subject.is_comment_line( "###"    ), true );
    
    BOOST_CHECK_EQUAL( subject.is_comment_line( "content # comment" ), false);
    
    BOOST_CHECK_EQUAL( subject.is_comment_line( ""       ), false );
    BOOST_CHECK_EQUAL( subject.is_comment_line( "   "    ), false );
    BOOST_CHECK_EQUAL( subject.is_comment_line( "\t\t"   ), false );
    BOOST_CHECK_EQUAL( subject.is_comment_line( " \t \t" ), false );
}

