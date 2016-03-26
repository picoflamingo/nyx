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


#include <sys/poll.h>

#include <nyx.h>
#include <nyx_list.h>
#include <nyx_mbuf.h>
#include <nyx_queue.h>
#include <nyx_channel.h>
#include <nyx_net.h>

NYX_NET*  
nyx_net_init ()
{
  NYX_NET    *aux;
  
  if ((aux = malloc (sizeof(NYX_NET))) == NULL)
    {
      fprintf (stderr, "Cannot allocate memory for NYX_NET object.\n");
      return NULL;
    }

  aux->run = 0;
  /* Create lists */
  if ((aux->channel = nyx_list_new ("channels", NYX_NET_DEFAULT_N, sizeof (NYX_CHANNEL*))) == NULL)
    {
      fprintf (stderr, "Cannot allocate channel NYX_LIST for NYX_NET object\n");
      free (aux);
      return NULL;
    }
  if ((aux->queue = nyx_list_new ("queue", NYX_NET_DEFAULT_N, sizeof (NYX_QUEUE*))) == NULL)
    {
      fprintf (stderr, "Cannot allocate channel NYX_LIST for NYX_NET object\n");
      free (aux);
      return NULL;
    }

  /* Create struct pollfd array for polling */
  aux->fd_cap = NYX_NET_DEFAULT_N;
  aux->fd_n   = 0;
  aux->tmo    = NYX_NET_DEFAULT_TMO;
  if ((aux->fd = malloc (sizeof(struct pollfd) * aux->fd_cap)) == NULL)
    {
      fprintf (stderr, "Cannot allocate memory for poll\n");
      nyx_list_free (aux->channel);
      free (aux);
      return NULL;
    }

  return aux;
}

int       
nyx_net_free (NYX_NET *net)
{
  if (!net) return -1;
  
  /* TODO: Close all connections....*/
  if (net->channel) nyx_list_free (net->channel);
  if (net->queue) nyx_list_free (net->queue);

  free (net);
  return 0;
}

int
_nyx_net_del_fd (NYX_NET *net, int s)
{
  int i, n, aux;

  n = net->fd_n;
  for (i = 0; i < n; i++)
    {
      if (net->fd[i].fd == s)
	{
	  aux = i;
	  memcpy (&net->fd[i], &net->fd[net->fd_n - 1], sizeof(struct pollfd));
	  net->fd_n --;
	  return aux;
	}
    }
  fprintf (stderr, "FD %d not found....\n", s);
  return -1;
}

int
_nyx_net_add_fd (NYX_NET *net, int s)
{
  int   i;
  struct pollfd *aux;

  if (net->fd_n == net->fd_cap) // If list is full... realloc
    {
      /* We have to reallocate fd list  */
      if ((aux = realloc (net->fd, 
			  sizeof(struct pollfd) * (net->fd_cap + NYX_NET_DEFAULT_INC))) == NULL)
	{
	  fprintf(stderr, "Cannot reallocate fd list\n");
	  return -1;
	}
      net->fd_cap += NYX_NET_DEFAULT_INC;
      net->fd = aux;
    }
  i = net->fd_n;
  /* List will be compacted so we can always add at the end */
  net->fd[i].fd = s;
  net->fd[i].events = POLLIN || POLLHUP;
  net->fd_n++;

  return net->fd_n;
}

int       
nyx_net_register   (NYX_NET *net, NYX_CHANNEL *c, NYX_QUEUE *q)
{
  if (!net) return -1;
  if (!c) return -1;

  /* Add channel to channel list */
  nyx_list_add_item (net->channel, c);
  nyx_list_add_item (net->queue, q);
  /* Add fd to struct pollfd */
  _nyx_net_add_fd (net, c->s);
 
  return 0;
}

int       
nyx_net_unregister (NYX_NET *net, NYX_CHANNEL *c)
{
  int  indx;

  if (!net) return -1;

  if ((indx = _nyx_net_del_fd (net, c->s)) < 0)
    {
      fprintf (stderr, "Cannot unregister... channel not found (%d)\n", c->s);
      return -1;
    }
  /* The fd has been removed from the poll list, now we have to destroy it */
  /* First remove from the channel list, then destry the channel */
  nyx_list_del_item_by_index (net->channel, indx);
  nyx_list_del_item_by_index (net->queue, indx);
  
  nyx_channel_free (c);

  return 0;
}


int       
nyx_net_run_step (NYX_NET *net)
{
  int  r, n, i;
  NYX_CHANNEL   *c;
  NYX_QUEUE     *q;
  NYX_NET_MSG   *m;

  if (!net) return -1;

  n = net->fd_n;
  if ((r = poll (net->fd, n, net->tmo)) < 0)
    {
      perror ("poll:");
      return -1;
    }
  if (r == 0)
    {

      return 0;
    }

  //printf ("%d fd with data out of %d\n", r, n);
  /* At this point we have something to read.
   * Check which file descriptor got data... */
  for (i = 0; i < n; i++)
    {
      //printf ("Checking %d events: 0x%0x\n", i, net->fd[i].revents);
      if (net->fd[i].revents & POLLHUP)
	{
	  c = nyx_list_get_item (net->channel, i);
	  nyx_net_unregister (net, c);
	}
      if (net->fd[i].revents & POLLIN)
	{
	  //printf ("Got data in %d\n", i);
	  /* Got data to read... call default cb!*/
	  c = nyx_list_get_item (net->channel, i);
	  q = nyx_list_get_item (net->queue, i);
	  m = nyx_net_msg_new (net, NULL, 0, c, q);
	  if (c->cb) 
	    {
	      if ((c->cb (c, m)) <= 0)
		{
		  perror ("read/accept:");
		  fprintf (stderr, "Error reading data... unregistering channel\n");
		  nyx_net_unregister (net, c);
		}
	    }
	}
    }


  return 0;
}

int       
nyx_net_run  (NYX_NET *net)
{
  if (!net) return -1;

  net->run = 1;

  while (net->run)
    nyx_net_run_step (net);

  return 0;
}

/* Network Messages implementation */

NYX_NET_MSG*  
nyx_net_msg_new (NYX_NET *n, void *data, int size, 
		 NYX_CHANNEL *c, NYX_QUEUE *q)
{
  NYX_NET_MSG *msg;

  if (!n) return NULL;

  if ((msg = malloc (sizeof(NYX_NET_MSG)))== NULL)
    {
      fprintf (stderr, "Cannot create NET_MSG object\n");
      return NULL;
    }

  msg->data = data;
  msg->size = size;
  msg->c = c;
  msg->q = q;
  msg->net = n;


  return msg;
}

int           
nyx_net_msg_free (NYX_NET_MSG *msg)
{
  if (!msg) return -1;

  free (msg);

  return 0;
}

int           
nyx_net_msg_set (NYX_NET_MSG *msg, void *data, int size, 
		 NYX_CHANNEL *c, NYX_QUEUE *q)
{
  if (!msg) return -1;
  msg->data = data;
  msg->size = size;
  msg->c = c;
  msg->q = q;

  return 0;
}

NYX_NET_MSG*  
nyx_net_msg_clone (NYX_NET_MSG *msg)
{
  NYX_NET_MSG  *m;
  int          size = msg->size;

  if ((m = nyx_net_msg_new (msg->net, msg->data, size, msg->c, msg->q)) == NULL)
    {
      fprintf (stderr, "Cannot clone NYX_NET_MSG object\n");
      return NULL;
    }

  /* We actually clone the buffer*/
  if ((m->data = malloc (size)) == NULL)
    {
      fprintf (stderr, "Cannot allocate buffer for NYX_NET_MSG clone\n");
      nyx_net_msg_free (m);
      return NULL;
    }
  memcpy (m->data, msg->data, size);
  m->size = size;
  m->c = msg->c;
  m->q = msg->q;
  m->net = msg->net;

  return m;
}

