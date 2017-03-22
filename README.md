# CommentedConfigFile C++ Class

(c) 2017 Stefan Hundhammer <Stefan.Hundhammer@gmx.de>

License: GPL V2

## Overview

- CommentedConfigFile class
- ColumnConfigFile class
- Generic Diff class for string vectors


## System Requirements:

- C++
- STL
- Boost
- Autotools (just for the demo programs and the test suite)

This class should work on Linux/BSD/Unix-like systems and on all kinds of MS
Windows. It's just some C++ classes after all.


## CommentedConfigFile

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

(line numbers added for easier reference)

    001	   # Header comment 1
    002	   # Header comment 2
    003	   # Header comment 3
    004
    005
    006	   # Header comment 4
    007	   # Header comment 5
    008
    009	   # Content line 1 comment 1
    010	   # Content line 1 comment 2
    011	   content line 1
    012	   content line 2
    013
    014	   content line 3
    015
    016	   content line 4
    017	   content line 5 # Line comment 5
    018	   # Content line 6 comment 1
    019
    020	   content line 6 # Line comment 6
    021	   content line 7
    022
    023	   # Footer comment 1
    024	   # Footer comment 2
    025
    026	   # Footer comment 3


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


## Building

Once:

    ./build-all

After that:

    make


## Installing

There is nothing to install: It's just C++ classes.
You can add them to a library or to an application.


## Installing Boost Headers

Ubuntu:

    sudo apt-get install libboost-dev libboost-test-dev


## Running the Test Suite

    make check

To run just one single test (to get more output if it failed):

    cd test
    ./container_ops.test

(or whichever of them failed)


## Sample Output

The `ccf_demo` program in `src/main.cc` produces this output from my `/etc/fstab` file:

    <Header>
       1: # /etc/fstab: static file system information.
       2: #
       3: # [sh @ balrog] ~ %  sudo blkid | column -t
       4: #
       5: # /dev/sda1:  LABEL="Win-Boot"    UUID="C6CC71BDCC71A877"                      TYPE="ntfs"
       6: # /dev/sda2:  LABEL="Win-App"     UUID="3E5E77515E770147"                      TYPE="ntfs"
       7: #
       8: # /dev/sdb1:  LABEL="swap"        UUID="be72e905-a417-41a4-a75f-12c0cf774f6a"  TYPE="swap"
       9: # /dev/sdb2:  LABEL="openSUSE"    UUID="1d0bc24c-ae68-4c4e-82af-b3e184b2ac9d"  TYPE="ext4"
      10: # /dev/sdb3:  LABEL="Ubuntu"      UUID="f5c15fbd-0417-4711-a0b7-f66b608bad0c"  TYPE="ext4"
      11: # /dev/sdb5:  LABEL="work"        UUID="7e1d65c8-c6e3-4824-ac1c-c3a4ba90f54f"  TYPE="ext4"
      12: #
      13: #
      14: # <file system>              <mount point>   <type> <options>         <dump>  <pass>
      15:
    </Header>

    <Content>
      Entry #1 content  : /dev/disk/by-label/swap      none             swap  sw                         0  0
      Entry #2 content  : /dev/disk/by-label/openSUSE  /alternate-root  ext4  defaults                   0  2
      Entry #3 content  : /dev/disk/by-label/Ubuntu    /                ext4  errors=remount-ro          0  1
      Entry #4 content  : /dev/disk/by-label/work      /work            ext4  defaults                   0  2

      Entry #5 comment 1:
      Entry #5 comment 2: # Windows disk
      Entry #5 content  : /dev/disk/by-label/Win-Boot  /win/boot        ntfs  defaults,umask=007,gid=46  0  0
      Entry #6 content  : /dev/disk/by-label/Win-App   /win/app         ntfs  defaults,umask=007,gid=46  0  0
    </Content>

    <Footer>
      1:
      2:
      3: # nas:/share/sh              /nas/sh          nfs   bg,intr,soft,retry=6       0  0
      4: # nas:/share/work            /nas/work        nfs   bg,intr,soft,retry=6       0  0
      5:
    </Footer>

I.e. it correctly detected the header comments, the footer comments and the
comment that belongs to _Entry #5_. When the entries are rearranged, e.g. the
entries for the Windows disk are moved above the Linux disk, that comment
remains attached to that entry, i.e. it is moved together with that entry.

Of course that has limitations. If a comment does not belong to the next entry,
but to the previous one, it is moved to the wrong location. That's life. But
it's much better than throwing away all comments every time a program touches a
config file.


## ColumnConfigFile

This class is derived from CommentedConfigFile. In addition to the comment
handling, this class also manages splitting up column-oriented config files
into the columns.

The content lines may all have the same number of columns (like in
`/etc/fstab`), or they might have different numbers of columns.


## Diff

This is a generic Diff class for STL `vector<string>` that works just like the
Linux `diff -u` command - with or without context lines, as configured.

The `ccf_diff` example can be used pretty much as a drop-in replacement for
`diff -u`.

This Diff class may not always return the absolute minimum diff (or "edit
script"), but it will always be a human readable one.

I had wondered why there is no ready-made class for this anywhere in STL or
even Boost (or is there?); this is useful in many cases; for example, when
writing just the changes done to a config file to the log. This is why this
class is part of this little class library: It is ready to be used on the
CommentedConfigFile level if you just configure diffs to be enabled there
(which just saves the old lines at tactical points such as reading and parsing
file content).
