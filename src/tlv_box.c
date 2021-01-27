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

static void tlv_box_release_tlv(value_t value) {
  tlv_t *tlv = (tlv_t *)value.value;
  free(tlv->value);
  free(tlv);
}

tlv_box_t *tlv_box_create() {
  tlv_box_t *box = (tlv_box_t *)malloc(sizeof(tlv_box_t));
  box->m_list = key_list_create(tlv_box_release_tlv);
  box->m_serialized_buffer = NULL;
  box->m_serialized_bytes = 0;
  return box;
}

// internal usage
int8_t tlv_box_putobject(tlv_box_t *box, tlv_tag_t tag, const void *value,
                         tlv_size_t length) {
  if (box->m_serialized_buffer != NULL) {
    return -1;
  }

  tlv_t *tlv = (tlv_t *)malloc(sizeof(tlv_t));
  tlv->tag = tag;
  tlv->length = length;
  tlv->value = (uint8_t *)malloc(length);
  memcpy(tlv->value, value, length);

  value_t object;
  object.value = tlv;

  if (key_list_add(box->m_list, tag, object) != 0) {
    return -1;
  }
  box->m_serialized_bytes += sizeof(tlv_tag_t) + sizeof(tlv_size_t) + length;

  return 0;
}

tlv_box_t *tlv_box_parse(uint8_t *buffer, tlv_size_t buffersize) {
  tlv_box_t *box = tlv_box_create();

  uint8_t *cached = (uint8_t *)malloc(buffersize);
  memcpy(cached, buffer, buffersize);

  tlv_size_t offset = 0, length = 0;
  while (offset < buffersize) {
    uint8_t type = (*(uint8_t *)(cached + offset));
    offset += sizeof(tlv_tag_t);
    length = (*(tlv_size_t *)(cached + offset));
    offset += sizeof(tlv_size_t);
    tlv_box_putobject(box, type, cached + offset, length);
    offset += length;
  }

  box->m_serialized_buffer = cached;
  box->m_serialized_bytes = buffersize;

  return box;
}

void tlv_box_destroy(tlv_box_t *box) {
  key_list_destroy(box->m_list);
  if (box->m_serialized_buffer != NULL) {
    free(box->m_serialized_buffer);
  }

  free(box);
  box = NULL;
}

uint8_t *tlv_box_get_buffer(tlv_box_t *box) { return box->m_serialized_buffer; }

tlv_size_t tlv_box_get_size(tlv_box_t *box) { return box->m_serialized_bytes; }

int8_t tlv_box_put_char(tlv_box_t *box, tlv_tag_t tag, int8_t value) {
  return tlv_box_putobject(box, tag, &value, sizeof(char));
}

int8_t tlv_box_put_uchar(tlv_box_t *box, tlv_tag_t tag, uint8_t value) {
  return tlv_box_putobject(box, tag, &value, sizeof(uint8_t));
}

int8_t tlv_box_put_short(tlv_box_t *box, tlv_tag_t tag, int16_t value) {
  return tlv_box_putobject(box, tag, &value, sizeof(short));
}

int8_t tlv_box_put_ushort(tlv_box_t *box, tlv_tag_t tag, uint16_t value) {
  return tlv_box_putobject(box, tag, &value, sizeof(uint16_t));
}

int8_t tlv_box_put_int(tlv_box_t *box, tlv_tag_t tag, int value) {
  return tlv_box_putobject(box, tag, &value, sizeof(int));
}

int8_t tlv_box_put_long(tlv_box_t *box, tlv_tag_t tag, int64_t value) {
  return tlv_box_putobject(box, tag, &value, sizeof(long));
}

int8_t tlv_box_put_longlong(tlv_box_t *box, tlv_tag_t tag, long long value) {
  return tlv_box_putobject(box, tag, &value, sizeof(long long));
}

int8_t tlv_box_put_float(tlv_box_t *box, tlv_tag_t tag, float value) {
  return tlv_box_putobject(box, tag, &value, sizeof(float));
}

int8_t tlv_box_put_double(tlv_box_t *box, tlv_tag_t tag, double value) {
  return tlv_box_putobject(box, tag, &value, sizeof(double));
}

int8_t tlv_box_put_string(tlv_box_t *box, tlv_tag_t tag, const char *value) {
  return tlv_box_putobject(box, tag, value, strlen(value) + 1);
}

int8_t tlv_box_put_bytes(tlv_box_t *box, tlv_tag_t tag, uint8_t *value,
                         tlv_size_t length) {
  return tlv_box_putobject(box, tag, value, length);
}

int8_t tlv_box_put_object(tlv_box_t *box, tlv_tag_t tag, tlv_box_t *object) {
  return tlv_box_putobject(box, tag, tlv_box_get_buffer(object),
                           tlv_box_get_size(object));
}

int8_t tlv_box_serialize(tlv_box_t *box) {
  if (box->m_serialized_buffer != NULL) {
    return -1;
  }

  tlv_size_t offset = 0;
  uint8_t *buffer = (uint8_t *)malloc(box->m_serialized_bytes);
  key_list_foreach(box->m_list, node) {
    tlv_t *tlv = (tlv_t *)node->value.value;
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

int8_t tlv_box_get_uchar(tlv_box_t *box, tlv_tag_t tag, uint8_t *value) {
  value_t object;
  if (key_list_get(box->m_list, tag, &object) != 0) {
    return 0;
  }
  tlv_t *tlv = (tlv_t *)object.value;
  *value = (*(uint8_t *)(tlv->value));
  return 1;
}

int8_t tlv_box_get_char(tlv_box_t *box, tlv_tag_t tag, char *value) {
  value_t object;
  if (key_list_get(box->m_list, tag, &object) != 0) {
    return 0;
  }
  tlv_t *tlv = (tlv_t *)object.value;
  *value = (*(char *)(tlv->value));
  return 1;
}

int8_t tlv_box_get_short(tlv_box_t *box, tlv_tag_t tag, short *value) {
  value_t object;
  if (key_list_get(box->m_list, tag, &object) != 0) {
    return 0;
  }
  tlv_t *tlv = (tlv_t *)object.value;
  *value = (*(short *)(tlv->value));
  return 1;
}

int8_t tlv_box_get_ushort(tlv_box_t *box, tlv_tag_t tag, uint16_t *value) {
  value_t object;
  if (key_list_get(box->m_list, tag, &object) != 0) {
    return 0;
  }
  tlv_t *tlv = (tlv_t *)object.value;
  *value = (*(uint16_t *)(tlv->value));
  return 1;
}

int8_t tlv_box_get_int(tlv_box_t *box, tlv_tag_t tag, int32_t *value) {
  value_t object;
  if (key_list_get(box->m_list, tag, &object) != 0) {
    return 0;
  }
  tlv_t *tlv = (tlv_t *)object.value;
  *value = (*(int *)(tlv->value));
  return 1;
}

int8_t tlv_box_get_long(tlv_box_t *box, tlv_tag_t tag, int64_t *value) {
  value_t object;
  if (key_list_get(box->m_list, tag, &object) != 0) {
    return 0;
  }
  tlv_t *tlv = (tlv_t *)object.value;
  *value = (*(long *)(tlv->value));
  return 1;
}

int8_t tlv_box_get_longlong(tlv_box_t *box, tlv_tag_t tag, long long *value) {
  value_t object;
  if (key_list_get(box->m_list, tag, &object) != 0) {
    return 0;
  }
  tlv_t *tlv = (tlv_t *)object.value;
  *value = (*(long long *)(tlv->value));
  return 1;
}

int8_t tlv_box_get_float(tlv_box_t *box, tlv_tag_t tag, float *value) {
  value_t object;
  if (key_list_get(box->m_list, tag, &object) != 0) {
    return 0;
  }
  tlv_t *tlv = (tlv_t *)object.value;
  *value = (*(float *)(tlv->value));
  return 1;
}

int8_t tlv_box_get_double(tlv_box_t *box, tlv_tag_t tag, double *value) {
  value_t object;
  if (key_list_get(box->m_list, tag, &object) != 0) {
    return 0;
  }
  tlv_t *tlv = (tlv_t *)object.value;
  *value = (*(double *)(tlv->value));
  return 1;
}

int8_t tlv_box_get_string(tlv_box_t *box, tlv_tag_t tag, char *value,
                          tlv_size_t *length) {
  return tlv_box_get_bytes(box, tag, (uint8_t *)value, length);
}

int8_t tlv_box_get_bytes(tlv_box_t *box, tlv_tag_t tag, uint8_t *value,
                         tlv_size_t *length) {
  value_t object;
  if (key_list_get(box->m_list, tag, &object) != 0) {
    return 0;
  }
  tlv_t *tlv = (tlv_t *)object.value;
  if (*length < tlv->length) {
    return 0;
  }
  memset(value, 0, *length);
  *length = tlv->length;
  memcpy(value, tlv->value, tlv->length);
  return 1;
}

int8_t tlv_box_get_bytes_ptr(tlv_box_t *box, tlv_tag_t tag, uint8_t **value,
                             tlv_size_t *length) {
  value_t object;
  if (key_list_get(box->m_list, tag, &object) != 0) {
    return 0;
  }
  tlv_t *tlv = (tlv_t *)object.value;
  *value = tlv->value;
  *length = tlv->length;
  return 1;
}

int8_t tlv_box_get_object(tlv_box_t *box, tlv_tag_t tag, tlv_box_t **object) {
  value_t value;
  if (key_list_get(box->m_list, tag, &value) != 0) {
    return 0;
  }
  tlv_t *tlv = (tlv_t *)value.value;
  *object = (tlv_box_t *)tlv_box_parse(tlv->value, tlv->length);
  return 1;
}

tlv_size_t tlv_box_value_length(tlv_box_t *box, tlv_tag_t tag) {
  value_t object;
  if (key_list_get(box->m_list, tag, &object) != 0) {
    return 0;
  }
  tlv_t *tlv = (tlv_t *)object.value;
  return tlv->length;
}