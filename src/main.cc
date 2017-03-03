
#include <iostream>
#include <string>

#include "CommentedConfigFile.h"

using std::string;
using std::cout;
using std::cerr;
using std::endl;


void usage();

void usage()
{
    cerr << "\nUsage: ccf_demo <infile-name>\n" << endl;
    exit( 1 );
}


int main( int argc, char *argv[] )
{
    if ( argc != 2 )
        usage();

    string filename = argv[1];

    CommentedConfigFile file;
    file.read( filename );
}
