
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

    string_vec lines = file.format_lines();

    for ( size_t i=0; i < lines.size(); ++i )
        cout << lines[i] << endl;
}
