/**
 * ColumnConfigFile.h
 *
 * Author:  Stefan Hundhammer <Stefan.Hundhammer@gmx.de>
 * License: GPL V2 - see file LICENSE for details
 **/

#include <iostream>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include "ColumnConfigFile.h"


#define WHITESPACE                      " \t"
#define DEFAULT_MAX_COLUMN_WIDTH        40


string ColumnConfigFile::Entry::format()
{
    string result;

    for ( size_t i=0; i < columns.size(); ++i )
    {
        if ( ! result.empty() )
            result += "  ";

        string col = columns[i];

        if ( parent )
        {
            ColumnConfigFile * col_parent = dynamic_cast<ColumnConfigFile *>( parent );

            if ( col_parent )
            {
                size_t field_width = col_parent->get_column_width( i );

                if ( col.size() < field_width ) // Pad to desired width
                    col += string( field_width - col.size(), ' ' );
            }
        }

        result += col;
    }

    return result;
}


bool ColumnConfigFile::Entry::parse( const string & line )
{
    content = line;
    columns = split( line );

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
    CommentedConfigFile(),
    max_column_width( DEFAULT_MAX_COLUMN_WIDTH )
{

}


ColumnConfigFile::~ColumnConfigFile()
{

}


string_vec ColumnConfigFile::format_lines()
{
    calc_column_widths();

    return CommentedConfigFile::format_lines();
}


int ColumnConfigFile::get_column_width( int column )
{
    if ( column_widths.empty() )
        calc_column_widths();

    if ( column >= (int) column_widths.size() )
        return 0;
    else
        return column_widths[ column ];
}


int ColumnConfigFile::get_max_column_width( int column )
{
    if ( column >= (int) max_column_widths.size() )
        return max_column_width;
    else
        return max_column_widths[ column ];
}


void ColumnConfigFile::set_max_column_width( int column, int new_size )
{
    if ( column >= (int) max_column_widths.size() )
        max_column_widths.resize( column+1 );

    max_column_widths[ column ] = new_size;
}


void ColumnConfigFile::calc_column_widths()
{
    int columns = 0;

    for ( int i=0; i < get_entry_count(); ++i )
    {
        ColumnConfigFile::Entry * entry =
            dynamic_cast<ColumnConfigFile::Entry*>( get_entry( i ) );

        if ( entry )
            columns = std::max( entry->get_column_count(), columns );
    }

    column_widths.clear();
    column_widths.resize( columns );

    for ( int i=0; i < get_entry_count(); ++i )
    {
        ColumnConfigFile::Entry * entry =
            dynamic_cast<ColumnConfigFile::Entry*>( get_entry( i ) );

        if ( entry )
        {
            for ( int col=0; col < columns; ++col )
            {
                int width = entry->get_column( col ).size();
                int max   = get_max_column_width( col );

                if ( max > 0 && width > max )
                    width = max;

                column_widths[ col ] = std::max( column_widths[ col ], width );
            }
        }
    }

    for ( int col=0; col < columns; ++col )
        std::cout << "Col " << col << " width: " << column_widths[col] << std::endl;
}
