
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

    cout << "<Header>" << endl;

    for ( size_t i=0; i < file.get_header_comments().size(); ++i )
        cout << "  " << i+1 << ": " << file.get_header_comments()[i] << endl;

    cout << "</Header>" << endl;
    cout << "\n<Content>" << endl;

    for ( int i=0; i < file.get_entry_count(); ++i )
    {
        CommentedConfigFile::Entry * entry = file.get_entry( i );

        if ( ! entry->get_comment_before().empty() )
            cout << endl;

        for ( size_t j=0; j < entry->get_comment_before().size(); ++j )
        {
            cout << "  Entry #" << i+1
                 << " comment " << j+1
                 << ": " << entry->get_comment_before()[j]
                 << endl;
        }

        cout << "  Entry #" << i+1 << " content  : " << entry->get_content() << endl;

        if ( ! entry->get_line_comment().empty() )
            cout << "  Entry #" << i+1 << " line comment: " << entry->get_line_comment() << endl;
    }

    cout << "</Content>\n" << endl;
    cout << "<Footer>" << endl;

    for ( size_t i=0; i < file.get_footer_comments().size(); ++i )
        cout << "  " << i+1 << ": " << file.get_footer_comments()[i] << endl;

    cout << "</Footer>" << endl;
}
