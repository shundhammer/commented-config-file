
AM_CPPFLAGS = -I$(top_srcdir)/src

LDADD = ../src/CommentedConfigFile.o -lboost_unit_test_framework

check_PROGRAMS =		\
	empty_lines.test	\
	comment_lines.test

AM_DEFAULT_SOURCE_EXT = .cc

TESTS = $(check_PROGRAMS)
