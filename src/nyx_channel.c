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
#include <string.h>
#include <stdlib.h>

#include <stdarg.h>

#include <math.h>

#include <unistd.h>


/* LInux specific*/
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>


#include <nyx_mbuf.h>
#include <nyx_channel.h>

/* Local functions */
/* This functions are called internally, c and buf are checked 
 * before calling the functions
 */
static ssize_t 
_default_write (NYX_CHANNEL* c, const void *buf, size_t count)
{
  if (c->s < 0 ) return -1;

  return write (c->s, buf, count);
}

static ssize_t 
_default_read  (NYX_CHANNEL* c, void *buf, size_t count)
{
  if (c->s < 0 ) return -1;

  return read (c->s, buf, count);
}


/* Resource format
 *  protocol://host:port[port1]/params
 *    protocols: tcp, udp, ssl
 *    host     : * -> server
 *               hostname, ip -> client
 *    port     : Port to connect or to listen to
 *    port1    : Optional only used for UDP to specify source port
 */
static int
_nyx_channel_init (NYX_CHANNEL *c)
{
#if 0
  char  *res;
  int   proto;
  char  *protos;
  char  *host;
  int   port1,port2;
  char  *arg;
  char  *aux, *aux1;
  char  *rd[5];

  if (!c) return -1;

  res = strdup (c->res);
  memset (rd, 0, 5);
  /* Parse resource */
  
  if ((aux = strchr (res, ':')) == NULL)
    {
      /* No colon provided... just a hostname */
      proto = NYX_DEFAULT_PROTO;
      port1 = NYX_DEFAULT_PORT;
      host = res;

    }
  else /* We had found at least one colon */
    {
      if ( (*(aux + 1) == '/') && ((*aux + 2) == '/'))
	{
	  /* It was a protocol */
	  
	}
      if ((aux1 = strchr (aux + 1, ':')) == NULL)
	{
	  fprintf (stderr, "Malformed resource name '%s'\n", res);
	  return -1;
	}
    }
  /* We have protocol and ports.... let's par it */
  protos = res;
  *aux = 0;
  printf ("Parsing Resource : %s\n", res);
  printf ("+ Protocol: %s\n", protos);
  /* Get host */
  if ((*(++aux) != '/') || (*(++aux)) != '/')
    {
      fprintf (stderr, "Malformed resource: Missing '//host'\n");
      return -1;
    }
  

  /* Call initialization function based on resource */
#endif
  return 0;
}

int
nyx_channel_udp (NYX_CHANNEL *c, char *host, int port, int sport)
{
  printf ("Not yet implemented\n");

  return 0;
}

int
nyx_channel_tcp_init (NYX_CHANNEL *c, char *host, int port, int is_server)
{
  struct sockaddr_in server;
  struct hostent     *he;
  int                ops = 1;

  if (!c) return -1;
  if (!host) return -1;
  if (port < 0) return -1;

  if ((c->s = socket (AF_INET, SOCK_STREAM, 0)) < 0)
    {
      perror ("nyx_channel_tcp_init (socket):");
      return -1;
    }
  c->proto = NYX_PROTO_TCP;
  c->port = port;

  server.sin_family = AF_INET;
  server.sin_port = htons (port);

  /* Resolve host name */
  if (host[0] == '*') 
    {
      server.sin_addr.s_addr = INADDR_ANY;
    }
  else
    {
      if ((he = gethostbyname (host)) == NULL)
	{
	  fprintf (stderr, "Cannot resolve host name '%s'. Error:%d\n", host, h_errno);
	  goto cleanup_and_exit;
	}
      server.sin_addr = *(struct in_addr *) he->h_addr;
    }


  if (is_server)
    {
      if ((setsockopt (c->s, SOL_SOCKET, SO_REUSEADDR, &ops, sizeof(ops))) < 0)
	perror ("nyx_channel_tcp_init (reuseaddr):");

      if ((bind (c->s, (struct sockaddr *) &server, sizeof(server)))< 0)
	{
	  perror ("nyx_channel_tcp_init (bind):");
	  goto cleanup_and_exit;
	}

      if ((listen (c->s, 10)) < 0) // XXX: Should be a parameter 
	{
	  perror ("nyx_channel_tcp_init (bind):");
	  goto cleanup_and_exit;
	}
		
    }
  else /* It is a client */
    {
      /* XXX: Make socket non-blocking... requires special code for connecting... */
      if ((connect (c->s, (struct sockaddr*)&server, sizeof(server))) < 0)
	{
	  perror ("nyx_channel_tcp_init (connect):");
	  goto cleanup_and_exit;
	}
    }

  return 0;
 cleanup_and_exit:
  close (c->s);
  c->s = -1;
  return -1;

}



/* Public interface */
NYX_CHANNEL* nyx_channel_new (char *res)
{
  NYX_CHANNEL *c;


  if ((c = malloc (sizeof(NYX_CHANNEL))) == NULL)
    {
      fprintf (stderr, "Cannot allocate memory for NYX_Channel\n");
      return NULL;
    }
  /* Initialize fields */
  if ((c->buf = nyx_mbuf_new (1024, NULL)) == NULL)
    {
      fprintf (stderr, "Cannot allocate internal buffer for NYX_Channel\n");
      free (c);
      return NULL;
    }

  c->s = -1;
  c->type = NYX_CHANNEL_NONE;
  c->res = NULL;
  c->proto = NYX_PROTO_NONE;
  c->priv = NULL;
  c->cb = NULL;
  c->check_cb = NULL;
  c->_write = _default_write;
  c->_read = _default_read;
  /*
  if (res)
    {
      c->res = strdup (res);
      //_nyx_channel_init (c);
    }
  */
  return c;
}


int          
nyx_channel_free (NYX_CHANNEL *c)
{
  if (!c) return -1;

  if (c->s >= 0) close (c->s);
  if (c->buf) nyx_mbuf_free (c->buf);
  if (c->res) free (c->res);

  free (c);


  return 0;
}

/* Accessor functions */
int          
nyx_channel_set_cb (NYX_CHANNEL *c, NYX_CHANNEL_CB func)
{
  if (!c) return -1;
  if (!func) return -1;

  c->cb = func;

  return 0;
}


int          
nyx_channel_set_check_cb (NYX_CHANNEL *c, NYX_CHANNEL_CHECK func)
{
  if (!c) return -1;
  if (!func) return -1;

  c->check_cb = func;

  return 0;

}

int          
nyx_channel_set_data (NYX_CHANNEL *c, void *data)
{
  if (!c) return -1;

  c->priv = data; /* Allows NULL */

  return 0;
}

void*        
nyx_channel_get_data (NYX_CHANNEL *c)
{
  if (!c) return NULL;

  return c->priv;
}


int          
nyx_channel_get_fd (NYX_CHANNEL *c)
{
  if (!c) return -1;

  return c->s;
}


/* Data access */
NYX_MBUF*    
nyx_channel_get_buffer (NYX_CHANNEL *c)
{
  if (!c) return NULL;

  return c->buf;
}

int          
nyx_channel_update (NYX_CHANNEL *c)
{
  char buffer[1024];
  int  rlen;

  if (!c) return -1;
  if ((rlen = c->_read (c, buffer, 1024)) < 0)
    {
      perror ("read:");
      return -1;
    }
  nyx_mbuf_append (c->buf, buffer, rlen);
  return rlen;
}


/* I/O Functions */
int          
nyx_channel_write (NYX_CHANNEL *c, void *buffer, int len)
{
  if (!c) return -1;
  
  return c->_write (c, buffer, len);
}

int          
nyx_channel_read  (NYX_CHANNEL *c, void *buffer, int len)
{
  if (!c) return -1;

  return c->_read (c, buffer, len);
}

int          
nyx_channel_print (NYX_CHANNEL *c, char *str)
{
  if (!c) return -1;
  if (!str) return -1;

  return c->_write (c, str, strlen(str));
}

int          
nyx_channel_read_line (NYX_CHANNEL *c, char *buffer, int len)
{
  char *aux;
  if (!c) return -1;

  if ((aux = nyx_mbuf_get_line (c->buf)) == NULL)
    return -1;
  if (strlen(aux) > len)
    {
      fprintf (stderr, "WARNING: Provided buffer not big enough... String truncated\n");
      strncpy (buffer, aux, len);
    }
  else
    strncpy (buffer, aux, strlen(aux));


  return 0;
}

char*        
nyx_channel_get_line (NYX_CHANNEL *c)
{
  if (!c) return NULL;
  /* Returned string is dynamically allocated. Caller has to free memory */
  return nyx_mbuf_get_line (c->buf);
}


int          
nyx_channel_connect (NYX_CHANNEL *c, char *res)
{
  if (!c) return -1;

  return 0;
}

NYX_CHANNEL*          
nyx_channel_accept (NYX_CHANNEL *c)
{
  NYX_CHANNEL        *cli;
  struct sockaddr_in client;
  socklen_t          slen = sizeof(struct sockaddr_in);
  int                s;

  if (!c) return NULL;
  if (!c->s) return NULL;

  /* We cannot acept connection on NYX_CHANNEL_SERVER */
  /* We may need to also check c->proto */
  if ((s = accept (c->s, (struct sockaddr*)&client, &slen)) < 0)
    {
      perror ("nyx_channel_accept (accept):");
      return NULL;
    }


  /* Create channel and initialize */
  cli = nyx_channel_new (NULL);
  cli->proto = c->proto;
  cli->s = s;

  return cli;
}


int          
nyx_channel_set_write_cb (NYX_CHANNEL *c, NYX_WRITE wcb)
{
  if (!c) return -1;

  c->_write = wcb; /* XXX: Allows NULL */

  return 0;
}

int          
nyx_channel_set_read_cb  (NYX_CHANNEL *c, NYX_READ  rcb)
{
  if (!c) return -1;

  c->_read = rcb;
  return 0;
}



/* I/O Helper functions */
int          
nyx_channel_pritnf (NYX_CHANNEL *c, char *fmt,...)
{
  if (!c) return -1;

  return 0;
}
/* Low level functions: Sockets */

