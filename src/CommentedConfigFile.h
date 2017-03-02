
#ifndef CommentedConfigFile_h
#define CommentedConfigFile_h

#include <string>
#include <vector>

using std::string;
using std::vector;

typedef vector<string> string_vec;


/**
 * Utility class to read and write config files that might contain comments.
 * This class tries to preserve any existing comments and keep them together
 * with the content line immediately following them.
 *
 * This class supports the notion of a header comment block, a footer comment
 * block, a comment block preceding any content line and a line comment on the
 * content line itself.
 *
 * A comment preceding a content line is stored together with the content line,
 * so moving around entries in the file will keep the comment with the content
 * line it belongs to.
 *
 * The default comment marker is '#' like in most Linux config files, but it
 * can be set with setCommentMarker().
 *
 * Example (line numbers added for easier reference):
 *
 *   001    # Header comment 1
 *   002    # Header comment 2
 *   003    # Header comment 3
 *   004
 *   005
 *   006    # Header comment 4
 *   007    # Header comment 5
 *   008
 *   009    # Content line 1 comment 1
 *   010    # Content line 1 comment 2
 *   011    content line 1
 *   012    content line 2
 *   013
 *   014    content line 3
 *   015
 *   016    content line 4
 *   017    content line 5 # Line comment 5
 *   018    # Content line 6 comment 1
 *   019
 *   020    content line 6 # Line comment 6
 *   021    content line 7
 *   022
 *   023    # Footer comment 1
 *   024    # Footer comment 2
 *   025
 *   026    # Footer comment 3
 *
 *
 * Empty lines or lines that have only whitespace belong to the next comment
 * block: The footer comment consists of lines 022..026.
 *
 * The only exception is the header comment that stretches from the start of
 * the file to the last empty line preceding a content line. This is what
 * separates the header comment from the comment that belongs to the first
 * content line. In this example, the header comment consists of lines
 * 001..008.
 *
 * Content line 1 in line 011 has comments 009..010.
 * Content line 2 in line 012 has no comment.
 * Content line 3 in line 014 has comment 013 (an empty line).
 * Content line 5 in line 017 has a line comment "# Line comment 5".
 * Content line 6 in line 020 has comments 018..019 and a line comment.
 *
 * Applications using this class can largely just ignore all the comment stuff;
 * the class will handle the comments automagically.
 **/
class CommentedConfigFile
{
public:

    /**
     * Class representing one content line and the preceding comments.
     *
     * This class is meant to be subclassed. When you do that, remember to also
     * overwrite the CommentedConfigFile::createEntry() factory method.
     **/
    class Entry
    {
    public:
        /**
         * Constructor. Not accepting a 'line' argument because that would need
         * to be parsed, and that would require calling the overloaded virtual
         * parse() function which is not possible in the constructor.
         **/
        Entry();

        /**
         * Format the content as a string.
         * Derived classes might choose to override this.
         * Do not add 'line_comment'; it is added automatically.
         **/
        virtual string format() { return content; }

        /**
         * Parse a content line. Return 'true' on success, 'false' on error.
         * Derived classes might choose to override this.
         **/
        virtual bool parse( const string & line ) { content = line; return true; }

        //
        // Data members
        //

        string_vec comment_before;
        string     line_comment;   // at the end of the line
        string     content;
    };

    /**
     * Constructor. If 'filename' is non-empty, this will parse the content of
     * the specified file.
     **/
    CommentedConfigFile( const string & filename = "" ):
        filename( filename ),
        comment_marker( "#" )
    {
        if ( ! filename.empty() )
            read( filename );
    }

    /**
     * Destructor. This deletes all entries.
     **/
    virtual ~CommentedConfigFile() { clear_entries(); }


    /**
     * Read 'filename' and replace the current content with it.
     * Return 'true' if success, 'false' if error.
     **/
    bool read( const string & filename );

    /**
     * Write the contents to 'filename' or, if 'filename' is empty, to the
     * original file that was used in the constructor or during the last
     * read().
     * Return 'true' if success, 'false' if error.
     **/
    bool write( const string & filename = "" );

    /**
     * Parse 'lines' and replace the current content with it.
     **/
    bool parse( const string_vec & lines );

    /**
     * Format the entire file as string lines, including header, footer and all
     * other comments.
     **/
    string_vec format_lines() const;

    /**
     * Factory method to create one entry.
     *
     * Derived classes can override this to create their own derived entry
     * class. Ownership of the created object is transferred to this class
     * which will delete it as appropriate.
     **/
    virtual Entry * create_entry() { return new Entry(); }

    //
    // Container operations
    //

    int size() const { return entries.size(); }
    bool empty() const { return entries.empty(); }

    vector<Entry *>::const_iterator begin() const { return entries.begin(); }
    vector<Entry *>::const_iterator end()   const { return entries.end(); }

    Entry * operator[]( int i ) { return entries[i]; }

    void clear_entries()
    {
        for ( int i=0; i < entries.size() )
            delete entries[i];
        entries.clear();
    }

    void clear_all()
    {
        clear_entries();
        header_comments.clear();
        footer_comments.clear();
    }

    void erase( int index ) { entries.erase( index ); }
    void remove( int index ) { erase( index ); }
    void insert( int before, const Entry * entry ) { entries.insert( before, entry ); }
    void push_back( const Entry * entry ) { entries.push_back( entry ); }
    void append( const Entry * entry ) { push_back( entry ); }
    void operator<<( const Entry * entry ) { push_back( entry ); }

    string_vec & get_header_comments() { return header_comments; }
    string_vec & get_footer_comments() { return footer_comments; }

    const string & get_filename() const { return filename; }

    string get_comment_marker() const { return comment_marker; }
    void set_comment_marker( const string & marker ) { comment_marker = marker; }

private:

    bool is_comment_line( const string & line );
    bool is_empty_line( const string & line );
    void split_off_comment( const string & line, string & content_ret, string & comment_ret );

    string          filename;
    string          comment_marker;

    string_vec      header_comments;
    vector<Entry *> entries;
    string_vec      footer_comments;

};

#endif // CommentedConfigFile_h
