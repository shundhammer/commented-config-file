
AC_INIT(commented-config-file, 0.1)
AC_CONFIG_SRCDIR(src/main.cc)
AM_INIT_AUTOMAKE
AM_CONFIG_HEADER(config.h)

CWARNS="-Wall -Wextra -Wformat=2 -Wmissing-prototypes"
CFLAGS="${CFLAGS} ${CWARNS}"

CXXWARNS="-Wall -Wextra -Wformat=2 -Wnon-virtual-dtor -Wno-unused-parameter"
CXXFLAGS="${CXXFLAGS} -std=c++11 ${CXXWARNS}"

AC_PROG_CXX
# AC_PREFIX_DEFAULT(/usr)

AC_OUTPUT(
    Makefile
    src/Makefile
    test/Makefile
    )
