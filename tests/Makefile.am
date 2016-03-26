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


noinst_PROGRAMS=test01 test_mbuf test_queue test_channel test_net \
		test_net_server test_server

test01_SOURCES=test01.c
test01_LDADD=../src/libnyx.la -lpthread

test_mbuf_SOURCES=test_mbuf.c
test_mbuf_LDADD=../src/libnyx.la -lpthread

test_queue_SOURCES=test_queue.c
test_queue_LDADD=../src/libnyx.la -lpthread

test_channel_SOURCES=test_channel.c
test_channel_LDADD=../src/libnyx.la -lpthread

test_net_SOURCES=test_net.c
test_net_LDADD=../src/libnyx.la -lpthread

test_net_server_SOURCES=test_net_server.c
test_net_server_LDADD=../src/libnyx.la -lpthread

test_server_SOURCES=test_server.c
test_server_LDADD=../src/libnyx.la -lpthread



