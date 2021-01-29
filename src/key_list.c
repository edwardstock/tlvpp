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
#include "../include/key_list.h"

tlv_key_list_t* key_list_create(value_releaser releaser) {
    tlv_key_list_t* list = (tlv_key_list_t*) malloc(sizeof(tlv_key_list_t));
    list->count = 0;
    list->header = NULL;
    list->releaser = releaser;
    return list;
}

int key_list_destroy(tlv_key_list_t* list) {
    key_list_node_t* current = list->header;
    while (current != NULL) {
        key_list_node_t* next = current->next;
        list->releaser(current->value);
        free(current);
        current = next;
    }
    free(list);
    return 0;
}

int key_list_count(tlv_key_list_t* list) {
    return list->count;
}

int key_list_keyset(tlv_key_list_t* list, tlv_tag_t* array, int array_size) {
    if (array_size < list->count) {
        return -1;
    }

    int i = 0;
    key_list_node_t* current = list->header;
    while (current != NULL) {
        array[i] = current->key;
        current = current->next;
        i++;
    }

    return i;
}

static key_list_node_t* key_list_get_node(tlv_key_list_t* list, tlv_tag_t key) {
    key_list_node_t* current = list->header;
    while (current != NULL) {
        if (key_compare(key, current->key)) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

static int key_list_remove_node(tlv_key_list_t* list, key_list_node_t* node) {
    if (node == list->header) {
        list->header = node->next;
    } else {
        node->prev->next = node->next;
    }

    if (node->next != NULL) {
        node->next->prev = node->prev;
    }

    list->releaser(node->value);

    free(node);
    list->count--;

    return 0;
}

int key_list_find_key(tlv_key_list_t* list, tlv_tag_t key) {
    return key_list_get_node(list, key) != NULL;
}

int key_list_add(tlv_key_list_t* list, tlv_tag_t key, tlv_t* value) {

    key_list_node_t* node = (key_list_node_t*) calloc(1, sizeof(key_list_node_t));
    if (node == NULL) {
        return -1;
    }

    node->key = key;
    node->value = value;

    if (list->header == NULL) {
        list->header = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        key_list_node_t* old_tail = list->tail;
        list->tail = node;
        node->prev = old_tail;
    }
    list->count++;

    return 0;
}

int key_list_get(tlv_key_list_t* list, tlv_tag_t key, tlv_t* value) {
    key_list_node_t* node = key_list_get_node(list, key);
    if (node == NULL) {
        return -1;
    }
    *value = *(node)->value;
    return 0;
}

int key_list_edit(tlv_key_list_t* list, tlv_tag_t key, tlv_t* value) {
    key_list_node_t* node = key_list_get_node(list, key);
    if (node == NULL) {
        return -1;
    }
    *(node)->value = *value;
    return 0;
}

int key_list_delete(tlv_key_list_t* list, tlv_tag_t key) {
    key_list_node_t* node = key_list_get_node(list, key);
    if (node == NULL) {
        return -1;
    }

    return key_list_remove_node(list, node);
}
