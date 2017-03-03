/**
 * CommentedConfigFile.cc
 *
 * Author:  Stefan Hundhammer <Stefan.Hundhammer@gmx.de>
 * License: GPL V2 - see file LICENSE for details
 **/


#include <fstream>
#include <iostream>
#include <algorithm>
#include <boost/algorithm/string.hpp>

#include "CommentedConfigFile.h"

using std::ifstream;
using std::cout;
using std::endl;

#define WHITESPACE " \t"


CommentedConfigFile::CommentedConfigFile():
    comment_marker( "#" )
{
}


CommentedConfigFile::~CommentedConfigFile()
{
    clear_entries();
}


CommentedConfigFile::Entry * CommentedConfigFile::entry( int index )
{
    if ( index < 0 || index >= (int) entries.size() )
        return 0;
    else
        return entries[ index ];
}


int CommentedConfigFile::index_of( const Entry * wanted_entry )
{
    for ( size_t i=0; i < entries.size(); ++i )
    {
	if ( entries[i] == wanted_entry )
	    return i;
    }

    return -1;
}


CommentedConfigFile::Entry * CommentedConfigFile::take( int index )
{
    Entry * entry = entries[ index ];
    entries.erase( entries.begin() + index );

    return entry;
}


void CommentedConfigFile::remove( int index )
{
    Entry * entry = take( index );
    delete entry;
}


void CommentedConfigFile::insert( int before, Entry * entry )
{
    entries.insert( entries.begin() + before, entry );
}


void CommentedConfigFile::append( Entry * entry )
{
    entries.push_back( entry );
}


bool CommentedConfigFile::read( const string & filename )
{
    this->filename = filename;

    if ( filename.empty() )
        return false;

    string     line;
    string_vec lines;
    ifstream   file( filename );

    while ( std::getline( file, line ) )
        lines.push_back( line );

    bool success = parse( lines );

    return success;
}


bool CommentedConfigFile::write( const string & new_filename )
{
    string name = new_filename.empty() ? this->filename : new_filename;

    return true; // FIXME
}


bool CommentedConfigFile::parse( const string_vec & lines )
{
    clear_all();

    int header_end    = find_header_comment_end( lines );
    int footer_start  = find_footer_comment_start( lines, header_end + 1 );
    int content_start = 0;
    int content_end   = lines.size() - 1;

    if ( header_end > -1 )
    {
        for ( int i=0; i <= header_end; ++i )
            header_comments.push_back( lines[i] );

        content_start = header_end + 1;
    }

    if ( footer_start > -1 )
    {
        for ( size_t i = footer_start; i < lines.size(); ++i )
            footer_comments.push_back( lines[i] );

        content_end = footer_start - 1;
    }

    bool success = parse_entries( lines, content_start, content_end );

    return success;
}


bool CommentedConfigFile::parse_entries( const string_vec & lines, int from, int end )
{
    string_vec comment_before;

    for ( int i = from; i <= end; ++i )
    {
        const string & line = lines[i];

        if ( is_empty_line( line ) || is_comment_line( line ) )
            comment_before.push_back( line );
        else // found a content line
        {
            CommentedConfigFile::Entry * entry = create_entry();

            if ( ! entry )
                throw std::runtime_error( "CommentedConfigFile::create_entry() returned NULL" );

            entry->comment_before = comment_before;
            comment_before.clear();
            string content;
            split_off_comment( line, content, entry->line_comment );
            append( entry );
            bool success = entry->parse( content );

            if ( ! success )
                return false;
        }
    }

    return true;
}


int CommentedConfigFile::find_header_comment_end( const string_vec & lines )
{
    int header_end      = -1;
    int last_empty_line = -1;

    for ( int i=0; i < (int) lines.size(); ++i )
    {
        const string & line = lines[i];

        if ( is_empty_line( line ) )
            last_empty_line = i;
        else if ( is_comment_line( line ) )
            header_end = i;
        else // found the first content line
            break;
    }

    if ( last_empty_line > 0 )
    {
        // This covers two cases:
        //
        // - If there were empty lines and no more comment lines before the
        //   first content line, the empty lines belong to the header comment.
        //
        // - If there were empty lines and then some more comment lines before
        //   the first content line, the comments after the last empty line no
        //   longer belong to the header comment, but to the first content
        //   entry. So let's go back to that last empty line.

        header_end = last_empty_line;
    }

    return header_end;
}


int CommentedConfigFile::find_footer_comment_start( const string_vec & lines, int from )
{
    int footer_start = -1;

    for ( int i = lines.size()-1; i >= from; --i )
    {
        const string & line = lines[i];

        if ( is_empty_line( line ) || is_comment_line( line ) )
            footer_start = i;
        else // found the last content line
            break;
    }

    return footer_start;
}


string_vec CommentedConfigFile::format_lines() const
{
    string_vec lines;

    // TO DO
    // TO DO
    // TO DO

    return lines;
}


void CommentedConfigFile::clear_entries()
{
    for ( size_t i=0; i < entries.size(); ++i )
	delete entries[i];

    entries.clear();
}


void CommentedConfigFile::clear_all()
{
    clear_entries();
    header_comments.clear();
    footer_comments.clear();
}


bool CommentedConfigFile::is_comment_line( const string & line )
{
    size_t pos = line.find_first_not_of( WHITESPACE );

    if ( pos == string::npos ) // No non-whitespace character in line
        return false;

    if ( pos == 0 )
        return boost::starts_with( line, comment_marker );
    else
        return boost::starts_with( line.substr( pos ), comment_marker );
}


bool CommentedConfigFile::is_empty_line( const string & line )
{
    if ( line.empty() )
        return true;

    return line.find_first_not_of( WHITESPACE ) == string::npos;
}


void CommentedConfigFile::split_off_comment( const string & line,
					     string & content_ret,
					     string & comment_ret )
{
    size_t pos = line.find( comment_marker );

    if ( pos == string::npos )
    {
        comment_ret = "";
        content_ret = line;
    }
    else
    {
        content_ret = line.substr( 0, pos-1 );
        comment_ret = line.substr( pos );
    }

    strip_trailing_whitespace( content_ret );
    strip_trailing_whitespace( comment_ret );
}


void CommentedConfigFile::strip_trailing_whitespace( string & line )
{
    size_t pos = line.find_last_not_of( WHITESPACE );

    if ( pos != string::npos )
        line = line.substr( 0,  pos+1 );
    else
        line.clear();
}
