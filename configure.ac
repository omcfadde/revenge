#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([revenge], [2.0.0], [omcfadde@gmail.com],
	[revenge], [http://gitorious.org/omcfadde/revenge])
AM_INIT_AUTOMAKE([subdir-objects])
AM_SILENT_RULES([yes])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

AC_DEFINE([_GNU_SOURCE], [1], [This function is a GNU extension.])

# Checks for programs.
AC_PROG_CC
AC_PROG_MAKE_SET

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lGL':
AC_CHECK_LIB([GL], [main])
# FIXME: Replace `main' with a function in `-lSDL':
AC_CHECK_LIB([SDL], [main])
# FIXME: Replace `main' with a function in `-lpci':
AC_CHECK_LIB([pci], [main])
# FIXME: Replace `main' with a function in `-lz':
AC_CHECK_LIB([z], [main])

PKG_CHECK_MODULES([PCI], [libpci >= 3.1.7])
PKG_CHECK_MODULES([SDL], [sdl >= 1.2.14])
PKG_CHECK_MODULES([ZLIB], [zlib >= 1.2.3.4])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdint.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_CHECK_FUNCS([memset mkdir munmap strerror])

AC_CONFIG_FILES([Makefile
		 src/Makefile])
AC_OUTPUT
