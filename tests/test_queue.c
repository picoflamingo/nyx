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
#include <nyx_queue.h>

typedef struct producer_arg_t
{
  NYX_QUEUE *q;
  char*     prefix;
  int       t;
} PROD_ARG;

void*
producer (void *arg)
{
  char   buf[1024];
  int    c = 0;

  PROD_ARG *a = (PROD_ARG*) arg;

  while (1)
    {
      memset (buf, 0, 1024);
      snprintf (buf, 1024, "%s-%d", a->prefix, c);
      printf ("Worker (%s): Adding '%s'\n", a->prefix, buf);
      c++;
      nyx_queue_add (a->q, strdup(buf));
      usleep (a->t);
    }
}

void*
consumer (void *arg)
{
  char *buf;
  NYX_QUEUE *q = (NYX_QUEUE*) arg;
  while (1)
    {
      buf = (char*) nyx_queue_get (q);
      printf ("CONSUMER: '%s'\n", buf);
      free (buf);
      usleep (400000);
    }
}

int
main (int argc, char *argv[])
{
  NYX_QUEUE  *q;
  PROD_ARG   p1, p2, p3;
  pthread_t  p1_tid, p2_tid, p3_tid;
  pthread_t  c1_tid, c2_tid;

  nyx_init ();

  q = nyx_queue_new (50);
  pthread_create (&c1_tid, NULL, consumer, q);
  pthread_create (&c2_tid, NULL, consumer, q);

  p1.t = 100000;
  p1.prefix = "Producer 1";
  p1.q = q;
  pthread_create (&p1_tid, NULL, producer, &p1);

  p2.t = 200000;
  p2.prefix = "Producer 2";
  p2.q = q;
  pthread_create (&p2_tid, NULL, producer, &p2);

  p3.t = 50000;
  p3.prefix = "Producer 3";
  p3.q = q;
  pthread_create (&p3_tid, NULL, producer, &p3);




  /* Run for a while */
  sleep (10);

  nyx_queue_dbg_dump (q);

  nyx_cleanup ();

  return 0;
}
