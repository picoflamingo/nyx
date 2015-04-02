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

#include "nyx_list.h"

NYX_LIST*  
nyx_list_new (char *id, int size, int item_size)
{
  NYX_LIST        *l;
  NYX_BASIC_ITEM  *bi;

  if (size < 0) return NULL;
  if ((l = (NYX_LIST*) malloc (sizeof(NYX_LIST))) == NULL)
    {
      fprintf (stderr, "Cannot allocate memory for list object\n");
      return NULL;
    }
  bi = (NYX_BASIC_ITEM*)l;
  bi->id = strdup (id);
  l->size = size;
  l->n = 0 ;
  l->item_size = item_size;
  l->item_free = NULL;
  if ((l->item = malloc (sizeof(void*) * size)) == NULL)
    {
      fprintf (stderr, "Cannot allocate memory for list items\n");
      free (l);
      return NULL;
    }
  
  return l;
}

int   
nyx_list_free (NYX_LIST *l)
{
  NYX_BASIC_ITEM *bi;

  if (!l) return -1;

  bi = (NYX_BASIC_ITEM*) l;

  if (bi->id) free (bi->id);
  if (!l->item_free) free (l->item);
  else
    {
      /* Delete items in list */
      fprintf (stderr, "Free item function not yet supported\n");
      free (l->item);
    }
  free (l);
  return 0;
}

int   
nyx_list_dump (NYX_LIST *l)
{
  NYX_BASIC_ITEM *bi;
  int         i;

  bi = (NYX_BASIC_ITEM*)l;
  printf ("List: '%s' with %d items\n", bi->id, l->n);
  for (i = 0; i < l->n; i++)
    {
      bi = (NYX_BASIC_ITEM*) l->item[i];
      printf ("ITEM: %d (%s)\n", i, bi->id);
    }

  return 0;
} 

int   
nyx_list_add_item (NYX_LIST *l, void* item)
{
  void *aux;

  if (!l) return -1;
  if (!item) return -1;

  /* Reallocate buffer if required */
  if (l->n == l->size) 
    {
#if 0
      fprintf (stderr, "List full... re-allocating...(%d ->%d)\n", 
	       l->size, l->size + SIZE_INC);
#endif      
      if ((aux = realloc (l->item, sizeof(void*) * (l->size + SIZE_INC))) == NULL)
	{
	  fprintf (stderr, "Cannot add element to list\n");
	  return -1;
	}
      l->size += SIZE_INC;
      l->item = aux;
    }

  l->item[l->n] = item;
  l->n++;
  return 0;
}


void*   
nyx_list_find_item (NYX_LIST *l, char *id)
{
  NYX_BASIC_ITEM*  bi;
  int   i;

  if (!l) return NULL;
  if (!id) return NULL;

  for (i = 0; i < l->n; i++)
    {
      bi = (NYX_BASIC_ITEM *) l->item[i];
      if (bi && !strcmp (bi->id, id)) return l->item[i];
    }
  return NULL;
}

void* 
nyx_list_get_item (NYX_LIST *l, int i)
{
  fprintf (stderr, "Function %s not yet implemented\n", __FUNCTION__);
  return 0;
}
