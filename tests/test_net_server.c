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

/* Normal client callback */
int
read_cb (NYX_CHANNEL *c, void *arg)
{
  NYX_NET_MSG *msg = (NYX_NET_MSG*)arg;
  char *txt;
  int  r;

  if ((r = nyx_channel_update (msg->c)) <= 0)
    {
      return -1;
    }
  // Check if there is a line available
  if ((txt = nyx_channel_get_line (msg->c)))
    {
      /* Update message and add to queue */
      nyx_net_msg_set (msg, (void*)txt, strlen(txt), msg->c, msg->q);
      nyx_queue_add (msg->q, msg);
    }
  return 1;
}

/* Accept callback */
int
accept_cb (NYX_CHANNEL *c, void *arg)
{
  NYX_CHANNEL *c1;
  NYX_NET_MSG *msg = (NYX_NET_MSG*)arg;
  
  if ((c1 = nyx_channel_accept (c)) == NULL)
    {
      fprintf (stderr, "Cannot accept connection... Ignoring\n");
      return -1;
    }
  nyx_channel_set_cb (c1, read_cb);
  fprintf (stderr, "Accepted connection on client %p\n", c1);
  /* Connection accepted. Registering new client */
  nyx_net_register (msg->net, c1, msg->q);

  return 1;
}


void*
consumer (void *arg)
{
  NYX_QUEUE *q = (NYX_QUEUE*) arg;
  NYX_NET_MSG *msg;

  while (1)
    {
      msg = (NYX_NET_MSG*) nyx_queue_get (q);
      printf ("CONSUMER: '%s'\n", (char*)msg->data);
      /* Do the echo thingy */
      if ((nyx_channel_write (msg->c, msg->data, msg->size)) < 0)
	{
	  fprintf (stderr, "Removing client %p\n", msg->c);
	  nyx_net_unregister (msg->net, msg->c);
	}
      nyx_channel_write (msg->c, "\n",1);

      free (msg->data);
      nyx_net_msg_free (msg);

    }
}


int
main (int argc, char *argv[])
{
  NYX_CHANNEL *c1;
  NYX_NET     *net;
  NYX_QUEUE   *q;
  pthread_t   tid;
  nyx_init ();

  /* Create client channels */
  c1 = nyx_channel_new (NULL);
  nyx_channel_tcp_init (c1, "*", 8000, 1);
  nyx_channel_set_cb (c1, accept_cb);


  /* Create Queue */
  q = nyx_queue_new (50);

  /* Create worker */
  pthread_create (&tid, NULL, consumer, q);
  
  /* Create net object */
  net = nyx_net_init ();

  /* register channels */
  nyx_net_register (net, c1, q);

  nyx_net_run (net);
  
  nyx_channel_free (c1);
  nyx_queue_free (q);
  nyx_net_free (net);

  nyx_cleanup ();
  return 0;
}
