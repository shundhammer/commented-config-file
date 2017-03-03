/**
 * ColumnConfigFile.h
 *
 * Author:  Stefan Hundhammer <Stefan.Hundhammer@gmx.de>
 * License: GPL V2 - see file LICENSE for details
 **/

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include "ColumnConfigFile.h"


#define WHITESPACE " \t"


string ColumnConfigFile::Entry::format()
{
    string result;

    for ( size_t i=0; i < col.size(); ++i )
    {
        if ( ! result.empty() )
            result += "  ";

        result += col[i];
    }

    return result;
}


bool ColumnConfigFile::Entry::parse( const string & line )
{
    content = line;
    col = split( line );

    return true;
}


string_vec ColumnConfigFile::Entry::split( const string & line ) const
{
    string_vec fields;

    boost::split( fields,
                  line,
                  boost::is_any_of( WHITESPACE ),
                  boost::token_compress_on );
    
    return fields;
}




ColumnConfigFile::ColumnConfigFile():
    CommentedConfigFile()
{

}


ColumnConfigFile::~ColumnConfigFile()
{

}
