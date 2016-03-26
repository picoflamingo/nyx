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


#ifndef NYX_WORKER_H
#define NYX_WORKER_H

#include <pthreads.h>

#include <nyx_queue.h>

/* Worker main function prototype */
struct nyx_worker_t;
typedef int (*NYX_WORKER_FUNC)(struct nyx_worker_t *w);

typedef struct nyx_worker_t
{
  pthread_t       tid;    /* Thread identification */
  NYX_CQUEUE      *q;     /* Associated concurrent queue */
  void            *data;  /* private data*/
  NYX_WORKER_FUNC f;      /* Worker task*/
  int             state;  /* Worker state */
} NYX_WORKER;

typedef struct nyx_cqueue_t
{
  NYX_QUEUE q;
  pthread_mutex_t  mutex;
  pthread_cond_t   data_available;
  pthread_cont_t   full;
} NYX_CQUEUE;

#ifdef __cplusplus
extern "C" {
#endif


  NYX_WORKER* nyx_worker_new ();
  int         nyx_worker_free (NYX_WORKER *w);
  
  int         nyx_worker_set_cqueue (NYX_WORKER w, NYX_CQUEUE q);
  int         nyx_worker_set_func (NYX_WORKER w, NYX_WORKER_FUNC f);
  int         nyx_worker_set_priv_data (NYX_WORKER w, void *data);
  void*       nyx_worker_get_priv_data (NYX_WORKER w);

  int         nyx_worker_run (NYX_WORKER w);
  int         nyx_worker_stop (NYX_WORKER w);

#ifdef __cplusplus
}
#endif

#endif
