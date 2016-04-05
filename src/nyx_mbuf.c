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
#include <stdarg.h>

#include <assert.h>

#include "nyx_mbuf.h"


NYX_MBUF*         
nyx_mbuf_new        (int size, char *str)
{
  NYX_MBUF  *aux;

  if (!str && size <= 0) return NULL;

  /* Allocate MBUF object*/
  if ((aux = malloc (sizeof (NYX_MBUF))) == NULL)
    {
      fprintf (stderr, "Cannot allocate memory!!\n");
      return NULL;
    }

  aux->len = 0;
  /* XXX: Use strnlen? */

  if (str) aux->size = strlen (str);
  else aux->size = size;

  /* Allocate internal Buffer */
  if ((aux->buf = malloc (aux->size)) == NULL)
    {
      fprintf (stderr, "Cannot allocate internal memory for MBUF\n");
      free (aux);
      return NULL;
    }

  memset (aux->buf, 0, aux->size);

  if (str) 
    {
      strcpy (aux->buf, str);
      aux->len = aux->size;
    }

  return aux;
}

int              
nyx_mbuf_free       (NYX_MBUF *b)
{
  if (!b) return -1;
  if (!b->buf) return -1;

  free (b->buf);
  free (b);

  return 0;
}

int              
nyx_mbuf_append     (NYX_MBUF *b, void *buf, int len)
{
  int    v1, v2;
  char  *aux;

  if (!b) return -1;
  if (!buf) return -1;
  if (len <=0) return -1;


  v1 = b->len;
  v2 = b->size;

  if ((v2 - v1) < len) /* Not enough space. Realloc */
    {
      while ((v2 - v1) < len) v2 += NYX_REALLOC_INC;
      if ((aux = realloc (b->buf, v2)) == NULL)
	{
	  fprintf (stderr, "Cannot reallocate buffer for update\n");
	  return -1;
	}
      /* realloc succesfull. Update object */
      b->buf = aux;

      memset (b->buf + b->size + 1, 0, v2 - b->size);
      b->size = v2;
    }
  /* Append data to the buffer */

  memcpy (b->buf + b->len, buf, len);

  b->len += len;
  return 0;
}


int              
nyx_mbuf_append_txt (NYX_MBUF *b, char *str)
{
  if (!b) return -1;
  if (!str) return -1;

  nyx_mbuf_append (b, str, strlen(str));
  return 0;
}

char*            
nyx_mbuf_peek_line   (NYX_MBUF *b)
{
  int   l;
  char  *aux, *ret, c;

  if (!b) return NULL;
  if (!b->buf) return NULL;
  if (b->len == 0) return NULL;
  if ((aux = strchr (b->buf, '\n')) == NULL) return NULL;
  //c = *aux;
  //*aux = 0;

  l = aux - b->buf + 2;  

  //ret = strdup (b->buf);
  ret = malloc (l);
  memset (ret, 0, l);
  strncpy (ret, b->buf, l-1);
  //*aux = c;

  return ret;
}

char*            
nyx_mbuf_get_line   (NYX_MBUF *b)
{
  char  *ret;
  int   len;

  if ((ret = nyx_mbuf_peek_line (b)) == NULL) return NULL;
  len = strlen (ret);

  memmove (b->buf, b->buf + len, b->len - len);
  b->len -= len;
  memset (b->buf + b->len + 1, 0, len);
  
  return ret;
}


int              
nyx_mbuf_printf     (NYX_MBUF *b, char *fmt, ...)
{
  /*
  char    buf[IBUF_SIZE2];
  int     len;
  va_list arg;

  if (!c) return -1;
  if (!fmt) return -1;
  va_start (arg, fmt);

  memset (buf, 0, IBUF_SIZE2);
  len = vsnprintf (buf, IBUF_SIZE2, fmt, arg);
  if (len >= IBUF_SIZE2)
    {
      fprintf (stderr, "%s: Output truncated!!!\n", __FUNCTION__);
      buf[IBUF_SIZE2 - 1] = 0;
    }

  len = mn_write (c, buf, len);

  va_end (arg);
  */
  return 0;
}


int              
nyx_mbuf_get_len    (NYX_MBUF *b)
{
  if (!b) return -1;
  else return b->len;
}

int              
nyx_mbuf_get_size    (NYX_MBUF *b)
{
  if (!b) return -1;
  else return b->size;
}


void*            
nyx_mbuf_get_data   (NYX_MBUF *b)
{
  if (!b) return NULL;
  else return b->buf;

  return 0;
}

