/*!
 * tlvpp.
 * data.h
 *
 * \date 01/28/2021
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */
#ifndef TLVPP_DATA_H
#define TLVPP_DATA_H

#include "byte_order.h"

#include <stdint.h>

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

#pragma pack(push, 2)
typedef struct _tlv {
    tlv_tag_t tag;
    tlv_size_t length;
    uint8_t* value;
} tlv_t;
#pragma pack(pop)

typedef void (*value_releaser)(tlv_t* value);

typedef struct key_list_node {
    tlv_tag_t key;
    tlv_t* value;
    struct key_list_node* prev;
    struct key_list_node* next;
} key_list_node_t;

typedef struct key_list {
    int count;
    key_list_node_t* header;
    key_list_node_t* tail;
    value_releaser releaser;
} tlv_key_list_t;

#pragma pack(push, 2)
typedef struct _tlv_box {
    tlv_key_list_t* m_list;
    uint8_t* m_serialized_buffer;
    tlv_size_t m_serialized_bytes;
} tlv_box_t;
#pragma pack(pop)

static void to_bytes_double_le(uint8_t* bytes, double value) {
    union {
        double a;
        unsigned char bytes[8];
    } thing;
    thing.a = value;
    for (size_t i = 0; i < sizeof(double); i++) {
        bytes[i] = thing.bytes[i];
    }
}

static double to_double_le(const uint8_t* bytes) {
    union {
        double a;
        unsigned char bytes[8];
    } thing;
    for (size_t i = 0; i < sizeof(double); i++) {
        thing.bytes[i] = bytes[i];
    }
    return thing.a;
}

static void to_bytes_float_le(uint8_t* bytes, float float_variable) {
    union {
        float a;
        unsigned char bytes[4];
    } thing;
    thing.a = float_variable;
    for (size_t i = 0; i < sizeof(float); i++) {
        bytes[i] = thing.bytes[i];
    }
}

static float to_float_le(const uint8_t* bytes) {
    union {
        float a;
        unsigned char bytes[4];
    } thing;
    for (size_t i = 0; i < sizeof(float); i++) {
        thing.bytes[i] = bytes[i];
    }
    return thing.a;
}

static uint64_t to_num_le(const uint8_t* data, size_t len) {
    if (len == 2) {
        return (data[1] << 8u) | (data[0]);
    } else if (len == 4) {
        return (data[3]) << 24u | (data[2]) << 16u | (data[1]) << 8u | (data[0]);
    } else if (sizeof(uint64_t) == len) {
        return (uint64_t)(data[7]) << 56u | (uint64_t)(data[6]) << 48u | (uint64_t)(data[5]) << 40u |
               (uint64_t)(data[4]) << 32u | (uint64_t)(data[3]) << 24u | (uint64_t)(data[2]) << 16u |
               (uint64_t)(data[1]) << 8u | (data[0]);
    } else {
        return data[0];
    }
}

static void to_bytes_le(uint8_t* data, uint64_t src, size_t len) {
    if (len == 1) {
        data[0] = src & 0xFFu;
    } else if (len == 2) {
        data[0] = (src) &0xFFu;
        data[1] = (src >> 8) & 0xFFu;
    } else if (len == 4) {
        data[0] = (src) &0xFF;
        data[1] = (src >> 8) & 0xFF;
        data[2] = (src >> 16) & 0xFF;
        data[3] = (src >> 24) & 0xFF;
    } else if (len == 8) {
        data[0] = (src) &0xFFu;
        data[1] = (src >> 8) & 0xFFu;
        data[2] = (src >> 16) & 0xFFu;
        data[3] = (src >> 24) & 0xFFu;
        data[4] = (src >> 32) & 0xFFu;
        data[5] = (src >> 40) & 0xFFu;
        data[6] = (src >> 48) & 0xFFu;
        data[7] = (src >> 56) & 0xFFu;
    }
}

#define key_compare(a, b) ((a == b) ? 1 : 0)

#endif // TLVPP_DATA_H
