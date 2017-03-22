
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE diff

#include <iostream>
#include <iomanip>
#include <sstream>
#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string.hpp>

#include "Diff.h"

using std::cout;
using std::endl;
using std::ostream;
using std::stringstream;


ostream & operator<<( ostream & stream, const string_vec & lines )
{
    stream << endl;

    for ( size_t i=0; i < lines.size(); ++i )
        stream << std::setfill('0') << std::setw(2) << i+1
               << ": \"" << lines[i] << "\"" << endl;

    return stream;
}


boost::test_tools::predicate_result
check_diff( const string_vec & input_a,
            const string_vec & input_b,
            const string_vec & expected,
            int                context = 0 )
{
    boost::test_tools::predicate_result result( false );
    stringstream str;

    string_vec actual = Diff::diff( input_a, input_b, context );

    if ( actual.size() != expected.size() )
    {
        str << "Actual:" << actual << "\nExpected: " << expected;

        result.message() << "Size mismatch: Actual size:"
                         << actual.size()
                         << "; Expected size: " << expected.size()
                         << "\n\n" << str.str();
        return result;
    }


    for ( size_t i = 0;
          i < expected.size() && i < actual.size();
          ++i )
    {
        if ( i == 0 && boost::starts_with( expected[i], "@@ ??" ) )
            continue;

        if ( actual[i] != expected[i] )
        {
            str << "Actual:" << actual << "\nExpected: " << expected;

            result.message() << "\n\nResult line #" << i+1 << " mismatch:"
                             << "\nActual   line: \"" << actual[i]   << "\""
                             << "\nExpected line: \"" << expected[i] << "\""
                             << "\n\n" << str.str();
            return result;
        }
    }

    return true;
}


BOOST_AUTO_TEST_CASE( diff_simple )
{
    string_vec input01 = {
        "aaa",
        "bbb",
        "ccc",
        "ddd"
    };

    string_vec input02 = {
        "aaa",
        "ccc",
        "ddd"
    };

    string_vec aaa = {
        "aaa"
    };

    string_vec aaa_bbb = {
        "aaa",
        "bbb"
    };
    
    string_vec empty;


    BOOST_CHECK( check_diff( input01, input02, { "@@ ???", "-bbb" } ) );
    BOOST_CHECK( check_diff( input02, input01, { "@@ ???", "+bbb" } ) );
    BOOST_CHECK( check_diff( input01, input01, {}                   ) );
    BOOST_CHECK( check_diff( {},      aaa,     { "@@ ???", "+aaa" } ) );
    BOOST_CHECK( check_diff( aaa,     {},      { "@@ ???", "-aaa" } ) );
    BOOST_CHECK( check_diff( {},      aaa_bbb, { "@@ ???", "+aaa", "+bbb" } ) );
    BOOST_CHECK( check_diff( aaa_bbb, {},      { "@@ ???", "-aaa", "-bbb" } ) );

}


