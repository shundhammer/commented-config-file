
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE commented-config-file

#include <boost/test/unit_test.hpp>
#include <iostream>

#define protected public
#define private   public
#include "CommentedConfigFile.h"


BOOST_AUTO_TEST_CASE( formatter_with_entry_comments )
{
    string_vec input = {
        "# header 00",
        "# header 01",
        "",
        "# header 02",
        "",
        "# entry 00 comment 00",
        "# entry 00 comment 01",
        "entry 00 content",
        "# entry 01 comment 00",
        "entry 01 content # entry 01 line comment",
        "",
        "# entry 02 comment 00",
        "# entry 02 comment 01  ",
        "# entry 02 comment 02",
        "entry 02 content",
        "entry 03 content",
        "entry 04 content",
        "",
        "# footer 00",
        "# footer 01"
    };

    CommentedConfigFile subject;
    subject.parse( input );
    string_vec output = subject.format_lines();

    BOOST_CHECK_EQUAL( input.size(), output.size() );
    
    for ( size_t i=0; i < input.size(); ++i )
        BOOST_CHECK_EQUAL( input[i], output[i] );
}

