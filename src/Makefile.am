#
# Nyx: A library with basic functions
# Copyright (c) 2015 David Mart.nez Oliveira
#
# This file is part of Nyx
#
# Nyx is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Nyx is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Nyx.  If not, see <http://www.gnu.org/licenses/>.
#

#AM_CFLAGS=-I$(srcdir)/../include



# Nyx Library

EXTRA_DIST= nyx.h nyx_list.h nyx_mbuf.h nyx_queue.h nyx_channel.h \
	    nyx_net.h nyx_server.h
include_HEADERS= nyx.h nyx_list.h nyx_mbuf.h nyx_queue.h nyx_channel.h \
		 nyx_net.h nyx_server.h

lib_LTLIBRARIES = libnyx.la
libnyx_la_SOURCES= nyx.c nyx_list.c nyx_mbuf.c nyx_queue.c nyx_channel.c \
	           nyx_net.c nyx_server.c
libnyx_la_LIBADD=-lpthread

