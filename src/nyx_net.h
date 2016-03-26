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

#ifndef NYX_NET_H
#define NYX_NET_H

#include <sys/poll.h>

#include <nyx_list.h>
#include <nyx_queue.h>
#include <nyx_channel.h>


#define NYX_NET_DEFAULT_N   10
#define NYX_NET_DEFAULT_INC 10
#define NYX_NET_DEFAULT_TMO 1000

typedef struct nyx_net_t
{
  int              fd_cap, fd_n; 
  struct pollfd    *fd;          /* pollfd structure */
  NYX_LIST         *channel;     /* channel List */
  NYX_LIST         *queue;       /* Associated Queue to channel */
  int              run;
  int              tmo;
} NYX_NET;


typedef struct nyx_net_msg_t
{
  NYX_NET     *net;
  void        *data;
  int         size;
  NYX_CHANNEL *c;
  NYX_QUEUE   *q;
} NYX_NET_MSG;


#ifdef __cplusplus
extern "C" {
#endif

  NYX_NET*  nyx_net_init ();
  int       nyx_net_free (NYX_NET *net);

  int       nyx_net_register   (NYX_NET *net, NYX_CHANNEL *c, NYX_QUEUE *q);
  int       nyx_net_unregister (NYX_NET *net, NYX_CHANNEL *c);

  int       nyx_net_run_step (NYX_NET *net);
  int       nyx_net_run      (NYX_NET *net);



  /* Network Messages */
  NYX_NET_MSG*  nyx_net_msg_new (NYX_NET *net, void *data, int size, 
				 NYX_CHANNEL *c, NYX_QUEUE *q);
  int           nyx_net_msg_free (NYX_NET_MSG *msg);
  int           nyx_net_msg_set (NYX_NET_MSG *msg, void *data, int size, 
				 NYX_CHANNEL *c, NYX_QUEUE *q);
  NYX_NET_MSG*  nyx_net_msg_clone (NYX_NET_MSG *msg);

#ifdef __cplusplus
}
#endif




#endif
