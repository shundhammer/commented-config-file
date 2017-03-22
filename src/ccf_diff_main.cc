/**
 * ccf_diff_main.cc
 *
 * This is a drop-in replacement for the 'diff -u' command.
 *
 * Author:  Stefan Hundhammer <Stefan.Hundhammer@gmx.de>
 * License: GPL V2 - see file LICENSE for details
 **/

#include <fstream>
#include <iostream>
#include <string>

#include "Diff.h"

using std::string;
using std::cout;
using std::cerr;
using std::endl;


void usage();
string_vec read_file( const string & filename );


void usage()
{
    cerr << "\nUsage: ccf_diff <file-1> <file-2>\n" << endl;
    exit( 1 );
}


string_vec read_file( const string & filename )
{
    string_vec lines;

    if ( ! filename.empty() )
    {
        string  line;
        std::ifstream file( filename );

        while ( std::getline( file, line ) )
            lines.push_back( line );
    }

    return lines;
}


int main( int argc, char *argv[] )
{
    if ( argc != 3 )
        usage();

    int context_len = 3;

    string filename1  = argv[1];
    string filename2  = argv[2];

    string_vec lines1 = read_file( filename1 );
    string_vec lines2 = read_file( filename2 );

    string_vec diff  = Diff::diff( lines1, lines2, context_len );
    string_vec patch = Diff::format_patch_header( filename1, filename2 );
    Diff::add_lines( patch, diff );

    for ( size_t i=0; i < patch.size(); ++i )
        cout << patch[i] << endl;
}
