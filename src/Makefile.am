#AM_CFLAGS=-I$(srcdir)/../include


noinst_PROGRAMS=test01

test01_SOURCES=test01.c
test01_LDADD=./libnyx.la



# Nyx Library

EXTRA_DIST= nyx.h nyx_list.h
include_HEADERS= nyx.h nyx_list.h

lib_LTLIBRARIES = libnyx.la
libnyx_la_SOURCES= nyx.c nyx_list.c
#libinyx_la_LIBADD=-lbluetooth

