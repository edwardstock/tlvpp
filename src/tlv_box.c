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
#include "../include/tlv_box.h"

#include <stdio.h>
#include <string.h>

static void tlv_box_release_tlv(tlv_t* value) {
    free(value->value);
    free(value);
}

tlv_box_t* tlv_box_create() {
    tlv_box_t* box = (tlv_box_t*) calloc(1, sizeof(tlv_box_t));
    if (box == NULL) {
        return NULL;
    }
    box->m_list = key_list_create(tlv_box_release_tlv);
    box->m_serialized_buffer = NULL;
    box->m_serialized_bytes = 0;
    return box;
}

// internal usage
int8_t tlv_box_putobject(tlv_box_t* box, tlv_tag_t tag, const uint8_t* value, tlv_size_t length) {
    if (box->m_serialized_buffer != NULL) {
        return -1;
    }

    tlv_t* tlv = (tlv_t*) malloc(sizeof(tlv_t));
    tlv->tag = tag;
    tlv->length = length;
    tlv->value = (uint8_t*) malloc(length);
    if (tlv->value != NULL) {
        memcpy(tlv->value, value, length);
    }

    if (key_list_add(box->m_list, tag, tlv) != 0) {
        return -1;
    }
    box->m_serialized_bytes += sizeof(tlv_tag_t) + sizeof(tlv_size_t) + length;

    return 0;
}

tlv_box_t* tlv_box_parse(const uint8_t* buffer, tlv_size_t buffer_size) {
    tlv_box_t* box = tlv_box_create();

    uint8_t* cached = (uint8_t*) malloc(buffer_size);
    memcpy(cached, buffer, buffer_size);

    tlv_size_t offset = 0, length = 0;
    while (offset < buffer_size) {
        //        tlv_tag_t tag = (*(uint8_t*) (cached + offset));
        tlv_tag_t tag = to_num_le(cached + offset, sizeof(tlv_tag_t));
        offset += sizeof(tlv_tag_t);
        length = to_num_le(cached + offset, sizeof(tlv_size_t));
        //        length = (*(tlv_size_t*) (cached + offset));
        offset += sizeof(tlv_size_t);
        tlv_box_putobject(box, tag, cached + offset, length);
        offset += length;
    }

    box->m_serialized_buffer = cached;
    box->m_serialized_bytes = buffer_size;

    return box;
}

void tlv_box_destroy(tlv_box_t* box) {
    key_list_destroy(box->m_list);
    if (box->m_serialized_buffer != NULL) {
        free(box->m_serialized_buffer);
    }

    free(box);
    box = NULL;
}

uint8_t* tlv_box_get_buffer(tlv_box_t* box) {
    return box->m_serialized_buffer;
}

tlv_size_t tlv_box_get_size(tlv_box_t* box) {
    return box->m_serialized_bytes;
}

int8_t tlv_box_put_char(tlv_box_t* box, tlv_tag_t tag, int8_t value) {
    uint8_t d[1];
    to_bytes_le(d, value, 1);
    return tlv_box_putobject(box, tag, d, sizeof(int8_t));
}

int8_t tlv_box_put_uchar(tlv_box_t* box, tlv_tag_t tag, uint8_t value) {
    uint8_t d[1];
    to_bytes_le(d, value, 1);
    return tlv_box_putobject(box, tag, d, sizeof(uint8_t));
}

int8_t tlv_box_put_short(tlv_box_t* box, tlv_tag_t tag, int16_t value) {
    uint8_t d[2];
    to_bytes_le(d, value, 2);
    return tlv_box_putobject(box, tag, d, sizeof(int16_t));
}

int8_t tlv_box_put_ushort(tlv_box_t* box, tlv_tag_t tag, uint16_t value) {
    uint8_t d[2];
    to_bytes_le(d, value, 2);
    return tlv_box_putobject(box, tag, d, sizeof(uint16_t));
}

int8_t tlv_box_put_int(tlv_box_t* box, tlv_tag_t tag, int32_t value) {
    uint8_t d[4];
    to_bytes_le(d, value, 4);
    return tlv_box_putobject(box, tag, d, sizeof(int32_t));
}

int8_t tlv_box_put_int64(tlv_box_t* box, tlv_tag_t tag, int64_t value) {
    uint8_t d[8];
    to_bytes_le(d, value, 8);
    return tlv_box_putobject(box, tag, d, sizeof(int64_t));
}

int8_t tlv_box_put_uint64(tlv_box_t* box, tlv_tag_t tag, uint64_t value) {
    uint8_t d[8];
    to_bytes_le(d, value, 8);
    return tlv_box_putobject(box, tag, d, sizeof(uint64_t));
}

int8_t tlv_box_put_float(tlv_box_t* box, tlv_tag_t tag, float value) {
    uint8_t d[4];
    to_bytes_float_le(d, value);
    return tlv_box_putobject(box, tag, d, sizeof(float));
}

int8_t tlv_box_put_double(tlv_box_t* box, tlv_tag_t tag, double value) {
    uint8_t d[8];
    to_bytes_double_le(d, value);
    return tlv_box_putobject(box, tag, d, sizeof(double));
}

int8_t tlv_box_put_string(tlv_box_t* box, tlv_tag_t tag, const char* value) {
    return tlv_box_putobject(box, tag, (uint8_t*) value, strlen(value));
}

int8_t tlv_box_put_bytes(tlv_box_t* box, tlv_tag_t tag, const uint8_t* value, tlv_size_t length) {
    return tlv_box_putobject(box, tag, value, length);
}

int8_t tlv_box_put_object(tlv_box_t* box, tlv_tag_t tag, tlv_box_t* object) {
    return tlv_box_putobject(box, tag, tlv_box_get_buffer(object), tlv_box_get_size(object));
}

int8_t tlv_box_serialize(tlv_box_t* box) {
    if (box->m_serialized_buffer != NULL) {
        return -1;
    }

    tlv_size_t offset = 0;
    uint8_t* buffer = (uint8_t*) malloc(box->m_serialized_bytes);
    key_list_foreach(box->m_list, node) {
        tlv_t* tlv = node->value;
        memcpy(buffer + offset, &tlv->tag, sizeof(tlv_tag_t));
        offset += sizeof(tlv_tag_t);

        memcpy(buffer + offset, &tlv->length, sizeof(tlv_size_t));
        offset += sizeof(tlv_size_t);
        memcpy(buffer + offset, tlv->value, tlv->length);
        offset += tlv->length;
    }

    box->m_serialized_buffer = buffer;

    return 0;
}

int8_t tlv_box_get_uchar(tlv_box_t* box, tlv_tag_t tag, uint8_t* value) {
    tlv_t object;
    if (key_list_get(box->m_list, tag, &object) != 0) {
        return 0;
    }
    *value = (uint8_t) to_num_le(object.value, 1);
    return 1;
}

int8_t tlv_box_get_char(tlv_box_t* box, tlv_tag_t tag, char* value) {
    tlv_t object;
    if (key_list_get(box->m_list, tag, &object) != 0) {
        return 0;
    }
    *value = (char) to_num_le(object.value, 1);
    return 1;
}

int8_t tlv_box_get_short(tlv_box_t* box, tlv_tag_t tag, short* value) {
    tlv_t object;
    if (key_list_get(box->m_list, tag, &object) != 0) {
        return 0;
    }

    *value = to_num_le(object.value, 2);
    return 1;
}

int8_t tlv_box_get_ushort(tlv_box_t* box, tlv_tag_t tag, uint16_t* value) {
    tlv_t object;
    if (key_list_get(box->m_list, tag, &object) != 0) {
        return 0;
    }

    *value = to_num_le(object.value, 2);
    return 1;
}

int8_t tlv_box_get_int(tlv_box_t* box, tlv_tag_t tag, int32_t* value) {
    tlv_t object;
    if (key_list_get(box->m_list, tag, &object) != 0) {
        return 0;
    }

    *value = to_num_le(object.value, 4);
    return 1;
}

int8_t tlv_box_get_uint(tlv_box_t* box, tlv_tag_t tag, uint32_t* value) {
    tlv_t object;
    if (key_list_get(box->m_list, tag, &object) != 0) {
        return 0;
    }

    *value = to_num_le(object.value, 4);
    return 1;
}

int8_t tlv_box_get_int64(tlv_box_t* box, tlv_tag_t tag, int64_t* value) {
    tlv_t object;
    if (key_list_get(box->m_list, tag, &object) != 0) {
        return 0;
    }
    *value = to_num_le(object.value, 8);
    return 1;
}

int8_t tlv_box_get_uint64(tlv_box_t* box, tlv_tag_t tag, uint64_t* value) {
    tlv_t object;
    if (key_list_get(box->m_list, tag, &object) != 0) {
        return 0;
    }

    *value = to_num_le(object.value, 8);

    return 1;
}

int8_t tlv_box_get_float(tlv_box_t* box, tlv_tag_t tag, float* value) {
    tlv_t object;
    if (key_list_get(box->m_list, tag, &object) != 0) {
        return 0;
    }

    *value = to_float_le(object.value);
    return 1;
}

int8_t tlv_box_get_double(tlv_box_t* box, tlv_tag_t tag, double* value) {
    tlv_t object;
    if (key_list_get(box->m_list, tag, &object) != 0) {
        return 0;
    }
    *value = to_double_le(object.value);
    return 1;
}

int8_t tlv_box_get_string(tlv_box_t* box, tlv_tag_t tag, char* value, tlv_size_t* length) {
    tlv_t object;
    if (key_list_get(box->m_list, tag, &object) != 0) {
        return 0;
    }

    if (*length < object.length) {
        return 0;
    }
    memset(value, 0, *length);
    *length = object.length + 1;
    memcpy(value, object.value, object.length);
    value[*length] = '\0';
    return 1;
}

int8_t tlv_box_get_bytes(tlv_box_t* box, tlv_tag_t tag, uint8_t* value, tlv_size_t* length) {
    tlv_t object;
    if (key_list_get(box->m_list, tag, &object) != 0) {
        return 0;
    }

    if (*length < object.length) {
        return 0;
    }
    memset(value, 0, *length);
    *length = object.length;
    memcpy(value, object.value, object.length);
    return 1;
}

int8_t tlv_box_get_bytes_ptr(tlv_box_t* box, tlv_tag_t tag, uint8_t** value, tlv_size_t* length) {
    tlv_t object;
    if (key_list_get(box->m_list, tag, &object) != 0) {
        return 0;
    }

    *value = object.value;
    *length = object.length;
    return 1;
}

int8_t tlv_box_get_object(tlv_box_t* box, tlv_tag_t tag, tlv_box_t** object) {
    tlv_t value;
    if (key_list_get(box->m_list, tag, &value) != 0) {
        return 0;
    }
    *object = (tlv_box_t*) tlv_box_parse(value.value, value.length);
    return 1;
}

tlv_size_t tlv_box_value_length(tlv_box_t* box, tlv_tag_t tag) {
    tlv_t object;
    if (key_list_get(box->m_list, tag, &object) != 0) {
        return 0;
    }

    return object.length;
}