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
#ifndef _TLV_BOX_H_
#define _TLV_BOX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "key_list.h"

#ifdef TLV_TAG_SIZE
typedef TLV_TAG_SIZE tlv_tag_t;
#else
typedef uint8_t tlv_tag_t;
#endif

#ifdef TLV_SIZE_SIZE
typedef TLV_SIZE_SIZE tlv_size_t;
#else
typedef uint16_t tlv_size_t;
#endif

typedef struct _tlv {
  tlv_tag_t tag;
  tlv_size_t length;
  uint8_t *value;
} tlv_t;

typedef struct _tlv_box {
  tlv_key_list_t *m_list;
  uint8_t *m_serialized_buffer;
  tlv_size_t m_serialized_bytes;
} tlv_box_t;

tlv_box_t *tlv_box_create();
tlv_box_t *tlv_box_parse(unsigned char *buffer, tlv_size_t buffersize);
void tlv_box_destroy(tlv_box_t *box);

unsigned char *tlv_box_get_buffer(tlv_box_t *box);
tlv_size_t tlv_box_get_size(tlv_box_t *box);

int8_t tlv_box_put_char(tlv_box_t *box, tlv_tag_t tag, int8_t value);
int8_t tlv_box_put_uchar(tlv_box_t *box, tlv_tag_t tag, uint8_t value);
int8_t tlv_box_put_short(tlv_box_t *box, tlv_tag_t tag, int16_t value);
int8_t tlv_box_put_ushort(tlv_box_t *box, tlv_tag_t tag, uint16_t value);
int8_t tlv_box_put_int(tlv_box_t *box, tlv_tag_t tag, int32_t value);
int8_t tlv_box_put_long(tlv_box_t *box, tlv_tag_t tag, int64_t value);
int8_t tlv_box_put_longlong(tlv_box_t *box, tlv_tag_t tag, long long value);
int8_t tlv_box_put_float(tlv_box_t *box, tlv_tag_t tag, float value);
int8_t tlv_box_put_double(tlv_box_t *box, tlv_tag_t tag, double value);
int8_t tlv_box_put_string(tlv_box_t *box, tlv_tag_t tag, const char *value);
int8_t tlv_box_put_bytes(tlv_box_t *box, tlv_tag_t tag, unsigned char *value,
                         tlv_size_t length);
int8_t tlv_box_put_object(tlv_box_t *box, tlv_tag_t tag, tlv_box_t *object);
int8_t tlv_box_serialize(tlv_box_t *box);

int8_t tlv_box_get_uchar(tlv_box_t *box, tlv_tag_t tag, uint8_t *value);
int8_t tlv_box_get_char(tlv_box_t *box, tlv_tag_t tag, char *value);
int8_t tlv_box_get_short(tlv_box_t *box, tlv_tag_t tag, int16_t *value);
int8_t tlv_box_get_ushort(tlv_box_t *box, tlv_tag_t tag, uint16_t *value);
int8_t tlv_box_get_int(tlv_box_t *box, tlv_tag_t tag, int32_t *value);
int8_t tlv_box_get_long(tlv_box_t *box, tlv_tag_t tag, int64_t *value);
int8_t tlv_box_get_longlong(tlv_box_t *box, tlv_tag_t tag, long long *value);
int8_t tlv_box_get_float(tlv_box_t *box, tlv_tag_t tag, float *value);
int8_t tlv_box_get_double(tlv_box_t *box, tlv_tag_t tag, double *value);
int8_t tlv_box_get_string(tlv_box_t *box, tlv_tag_t tag, char *value,
                          tlv_size_t *length);
int8_t tlv_box_get_bytes(tlv_box_t *box, tlv_tag_t tag, uint8_t *value,
                         tlv_size_t *length);
int8_t tlv_box_get_bytes_ptr(tlv_box_t *box, tlv_tag_t tag, uint8_t **value,
                             tlv_size_t *length);
int8_t tlv_box_get_object(tlv_box_t *box, tlv_tag_t tag, tlv_box_t **object);

tlv_size_t tlv_box_value_length(tlv_box_t *box, tlv_tag_t tag);

#ifdef __cplusplus
}
#endif

#endif //_TLV_BOX_H_
