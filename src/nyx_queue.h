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


#ifndef NYX_QUEUE_H
#define NYX_QUEUE_H

#include <pthread.h>

typedef struct nyx_queue_t
{
  int              cap;   /* Queue capacity */
  int              n;     /* Number of items in the queue */
  void*            *item; /* Storage */
  int              head;
  int              tail;
  pthread_mutex_t  mutex;
  pthread_cond_t   empty;
  pthread_cond_t   full;
} NYX_QUEUE;

#ifdef __cplusplus
extern "C" {
#endif


  NYX_QUEUE* nyx_queue_new  (int size);
  int        nyx_queue_free (NYX_QUEUE *q);
  int        nyx_queue_add  (NYX_QUEUE *q, void *item);
  void*      nyx_queue_get  (NYX_QUEUE *q);
  void*      nyx_queue_peek (NYX_QUEUE *q);
  
  void*      nyx_queue_dbg_dump (NYX_QUEUE *q);

#ifdef __cplusplus
}
#endif


#endif
