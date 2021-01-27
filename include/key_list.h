/*
 *  COPYRIGHT NOTICE
 *  Copyright (C) 2015, Jhuster, All Rights Reserved
 *  Author: Jhuster(lujun.hust@gmail.com)
 *
 *  https://github.com/Jhuster/TLV
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published
 *  by the Free Software Foundation; either version 2.1 of the License,
 *  or (at your option) any later version.
 */
#ifndef _KEY_LIST_H_
#define _KEY_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include <stdlib.h>

typedef struct _value {
  void *value;
} value_t;

typedef uint8_t tlv_key_t;
typedef void (*value_releaser)(value_t value);

#define key_compare(a, b) ((a == b) ? 1 : 0)

typedef struct key_list_node {
  tlv_key_t key;
  value_t value;
  struct key_list_node *prev;
  struct key_list_node *next;
} key_list_node_t;

typedef struct key_list {
  int count;
  key_list_node_t *header;
  value_releaser releaser;
} tlv_key_list_t;

tlv_key_list_t *key_list_create(value_releaser releaser);
int key_list_destroy(tlv_key_list_t *list);

int key_list_count(tlv_key_list_t *list);
int key_list_keyset(tlv_key_list_t *list, tlv_key_t *array, int array_size);
int key_list_find_key(tlv_key_list_t *list, tlv_key_t key);

int key_list_add(tlv_key_list_t *list, tlv_key_t key, value_t value);
int key_list_get(tlv_key_list_t *list, tlv_key_t key, value_t *value);
int key_list_edit(tlv_key_list_t *list, tlv_key_t key, value_t value);
int key_list_delete(tlv_key_list_t *list, tlv_key_t key);

#define key_list_foreach(L, V)                                                 \
  key_list_node_t *_node = NULL;                                               \
  key_list_node_t *V;                                                          \
  for (V = _node = L->header; _node != NULL; V = _node = _node->next)

#ifdef __cplusplus
}
#endif

#endif
