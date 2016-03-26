/*
 * Nyx: A library with basic functions
 * Copyright (c) 2015 David Mart.nez Oliveira
 *
 * This file is part of Nyx
 *
 * Nyx is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nyx is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nyx.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include <unistd.h>
#include <pthread.h>

#include <nyx.h>
#include <nyx_channel.h>

int
main (int argc, char *argv[])
{
  NYX_CHANNEL *c1, *c2;

  nyx_init ();
  c1 = nyx_channel_new (NULL);
 
  nyx_channel_tcp_init (c1, "127.0.0.1", 8000, 0);
  nyx_channel_print (c1, "Hello World!\n");
  nyx_channel_free (c1);

  c1 = nyx_channel_new (NULL);
  nyx_channel_tcp_init (c1, "*", 8001,1); 
  c2 = nyx_channel_accept (c1);
  nyx_channel_print (c2, "I'm Muzzy the Server!\n");
  nyx_channel_free (c2);
  nyx_channel_free (c1);

  nyx_cleanup ();
  return 0;
}
