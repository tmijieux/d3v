AC_INIT([d3v], [0.1], [tom dot tix at hotmail dot fr])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AM_SILENT_RULES([yes])
AC_CONFIG_MACRO_DIR([m4])

# TOOLS
AC_PROG_CC
AM_PROG_AR
PKG_PROG_PKG_CONFIG
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.19.8])
LT_INIT([])

# DEPS
PKG_CHECK_MODULES([GLIB], [glib-2.0])
PKG_CHECK_MODULES([GIO], [gio-2.0])
PKG_CHECK_MODULES([GLUT], [freeglut])
PKG_CHECK_MODULES([JPEG], [libjpeg])

# OUTPUT
AC_CONFIG_FILES([
    po/Makefile.in
    Makefile
    src/Makefile
])

AC_OUTPUT
