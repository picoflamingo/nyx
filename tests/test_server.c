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
#include <nyx_queue.h>
#include <nyx_net.h>
#include <nyx_server.h>


void*
consumer (void *arg)
{
  NYX_WORKER  *w = (NYX_WORKER *) arg;
  NYX_SERVER  *s = (NYX_SERVER*) nyx_worker_get_data (w);
  NYX_QUEUE   *q;
  NYX_NET_MSG *msg;
  int         cnt = 0;
  q = nyx_server_get_queue (s);
  while (1)
    {
      msg = (NYX_NET_MSG*) nyx_queue_get (q);

      if (msg->size == 0) 
	{
	  printf ("*** Size: 0\n");
	  continue;
	}
      nyx_server_printf (s, msg->c, "ECHO(%d):%s", cnt, msg->data);

      free (msg->data);
      nyx_net_msg_free (msg);
      cnt++;
    }
}


int
main (int argc, char *argv[])
{
  NYX_NET    *net;
  NYX_SERVER *s;

  nyx_init ();
  printf ("NYX Echo Server\n");

  s = nyx_server_new ("*", 8000);
  
  /* Create net object */
  net = nyx_net_init ();


  nyx_server_register (s, net, consumer);

  nyx_net_run (net);
  
  nyx_server_free (s);
  nyx_net_free (net);

  nyx_cleanup ();
  return 0;
}
