
AM_CPPFLAGS = -I$(top_srcdir)/src

LDADD = ../src/CommentedConfigFile.o	\
	../src/Diff.o			\
	-lboost_unit_test_framework

check_PROGRAMS =		\
	simple_string_ops.test	\
	container_ops.test	\
	parser.test		\
	formatter.test		\
	diff.test

AM_DEFAULT_SOURCE_EXT = .cc

TESTS = $(check_PROGRAMS)
