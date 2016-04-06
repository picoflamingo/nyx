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

#include <pthread.h>
#include <nyx_queue.h>

NYX_QUEUE* 
nyx_queue_new (int size)
{
  NYX_QUEUE   *q;
  
  if ((q = malloc (sizeof(struct nyx_queue_t))) == NULL)
    {
      fprintf (stderr, "Cannot allocate memory for Queue Object\n");
      return NULL;
    }

  q->cap = size;
  q->n = 0;

  if ((q->item = malloc (sizeof(void *) * size)) == NULL)
    {
      fprintf (stderr, "Cannot allocate memory for Queue buffer\n");
      free (q);
      return NULL;
    }
  
  q->head = q->tail = 0;
  /* Concurrent Objects Initialization */
  pthread_mutex_init (&q->mutex, NULL);
  pthread_cond_init  (&q->empty, NULL);
  pthread_cond_init  (&q->full, NULL);
  pthread_cond_broadcast (&q->empty);
  return q;
}

int        
nyx_queue_free (NYX_QUEUE *q)
{
  if (!q) return -1;

  if (q->item) free (q->item);
  free (q);

  return 0;
}

int        
nyx_queue_add  (NYX_QUEUE *q, void *item)
{
  if (!q) return -1;
  if (!item) return -1; /* NULL objects cannot be queued*/

  /* Enter Critical Region*/
  pthread_mutex_lock (&q->mutex);

  /* If the queue is full we have to wait to add our item
   * XXX: Check if we really need the while
   */
  while (q->n >= q->cap)
    pthread_cond_wait (&q->full, &q->mutex);
  
  /* At this point the queue is locked and there is available room*/
  q->item[q->tail] = item;
  q->n++;
  q->tail++;
  q->tail %= q->cap;

  /* Wake up threads waiting for data, if any */

  pthread_cond_broadcast (&q->empty);

  pthread_mutex_unlock (&q->mutex);
  return 0;
}


void*      
nyx_queue_get (NYX_QUEUE *q)
{
  void *item;

  if (!q) return NULL;

  /* Wait for mutual exclusion access */
  pthread_mutex_lock (&q->mutex);

  /* If the queue is empty... wait for data */
  while (q->n == 0)
    pthread_cond_wait (&q->empty, &q->mutex);
  
  /* Retrieve item from queue */
  item = q->item[q->head];
  q->n--;
  q->head++;
  q->head %= q->cap;

  /* Wake up threads waiting to store data */
  pthread_cond_broadcast (&q->full);

  /* Release queue. Leaves critical region */
  pthread_mutex_unlock (&q->mutex);

  return item;
}

void*      
nyx_queue_peek (NYX_QUEUE *q)
{
  void *item = NULL;

  if (!q) return NULL;

  /* Wait for mutual exclusion access */
  pthread_mutex_lock (&q->mutex);

  /* Retrieve item from queue */
  if (q->n > 0)
    item = q->item[q->head];

  /* Release queue. Leaves critical region */
  pthread_mutex_unlock (&q->mutex);

  return item;
}

void*      
nyx_queue_dbg_dump (NYX_QUEUE *q)
{
  int   i, indx;

  if (!q) return NULL;

  /* Wait for mutual exclusion access */
  pthread_mutex_lock (&q->mutex);

  printf ("Capacity: %d | #Items: %d | Head: %d | Tail: %d\n",
	  q->cap, q->n, q->head, q->tail);
  for (i = 0, indx = q->tail; i < q->n; i++)
    {
      printf ("Item %d (%d): %p\n", i, indx, q->item[indx]);
      indx++;
      indx %= q->cap;
    }

  /* Release queue. Leaves critical region */
  pthread_mutex_unlock (&q->mutex);

  

  return NULL;
}



