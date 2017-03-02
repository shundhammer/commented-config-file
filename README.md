# CommentedConfigFile C++ Class

(c) 2017 Stefan Hundhammer <Stefan.Hundhammer@gmx.de>

License: GPL V2

## System Requirements:

- C++
- STL
- Boost

## Overview

This is a utility class for C++ to read and write config files that might
contain comments.  This class tries to preserve any existing comments and keep
them together with the content line immediately following them.

This class supports the notion of a header comment block, a footer comment
block, a comment block preceding any content line and a line comment on the
content line itself.

A comment preceding a content line is stored together with the content line,
so moving around entries in the file will keep the comment with the content
line it belongs to.

The default comment marker is '#' like in most Linux config files, but it
can be set with setCommentMarker().

## Example

(line numbers added for easier reference):

    001    # Header comment 1
    002    # Header comment 2
    003    # Header comment 3
    004
    005
    006    # Header comment 4
    007    # Header comment 5
    008
    009    # Content line 1 comment 1
    010    # Content line 1 comment 2
    011    content line 1
    012    content line 2
    013
    014    content line 3
    015
    016    content line 4
    017    content line 5 # Line comment 5
    018    # Content line 6 comment 1
    019
    020    content line 6 # Line comment 6
    021    content line 7
    022
    023    # Footer comment 1
    024    # Footer comment 2
    025
    026    # Footer comment 3


Empty lines or lines that have only whitespace belong to the next comment
block: The footer comment consists of lines 022..026.

The only exception is the header comment that stretches from the start of
the file to the last empty line preceding a content line. This is what
separates the header comment from the comment that belongs to the first
content line. In this example, the header comment consists of lines
001..008.

- Content line 1 in line 011 has comments 009..010.
- Content line 2 in line 012 has no comment.
- Content line 3 in line 014 has comment 013 (an empty line).
- Content line 5 in line 017 has a line comment "# Line comment 5".
- Content line 6 in line 020 has comments 018..019 and a line comment.

Applications using this class can largely just ignore all the comment stuff;
the class will handle the comments automagically.
