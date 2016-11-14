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


#ifndef NYX_CHANNEL_H
#define NYX_CHANNEL_H

#include <nyx_mbuf.h>

#define NYX_DEFAULT_PORT 80

#define NYX_PROTO_NONE    0
#define NYX_PROTO_TCP     1
#define NYX_PROTO_UDP     2
#define NYX_PROTO_SSL     3
#define NYX_PROTO_MAX     NYX_PROTO_SSL




#define NYX_DEFAULT_PROTO NYX_PROTO_TCP

#define BUF_SIZE 4096

typedef struct nyx_msg_t
{
  void  *data;  /* Change it to a NYX_MBUF???? */
  int   size;
  void  *priv;  /* Private data... will contain NYX_CHANNEL for networking*/
} NYX_MSG;


#define NYX_CHANNEL_NONE   0
#define NYX_CHANNEL_SERVER 1
#define NYX_CHANNEL_CLIENT 2

struct nyx_channel_t;

typedef int     (*NYX_CHANNEL_CB) (struct nyx_channel_t* c, void* arg);
typedef NYX_MSG *(*NYX_CHANNEL_CHECK) (struct nyx_channel_t* c);
typedef ssize_t (*NYX_READ)     (struct nyx_channel_t* c, void *buf, size_t count);
typedef ssize_t (*NYX_WRITE)    (struct nyx_channel_t* c, const void *buf, size_t count);

typedef struct nyx_channel_t
{
  int               s;        /* File Descriptor*/
  int               type;     /* Type of channel*/
  char              *res;     /* Resouce String */
  int               port;
  int               proto;    /* Protocol identifier */
  void              *pro_data;/* Stores protocol specific information */
  void              *priv;    /* Private data */
  NYX_MBUF          *buf;     /* Data buffer*/
  NYX_CHANNEL_CB    cb;       /* Event callback fro NYX_NET*/
  NYX_CHANNEL_CHECK check_cb;  /* Callback to check if a msg is available*/
  /* Read/write function pointer for overwrite  */
  NYX_READ          _read;
  NYX_WRITE         _write;

  /*
  ssize_t (*_read)  (struct mobinet_channel_t* c, void *buf, size_t count);
  ssize_t (*_write) (struct mobinet_channel_t* c, const void *buf, size_t count);
  */
} NYX_CHANNEL;

#ifdef __cplusplus
extern "C" {
#endif


  NYX_CHANNEL* nyx_channel_new (char *res);
  int          nyx_channel_free (NYX_CHANNEL *c);

  /* Accessor functions */
  int          nyx_channel_set_cb (NYX_CHANNEL *c, NYX_CHANNEL_CB func);
  int          nyx_channel_set_check_cb (NYX_CHANNEL *c, NYX_CHANNEL_CHECK func);
  int          nyx_channel_set_data (NYX_CHANNEL *c, void *data);
  void*        nyx_channel_get_data (NYX_CHANNEL *c);
  int          nyx_channel_get_fd (NYX_CHANNEL *c);

  /* Data access */
  NYX_MBUF*    nyx_channel_get_buffer (NYX_CHANNEL *c);
  int          nyx_channel_update (NYX_CHANNEL *c);
  
  /* I/O Functions */
  int          nyx_channel_write (NYX_CHANNEL *c, void *buffer, int len);
  int          nyx_channel_read  (NYX_CHANNEL *c, void *buffer, int len);
  int          nyx_channel_print (NYX_CHANNEL *c, char *str);
  int          nyx_channel_read_line (NYX_CHANNEL *c, char *buffer, int len);
  char*        nyx_channel_get_line (NYX_CHANNEL *c);

  int          nyx_channel_connect (NYX_CHANNEL *c, char *res);
  NYX_CHANNEL* nyx_channel_accept (NYX_CHANNEL *c);

  int          nyx_channel_set_write_cb (NYX_CHANNEL *c, NYX_WRITE wcb);
  int          nyx_channel_set_read_cb  (NYX_CHANNEL *c, NYX_READ  rcb);
  

  /* I/O Helper functions */
  int          nyx_channel_printf (NYX_CHANNEL *c, char *fmt,...);

  int          nyx_channel_tcp_init (NYX_CHANNEL *c, char *host, int port, int is_server);

  /* Low level functions: Sockets */

#ifdef __cplusplus
}
#endif


#endif
