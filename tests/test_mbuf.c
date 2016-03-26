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

#include <nyx.h>
#include <nyx_mbuf.h>

int
main (int argc, char *argv[])
{
  NYX_MBUF *buf1;
  NYX_MBUF *buf2;
  char     *str;

  buf1 = nyx_mbuf_new (0, "This is a initialization string\nSecond\n");
  buf2 = nyx_mbuf_new (1024, NULL);
  nyx_mbuf_append_txt (buf1, "This is a second initialization string\n");
  nyx_mbuf_append_txt (buf2, "This is a second initialization string\n");

  printf ("%p - buf1 (%d - %d): '%s'\n", buf1, 
	  nyx_mbuf_get_size (buf1), 
	  nyx_mbuf_get_len (buf1), 
	  (char*) nyx_mbuf_get_data (buf1));
  printf ("%p - buf2 (%d - %d): '%s'\n", buf2, 
	  nyx_mbuf_get_size (buf2), 
	  nyx_mbuf_get_len (buf2), 
	  (char*) nyx_mbuf_get_data (buf2));
  
  str = nyx_mbuf_peek_line (buf1);
  printf ("Peek:'%s'\n", str);
  free (str);
  printf ("Peek -buf1 (%d - %d): '%s'\n",
	  nyx_mbuf_get_size (buf1), 
	  nyx_mbuf_get_len (buf1), 
	  (char*) nyx_mbuf_get_data (buf1));


  str = nyx_mbuf_get_line (buf1);
  printf ("\n\nGet:'%s'\n", str);
  free (str);
  printf ("Get -buf1 (%d - %d): '%s'\n",
	  nyx_mbuf_get_size (buf1), 
	  nyx_mbuf_get_len (buf1), 
	  (char*) nyx_mbuf_get_data (buf1));

  return 0;
}
