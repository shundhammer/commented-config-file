
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE commented-config-file

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <fstream>
#include <stdio.h>

#define protected public
#define private   public
#include "CommentedConfigFile.h"


BOOST_AUTO_TEST_CASE( container_operations )
{
    CommentedConfigFile::Entry * entry_a = new CommentedConfigFile::Entry();
    CommentedConfigFile::Entry * entry_b = new CommentedConfigFile::Entry();
    CommentedConfigFile::Entry * entry_c = new CommentedConfigFile::Entry();
    CommentedConfigFile::Entry * entry_d = new CommentedConfigFile::Entry();
    CommentedConfigFile::Entry * entry_e = new CommentedConfigFile::Entry();
    CommentedConfigFile::Entry * entry   = 0;

    entry_a->content = "Entry A";
    entry_b->content = "Entry B";
    entry_c->content = "Entry C";
    entry_d->content = "Entry D";
    entry_e->content = "Entry E";

    CommentedConfigFile subject;

    BOOST_CHECK_EQUAL( subject.empty(), true );

    subject << entry_b << entry_d << entry_e;

    BOOST_CHECK_EQUAL( subject.entry_count(), 3 );
    BOOST_CHECK_EQUAL( subject.entry(0), entry_b );
    BOOST_CHECK_EQUAL( subject.entry(1), entry_d );
    BOOST_CHECK_EQUAL( subject.entry(2), entry_e );

    subject.insert( 1, entry_c );
    
    BOOST_CHECK_EQUAL( subject.entry_count(), 4 );
    BOOST_CHECK_EQUAL( subject.entry(0), entry_b );
    BOOST_CHECK_EQUAL( subject.entry(1), entry_c );
    BOOST_CHECK_EQUAL( subject.entry(2), entry_d );
    BOOST_CHECK_EQUAL( subject.entry(3), entry_e );
    
    subject.insert( 0, entry_a );
    
    BOOST_CHECK_EQUAL( subject.entry_count(), 5 );
    BOOST_CHECK_EQUAL( subject.entry(0), entry_a );
    BOOST_CHECK_EQUAL( subject.entry(1), entry_b );
    BOOST_CHECK_EQUAL( subject.entry(2), entry_c );
    BOOST_CHECK_EQUAL( subject.entry(3), entry_d );
    BOOST_CHECK_EQUAL( subject.entry(4), entry_e );

    entry = subject.take( 3 );
    
    BOOST_CHECK_EQUAL( entry, entry_d );
    
    BOOST_CHECK_EQUAL( subject.entry_count(), 4 );
    BOOST_CHECK_EQUAL( subject.entry(0), entry_a );
    BOOST_CHECK_EQUAL( subject.entry(1), entry_b );
    BOOST_CHECK_EQUAL( subject.entry(2), entry_c );
    BOOST_CHECK_EQUAL( subject.entry(3), entry_e );

    BOOST_CHECK_EQUAL( subject.index_of( entry_c),  2 );
    BOOST_CHECK_EQUAL( subject.index_of( entry_d), -1 );
    BOOST_CHECK_EQUAL( subject.take(9), (void *) 0 );

    subject.clear_entries();
    
    BOOST_CHECK_EQUAL( subject.empty(), true );
    BOOST_CHECK_EQUAL( subject.entry_count(), 0 );
}
