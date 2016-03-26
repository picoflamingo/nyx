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

#ifndef NYX_MBUF_H
#define NYX_MBUF_H

#define NYX_REALLOC_INC  1024

typedef struct nyx_mbuf_t
{
  char *buf;
  int  size;  /* Buffer size*/
  int  len;   /* Lenght of data */
} NYX_MBUF;

#ifdef __cplusplus
extern "C" {
#endif

  NYX_MBUF*        nyx_mbuf_new        (int size, char *str);
  int              nyx_mbuf_free       (NYX_MBUF *b);

  int              nyx_mbuf_append     (NYX_MBUF *b, void *buf, int len);
  int              nyx_mbuf_append_txt (NYX_MBUF *b, char *str);
  char*            nyx_mbuf_peek_line  (NYX_MBUF *b);
  char*            nyx_mbuf_get_line   (NYX_MBUF *b);
  int              nyx_mbuf_printf     (NYX_MBUF *b, char *fmt, ...);

  int              nyx_mbuf_get_len    (NYX_MBUF *b);
  int              nyx_mbuf_get_size   (NYX_MBUF *b);
  void*            nyx_mbuf_get_data   (NYX_MBUF *b);


#ifdef __cplusplus
}
#endif


#endif
