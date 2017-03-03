/**
 * ColumnConfigFile.h
 *
 * Author:  Stefan Hundhammer <Stefan.Hundhammer@gmx.de>
 * License: GPL V2 - see file LICENSE for details
 **/


#ifndef ColumnConfigFile_h
#define ColumnConfigFile_h

#include "CommentedConfigFile.h"

/**
 * Utility class to read and write column-oriented config files that might
 * contain comments that should be preserved when writing the file.
 *
 * One example for this would be /etc/fstab:
 *  
 *    # /etc/fstab
 *    #
 *    # <file system>   <mount point>  <type> <options>     <dump> <pass>
 *    
 *    /dev/disk/by-label/swap    none   swap  sw                 0  0
 *    /dev/disk/by-label/Ubuntu  /      ext4  errors=remount-ro  0  1
 *    /dev/disk/by-label/work    /work  ext4  defaults           0  2
 *
 * There are 6 columns, separated by whitespace. This class is a refinement of
 * the more generic CommentedConfigFile class to handle such cases.
 **/
class ColumnConfigFile: public CommentedConfigFile
{
public:
    
    class Entry: public CommentedConfigFile::Entry
    {
    public:
	Entry() {}
	virtual ~Entry() {}

	/**
	 * Format the content as a string.
         *
	 * Reimplemented from CommentedConfigFile.
	 **/
	virtual string format();

	/**
	 * Parse a content line. Return 'true' on success, 'false' on error.
         *
	 * Reimplemented from CommentedConfigFile.
	 **/
	virtual bool parse( const string & line );

        /**
         * Return the number of columns for this entry.
         **/
        int column_count() const { return col.size(); }

        /**
         * Return one of the columns for this entry.
         **/
        string column( int i ) const { return col[i]; }

        /**
         * Set a new value for column no. 'i'.
         **/
        void set_column( int i, const string & new_value )
            { col[i] = new_value; }

        void add_column( const string & new_value )
            { col.push_back( new_value ); }

    protected:

        /**
         * Split a line into fields and return them.
         **/
        virtual string_vec split( const string & line ) const;

    private:
        
        string_vec col;
    };

    
    ColumnConfigFile();
    virtual ~ColumnConfigFile();

    virtual Entry * create_entry() { return new ColumnConfigFile::Entry(); }
};

#endif // ColumnConfigFile_h
