
#include <iostream>
#include <string>

#include "ColumnConfigFile.h"

using std::string;
using std::cout;
using std::cerr;
using std::endl;


void usage();

void usage()
{
    cerr << "\nUsage: col_demo <infile-name>\n" << endl;
    exit( 1 );
}


int main( int argc, char *argv[] )
{
    if ( argc != 2 )
        usage();

    string filename = argv[1];

    ColumnConfigFile file;
    file.read( filename );

    // Values that are reasonable for /etc/fstab

    int n=0;
    file.set_max_column_width( n++, 45 ); // device; just enough for UUID=...
    file.set_max_column_width( n++, 15 ); // mount point
    file.set_max_column_width( n++,  6 ); // fs type
    file.set_max_column_width( n++, 30 ); // mount options
    file.set_max_column_width( n++,  1 ); // dump priority
    file.set_max_column_width( n++,  1 ); // fsck pass

    // file.set_pad_columns( false );

    string_vec lines = file.format_lines();

    for ( size_t i=0; i < lines.size(); ++i )
        cout << lines[i] << endl;
}
