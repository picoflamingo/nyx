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

#ifndef NYX_SERVER_H
#define NYX_SERVER_H

#include <nyx_list.h>
#include <nyx_queue.h>
#include <nyx_channel.h>
#include <nyx_net.h>

//#define BUF_SIZE 4096

/* NYX_WORKER to be moved into NYX_NET or its own file */
#include <pthread.h>

typedef struct nyx_worker_t
{
  pthread_t       tid;
  NYX_QUEUE       *q;
  pthread_mutex_t mutex;
  int             run;
  void            *args;
  void            *data;

} NYX_WORKER;


typedef void* (*NYX_WORKER_FUNC) (void *);

typedef struct nyx_server_t
{
  NYX_CHANNEL   *s;
  NYX_LIST      *clients;
  NYX_QUEUE     *q;
  NYX_WORKER    *w;
  NYX_NET       *net;
} NYX_SERVER;

#ifdef __cplusplus
extern "C" {
#endif

  NYX_SERVER*   nyx_server_new (char *ip, int port);
  int           nyx_server_free (NYX_SERVER *s);
  int           nyx_server_register (NYX_SERVER *s, NYX_NET *net, NYX_WORKER_FUNC f);

  int           nyx_server_printf   (NYX_SERVER *s, NYX_CHANNEL *c, char *fmt, ...);

  /* Accessors */
  NYX_CHANNEL*  nyx_server_get_channel (NYX_SERVER *s);
  NYX_QUEUE*    nyx_server_get_queue (NYX_SERVER *s);
  NYX_WORKER*   nyx_server_get_worker (NYX_SERVER *s);

  NYX_WORKER*   nyx_worker_new (NYX_QUEUE *q, void *data);
  int           nyx_worker_free (NYX_WORKER *w);

  int           nyx_worker_set_data (NYX_WORKER *w, void *data);
  void*         nyx_worker_get_data (NYX_WORKER *w);

  int           nyx_worker_start (NYX_WORKER *w, NYX_WORKER_FUNC f, void *arg);
  int           nyx_worker_stop  (NYX_WORKER *w);

#ifdef __cplusplus
}
#endif




#endif
