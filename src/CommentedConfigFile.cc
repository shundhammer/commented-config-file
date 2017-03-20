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

#define WHITESPACE " \t"

using std::cout;
using std::endl;


CommentedConfigFile::CommentedConfigFile():
    comment_marker( "#" ),
    diff_enabled( false )
{
}


CommentedConfigFile::~CommentedConfigFile()
{
    clear_entries();
}


CommentedConfigFile::Entry * CommentedConfigFile::get_entry( int index ) const
{
    if ( index < 0 || index >= (int) entries.size() )
        return 0;
    else
        return entries[ index ];
}


int CommentedConfigFile::get_index_of( const Entry * wanted_entry ) const
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
    if ( index < 0 || index >= (int) entries.size() )
        return 0;

    Entry * entry = entries[ index ];
    entries.erase( entries.begin() + index );
    entry->set_parent( 0 );

    return entry;
}


void CommentedConfigFile::remove( int index )
{
    Entry * entry = take( index );

    if ( entry )
        delete entry;
}


void CommentedConfigFile::remove( Entry * entry )
{
    if ( ! entry )
        return;

    int index = get_index_of( entry );

    if ( index != -1 )
        remove( index );
}


void CommentedConfigFile::insert( int before, Entry * entry )
{
    entries.insert( entries.begin() + before, entry );
    entry->set_parent( this );
}


void CommentedConfigFile::append( Entry * entry )
{
    entries.push_back( entry );
    entry->set_parent( this );
}


bool CommentedConfigFile::read( const string & filename )
{
    this->filename = filename;

    if ( filename.empty() )
        return false;

    string     line;
    string_vec lines;
    std::ifstream file( filename );

    while ( std::getline( file, line ) )
        lines.push_back( line );

    bool success = parse( lines );

    return success;
}


bool CommentedConfigFile::write( const string & new_filename )
{
    string name = new_filename;

    if ( new_filename.empty() )
        name = this->filename;
    else
        this->filename = name;

    if ( name.empty() ) // Support for mocking:
        return true;    // Pretend everything worked just fine.

    std::ofstream file( name, std::ofstream::out | std::ofstream::trunc );

    if ( ! file.is_open() )
        return false;

    string_vec lines = format_lines();

    for ( size_t i=0; i < lines.size(); ++i )
        file << lines[i] << "\n"; // no endl: Don't flush after every line

    return true;
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

    if ( diff_enabled )
        save_orig();

    return success;
}


bool CommentedConfigFile::parse_entries( const string_vec & lines,
                                         int from,
                                         int end )
{
    string_vec comment_before;
    bool success = true;

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

            entry->set_comment_before( comment_before );
            comment_before.clear();
            string content;
            string line_comment;
            split_off_comment( line, content, line_comment );
            entry->set_line_comment( line_comment );
            bool ok = entry->parse( content, i+1 );

            if ( ok )
                append( entry );
            else
            {
                success = false;
                delete entry;
            }
        }
    }

    return success;
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
        header_end = last_empty_line;

        // This covers two cases:
        //
        // - If there were empty lines and no more comment lines before the
        //   first content line, the empty lines belong to the header comment.
        //
        // - If there were empty lines and then some more comment lines before
        //   the first content line, the comments after the last empty line no
        //   longer belong to the header comment, but to the first content
        //   entry. So let's go back to that last empty line.
    }

    return header_end;
}


int CommentedConfigFile::find_footer_comment_start( const string_vec & lines,
                                                    int from )
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


string_vec CommentedConfigFile::format_lines()
{
    string_vec lines = header_comments;

    for ( size_t i=0; i < entries.size(); ++i )
    {
        Entry * entry = entries[i];

        for ( size_t j=0; j < entry->get_comment_before().size(); ++j )
            lines.push_back( entry->get_comment_before()[j] );

        string line = entry->format();

        if ( ! entry->get_line_comment().empty() )
            line += " " + entry->get_line_comment();

        lines.push_back( line );
    }

    for ( size_t i=0; i < footer_comments.size(); ++i )
        lines.push_back( footer_comments[i] );

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
}


void CommentedConfigFile::strip_trailing_whitespace( string & line )
{
    size_t pos = line.find_last_not_of( WHITESPACE );

    if ( pos != string::npos )
        line = line.substr( 0,  pos+1 );
    else
        line.clear();
}


string_vec CommentedConfigFile::diff()
{
    return diff( orig_lines, format_lines() );;
}


string_vec CommentedConfigFile::diff( const string_vec & formatted_lines )
{
    return diff( orig_lines, formatted_lines );
}


void CommentedConfigFile::save_orig()
{
    save_orig( format_lines() );
}


void CommentedConfigFile::save_orig( const string_vec & new_orig_lines )
{
    orig_lines = new_orig_lines;
}


string_vec CommentedConfigFile::diff( const string_vec & old_lines,
                                      const string_vec & new_lines )
{
    string_vec diff_lines;

    int old_start = 0;
    int old_end   = old_lines.size() - 1;

    int new_start = 0;
    int new_end   = new_lines.size() - 1;


    // while ( old_start < old_end || new_start < new_end )
    {
        // Skip common lines at the start

        while ( old_start <= old_end && new_start <= new_end &&
                old_lines[ old_start ] == new_lines[ new_start ] )
        {
            // cout << "Skipping common start: " << old_lines[ old_start ] << endl;
            ++old_start;
            ++new_start;
        }

        // Skip common lines at the end

        while ( old_end > old_start && new_end > new_start &&
                old_lines[ old_end ] == new_lines[ new_end ] )
        {
            // cout << "Skipping common end  : " << old_lines[ old_end ] << endl;
            --old_end;
            --new_end;
        }


        while ( old_start <= old_end )
            diff_lines.push_back( string( "-" ) + old_lines[ old_start++ ] );

        while ( new_start <= new_end )
            diff_lines.push_back( string( "+" ) + new_lines[ new_start++ ] );

#if 0
        // Check if there is a diff at all

        if ( old_start < old_end || new_start < new_end )
        {
            // cout << "We have a diff" << endl;


            const string & old_line = old_lines[ old_start ];
            int moved_pos = -1;

            // Try to find the old line at another position in the new lines

            for ( int i = new_start; i < new_end && moved_pos == -1; ++i )
            {
                if ( new_lines[i] == old_line )
                    moved_pos = i;
            }

            if ( moved_pos == -1 ) // The old line was deleted
            {
                cout << "Deleted: " << old_line << endl;
                result.push_back( string( "-" ) + old_line );
                ++old_start;
            }
            else
            {
                int inserted_lines = moved_pos - new_start;

                for ( int i=0; i < inserted_lines; ++i )
                    result.push_back( string( "+" ) + new_lines[ new_start + i ] );

                new_start += inserted_lines;
            }
        }
#endif
    }

    return diff_lines;
}
