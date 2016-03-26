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

#include <nyx.h>
#include <nyx_list.h>

typedef struct my_type_t
{
  NYX_BASIC_ITEM bi;
  int            val;
} MY_TYPE;


int
main (int argc, char *argv[])
{
  NYX_LIST  *l;
  MY_TYPE   o1, o2, *p;

  nyx_init ();

  l = nyx_list_new ("test_list", 16, sizeof(MY_TYPE));
  
  p = &o1;
  OBJ_ID(p) = strdup ("Object1");
  p->val = 10;
  nyx_list_add_item (l, p);

  p = &o2;
  OBJ_ID(p) = strdup ("Object2");
  p->val = 20;
  nyx_list_add_item (l, p);

  nyx_list_dump (l);


  nyx_cleanup ();
  return 0;
}
