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


/* parameters to be passed to the worker.
 * In general this is application specific
 */
typedef struct my_worker_t
{
  char       *id;
  NYX_WORKER *w;
} MY_WORKER;

/* Default worker for nyx_server. parameter is of type NYX_WORKER 
 * Server object is provided in the private data field of the worker*/
void*
consumer (void *arg)
{
  NYX_WORKER  *w = (NYX_WORKER *) arg;
  NYX_SERVER  *s = (NYX_SERVER*) w->data;
  NYX_QUEUE   *q;
  NYX_NET_MSG *msg;

  q = nyx_server_get_queue (s);
  while (1)
    {
      msg = (NYX_NET_MSG*) nyx_queue_get (q);

      nyx_server_printf (s, msg->c, "DEFAULT:%s", msg->data);

      free (msg->data);
      nyx_net_msg_free (msg);
      sleep (2);
    }
}

void*
consumer1 (void *arg)
{
  MY_WORKER   *mw = (MY_WORKER *) arg;
  NYX_WORKER  *w = mw->w;
  NYX_SERVER  *s = (NYX_SERVER*) nyx_worker_get_data (w);
  NYX_QUEUE   *q;
  NYX_NET_MSG *msg;

  q = nyx_server_get_queue (s);
  while (1)
    {
      msg = (NYX_NET_MSG*) nyx_queue_get (q);
      if (!msg) continue;
      nyx_server_printf (s, msg->c, "%s:%s", mw->id, msg->data);

      free (msg->data);
      nyx_net_msg_free (msg);
      sleep (1); 
    }

}

NYX_WORKER*
add_worker (NYX_SERVER *s, char *id)
{
  NYX_WORKER *w;
  NYX_QUEUE  *q;
  MY_WORKER  *mw;

  q = nyx_server_get_queue (s);
  printf ("Starting up worker '%s'...\n", id);

  w = nyx_worker_new (q, (void*)s);

  mw = malloc (sizeof(MY_WORKER));
  mw->id = strdup (id);
  mw->w = w;

  nyx_worker_start (w, consumer1, (void*)mw);

  return w;
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

  /* Create additional workers */
  NYX_WORKER *w[16];
  int        n_workers = 0;

  /* Create some workers... worker pool */
  w[n_workers++] = add_worker (s, "Worker1");
  w[n_workers++] = add_worker (s, "Worker2");
  w[n_workers++] = add_worker (s, "Worker3");


  nyx_net_run (net);

  for (;n_workers > 0;) nyx_worker_free (w[--n_workers]);

  nyx_server_free (s);
  nyx_net_free (net);

  nyx_cleanup ();
  return 0;
}
