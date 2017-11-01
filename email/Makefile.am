## Process this file with automake to produce Makefile.in
include $(top_srcdir)/flymake.am

AUTOMAKE_OPTIONS = 1.6 foreign

EXTRA_DIST = email.h address.h

AM_CPPFLAGS = -I$(top_srcdir)

noinst_LIBRARIES = libemail.a

libemail_a_SOURCES = address.c

