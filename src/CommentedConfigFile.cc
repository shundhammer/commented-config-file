/**
 * CommentedConfigFile.cc
 *
 * Author:  Stefan Hundhammer <Stefan.Hundhammer@gmx.de>
 * License: GPL V2 - see file LICENSE for details
 **/


#include <fstream>
#include <iostream>
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
    delete take( index );
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
    clear_all();

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
    for ( size_t i=0; i < lines.size(); ++i )
    {
        const string & line = lines[i];

        cout << ( is_comment_line( line ) ? "C" : "." );
        cout << ( is_empty_line  ( line ) ? "E" : "." );
        cout << "  " << i << ": " << line << endl;
    }

    return true;
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

}


void CommentedConfigFile::strip_trailing_whitespace( string & line )
{

}
