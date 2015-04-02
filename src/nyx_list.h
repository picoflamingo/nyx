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


#ifndef NYX_LIST_H
#define NYX_LIST_H

#define SIZE_INC  16

typedef struct nyx_basic_item_t
{
  char   *id;
} NYX_BASIC_ITEM;


typedef struct nyx_list_t
{
  NYX_BASIC_ITEM bi;
  int            size;
  int            n;
  int            item_size;
  int            (*item_free) (void *);
  void            **item;
} NYX_LIST;

#define OBJ_ID(p) ((NYX_BASIC_ITEM*)p)->id

#ifdef __cplusplus
extern "C" {
#endif

  NYX_LIST* nyx_list_new (char *id, int size, int item_size);
  int       nyx_list_free (NYX_LIST *l);
  int       nyx_list_dump (NYX_LIST *l);

  int       nyx_list_add_item  (NYX_LIST *l, void* item);
  void*     nyx_list_find_item (NYX_LIST *l, char *id);
  void*     nyx_list_get_item  (NYX_LIST *l, int i);


#ifdef __cplusplus
}
#endif


#endif
