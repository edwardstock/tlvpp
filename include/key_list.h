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

#include "data.h"

#include <inttypes.h>
#include <stdlib.h>

tlv_key_list_t* key_list_create(value_releaser releaser);
int key_list_destroy(tlv_key_list_t* list);

int key_list_count(tlv_key_list_t* list);
int key_list_keyset(tlv_key_list_t* list, tlv_tag_t* array, int array_size);
int key_list_find_key(tlv_key_list_t* list, tlv_tag_t key);

int key_list_add(tlv_key_list_t* list, tlv_tag_t key, tlv_t* value);
int key_list_get(tlv_key_list_t* list, tlv_tag_t key, tlv_t* value);
int key_list_edit(tlv_key_list_t* list, tlv_tag_t key, tlv_t* value);
int key_list_delete(tlv_key_list_t* list, tlv_tag_t key);

#define key_list_foreach(L, V)                                                                                         \
    key_list_node_t* _node = NULL;                                                                                     \
    key_list_node_t* V;                                                                                                \
    for (V = _node = L->header; _node != NULL; V = _node = _node->next)

#ifdef __cplusplus
}
#endif

#endif
