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
#include <string.h>
#include <stdlib.h>

#include <stdarg.h>

#include <nyx_list.h>
#include <nyx_queue.h>
#include <nyx_channel.h>
#include <nyx_net.h>

#include <nyx_server.h>

/* NYX_WORKER to be moved into NYX_NET or its own file */
#include <pthread.h>

/******************************************************/
/* NYX_Server implementation  */
/******************************************************/

/* Default callbacks */
// TODO: Store these callback in variables and the the application
//       change the,... Right now I need something simple up and running

/* Read callback 
 *   Just update internal channel buffer and if a line is available
 *   push the string into the associated queue
 */
static int
_nyx_server_read_cb (NYX_CHANNEL *c, void *arg)
{
  NYX_NET_MSG *msg = (NYX_NET_MSG*)arg;
  char *txt;
  int  r;

  if ((r = nyx_channel_update (msg->c)) <= 0)
    {
      return -1;
    }
  /* Checks if there is a line available */
  if ((txt = nyx_channel_get_line (msg->c)))
    {
      /* Update message and add to queue */
      nyx_net_msg_set (msg, (void*)txt, strlen(txt), msg->c, msg->q);
      nyx_queue_add (msg->q, msg);
    }

  return 1;
}

/* Accept callback 
 *   Accept the connection and register the new client 
 *   XXX: We should use the channel private data to keep a pointer
 *        to the server object so we can add new clients to a server list
 *   TODO: see above
 */
static int
_nyx_server_accept_cb (NYX_CHANNEL *c, void *arg)
{
  NYX_CHANNEL *c1;
  NYX_NET_MSG *msg = (NYX_NET_MSG*)arg;
  
  if ((c1 = nyx_channel_accept (c)) == NULL)
    {
      fprintf (stderr, "Cannot accept connection... Ignoring\n");
      return -1;
    }

  nyx_channel_set_cb (c1, _nyx_server_read_cb);

  /* Connection accepted. Registering new client */
  nyx_net_register (msg->net, c1, msg->q);

  return 1;
}

NYX_SERVER*    
nyx_server_new (char *ip, int port)
{
  NYX_SERVER *aux;

  if (!ip) return NULL;
  if (port < 0 || port > 65535) return NULL;

  if ((aux = malloc (sizeof(NYX_SERVER))) == NULL)
    {
      fprintf (stderr, "Cannot create NYX_SERVER object\n");
      return NULL;
    }
  if ((aux->s = nyx_channel_new (NULL)) == NULL)
    {
      fprintf (stderr, "Cannot create server channel object\n");
      free (aux);
      return NULL;
    }
  if ((aux->q = nyx_queue_new (32)) == NULL)
    {
      fprintf (stderr, "Cannot create server message queue object\n");
      nyx_channel_free (aux->s);
      free (aux);
      return NULL;
    }
  if ((aux->w = nyx_worker_new (aux->q, NULL)) == NULL)
    {
      fprintf (stderr , "Cannot create message worker object\n");
      nyx_queue_free (aux->q);
      nyx_channel_free (aux->s);
      free (aux);
    }

  aux->net = NULL;
  nyx_channel_tcp_init (aux->s, ip, port, 1);
  nyx_channel_set_cb (aux->s, _nyx_server_accept_cb);


  return aux;
}

int           
nyx_server_free (NYX_SERVER *s)
{
  if (!s) return -1;
  
  if (s->q) nyx_queue_free (s->q);
  if (s->w) nyx_worker_free (s->w);     
  if (s->s) nyx_channel_free (s->s);

  free (s);
  
  return 0;
}

int           
nyx_server_register (NYX_SERVER *s, NYX_NET *net, NYX_WORKER_FUNC f)
{
  if (!s) return -1;
  if (!f) return -1;
  if (!s->w) return -1;

  s->net = net;

  if (net)
    {
      nyx_worker_start (s->w, f, s);
      nyx_net_register (net, s->s, s->q);

    }

  return 0;
}


int           
nyx_server_printf   (NYX_SERVER *s, NYX_CHANNEL *c, char *fmt, ...)
{
  char    buf[BUF_SIZE];
  int     len;
  va_list arg;

  if (!c) return -1;
  if (!fmt) return -1;

  va_start (arg, fmt);

  memset (buf, 0, BUF_SIZE);
  len = vsnprintf (buf, BUF_SIZE, fmt, arg);

  if (len >= BUF_SIZE)
    {
      fprintf (stderr, "Output truncated!!!\n");
      buf[BUF_SIZE - 1] = 0;
    }

  if ((len = nyx_channel_write (c, buf, len)) < 0)
    {
      fprintf (stderr, "Error writting.... Unregistering client\n");
      nyx_net_unregister (s->net, c);
    }

  va_end (arg);
  return len;

}



/* XXX: We should probably add a mutex to access server fields
 *      as different workers may want to check values
 *      On the other hand they shouldn't change during the
 *      server's life.
 */
NYX_CHANNEL*  
nyx_server_get_channel (NYX_SERVER *s)
{
  if (!s) return NULL;

  return s->s;
}

NYX_QUEUE*    
nyx_server_get_queue (NYX_SERVER *s)
{
  if (!s) return NULL;

  return s->q;
}

NYX_WORKER*   
nyx_server_get_worker (NYX_SERVER *s)
{
  if (!s) return NULL;

  return s->w;

}


/*** WORKER ****/

NYX_WORKER*
nyx_worker_new (NYX_QUEUE *q, void *data)
{
  NYX_WORKER *aux;

  if (!q) return NULL;

  if ((aux = malloc (sizeof(NYX_WORKER))) == NULL)
    {
      fprintf (stderr, "Cannot create NYX_WORKER object\n");
      return NULL;
    }
  
  aux->q = q;
  aux->data = data;

  return aux;
}

int           
nyx_worker_free (NYX_WORKER *w)
{
  if (!w) return -1;

  nyx_worker_stop (w);
  /* This object does not own neither the queue neither the data args */
  free (w);

  return 0;
}


int           
nyx_worker_start (NYX_WORKER *w, NYX_WORKER_FUNC f, void *arg)
{
  if (!w) return -1;

  w->run  = 1;
  pthread_create (&w->tid, NULL, f, arg);

  return 0;
}

int           
nyx_worker_stop  (NYX_WORKER *w)
{
  if (!w) return -1;
  
  w->run = 0;
  /* TODO: Wait for the thread to terminate */

  return 0;
}


