/**
 * CommentedConfigFile.cc
 *
 * Author:  Stefan Hundhammer <Stefan.Hundhammer@gmx.de>
 * License: GPL V2 - see file LICENSE for details
 **/


#include "CommentedConfigFile.h"


CommentedConfigFile::CommentedConfigFile( const string & filename ):
    filename( filename ),
    comment_marker( "#" )
{
    if ( ! filename.empty() )
        read( filename );
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
    this->filename = filename;

    return true; // FIXME
}


bool CommentedConfigFile::write( const string & new_filename )
{
    string name = new_filename.empty() ? this->filename : new_filename;

    // TO DO

    return true; // FIXME
}


bool CommentedConfigFile::parse( const string_vec & lines )
{
    return true; // FIXME
}


string_vec CommentedConfigFile::format_lines() const
{
    string_vec lines;

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
    return false; // FIXME
}


bool CommentedConfigFile::is_empty_line( const string & line )
{
    return false; // FIXME
}


void CommentedConfigFile::split_off_comment( const string & line,
                                             string & content_ret,
                                             string & comment_ret )
{
    
}


void CommentedConfigFile::strip_trailing_whitespace( string & line )
{
    
}
