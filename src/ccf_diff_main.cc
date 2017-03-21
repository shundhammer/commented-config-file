
#include <iostream>
#include <string>

#include "CommentedConfigFile.h"
#include "Diff.h"

using std::string;
using std::cout;
using std::cerr;
using std::endl;


void usage();

void usage()
{
    cerr << "\nUsage: ccf_diff <file-1> <file-2>\n" << endl;
    exit( 1 );
}


int main( int argc, char *argv[] )
{
    if ( argc != 3 )
        usage();

    CommentedConfigFile file1;
    file1.read( argv[1] );

    CommentedConfigFile file2;
    file2.read( argv[2] );

    string_vec diff = Diff::diff( file1.format_lines(),
                                  file2.format_lines(),
                                  3 );

    // cout << "Diff size: " << diff.size() << "\n" << endl;
    
    for ( size_t i=0; i < diff.size(); ++i )
    {
        cout << diff[i] << endl;
    }
}
