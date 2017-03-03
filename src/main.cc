
#include <stdio.h>


int main( int argc, char *argv[] )
{
    printf( "Hello, world!\n" );

    for ( int i=1; i < argc; ++i )
        printf( "Arg #%d: %s\n", i, argv[i] );
}
