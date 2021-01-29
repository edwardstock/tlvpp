/*!
 * trash.
 * tlvpp.cpp.c
 *
 * \date 01/27/2021
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "../include/tlvpp.h"

#include <string.h>
tlvpp::TLV::TLV() : box(tlv_box_create()) {
}
tlvpp::TLV::TLV(tlv_box_t* other) : box(tlv_box_create()) {
    memcpy(box, other, sizeof(tlv_box_t));
    tlv_box_destroy(other);
}
tlvpp::TLV::TLV(const uint8_t* data, tlv_size_t data_length) : box(tlv_box_parse(data, data_length)) {
}

tlvpp::TLV::TLV(tlvpp::TLV&& other) noexcept : box(tlv_box_create()) {
    memcpy(box, other.box, sizeof(tlv_box_t));
    tlv_box_destroy(other.box);
    other.box = nullptr;
}
tlvpp::TLV::TLV(const tlvpp::TLV& other) : box(tlv_box_create()) {
    memcpy(box, other.box, sizeof(tlv_box_t));
}

tlvpp::TLV& tlvpp::TLV::operator=(const tlvpp::TLV& other) {
    if (&other == this) {
        return *this;
    }
    box = tlv_box_create();
    memcpy(box, other.box, sizeof(tlv_box_t));
    return *this;
}
tlvpp::TLV& tlvpp::TLV::operator=(tlvpp::TLV&& other) noexcept {
    if (&other == this) {
        return *this;
    }
    box = tlv_box_create();
    memcpy(box, other.box, sizeof(tlv_box_t));
    tlv_box_destroy(other.box);
    other.box = nullptr;
    return *this;
}

tlvpp::TLV::~TLV() {
    tlv_box_destroy(box);
}

tlv_size_t tlvpp::TLV::serialize() {
    tlv_box_serialize(box);
    return tlv_box_get_size(box);
}

const uint8_t* tlvpp::TLV::get_serialized() const {
    return tlv_box_get_buffer(box);
}

void tlvpp::TLV::put(tlv_tag_t tag, uint8_t value) {
    tlv_box_put_uchar(box, tag, value);
}
void tlvpp::TLV::put(tlv_tag_t tag, int8_t value) {
    tlv_box_put_char(box, tag, value);
}
void tlvpp::TLV::put(tlv_tag_t tag, char value) {
    tlv_box_put_char(box, tag, value);
}

void tlvpp::TLV::put(tlv_tag_t tag, uint16_t value) {
    tlv_box_put_ushort(box, tag, value);
}
void tlvpp::TLV::put(tlv_tag_t tag, int16_t value) {
    tlv_box_put_short(box, tag, value);
}
void tlvpp::TLV::put(tlv_tag_t tag, uint32_t value) {
    tlv_box_put_int(box, tag, value);
}
void tlvpp::TLV::put(tlv_tag_t tag, int32_t value) {
    tlv_box_put_int(box, tag, value);
}
void tlvpp::TLV::put(tlv_tag_t tag, uint64_t value) {
    tlv_box_put_uint64(box, tag, value);
}
void tlvpp::TLV::put(tlv_tag_t tag, int64_t value) {
    tlv_box_put_int64(box, tag, value);
}

void tlvpp::TLV::put(tlv_tag_t tag, float value) {
    tlv_box_put_float(box, tag, value);
}
void tlvpp::TLV::put(tlv_tag_t tag, double value) {
    tlv_box_put_double(box, tag, value);
}
void tlvpp::TLV::put(tlv_tag_t tag, const uint8_t* data, tlv_size_t data_length) {
    tlv_box_put_bytes(box, tag, data, data_length);
}
void tlvpp::TLV::put(tlv_tag_t tag, const tlvpp::TLV& data) {

    tlv_box_put_object(box, tag, data.box);
}
void tlvpp::TLV::put(tlv_tag_t tag, const char* value) {
    tlv_box_put_string(box, tag, value);
}

#ifdef ARDUINO
void tlvpp::TLV::put(tlv_tag_t tag, const String& value) {
    tlv_box_put_string(box, tag, value.c_str());
}
#endif

uint8_t tlvpp::TLV::get_byte(tlv_tag_t tag) const {
    uint8_t out;
    tlv_box_get_uchar(box, tag, &out);
    return out;
}
int8_t tlvpp::TLV::get_char(tlv_tag_t tag) const {
    char out;
    tlv_box_get_char(box, tag, &out);
    return out;
}
uint16_t tlvpp::TLV::get_ushort(tlv_tag_t tag) const {
    uint16_t out;
    tlv_box_get_ushort(box, tag, &out);
    return out;
}
int16_t tlvpp::TLV::get_short(tlv_tag_t tag) const {
    int16_t out;
    tlv_box_get_short(box, tag, &out);
    return out;
}
uint32_t tlvpp::TLV::get_uint(tlv_tag_t tag) const {
    int32_t out;
    tlv_box_get_int(box, tag, &out);
    return (uint32_t) out;
}
int32_t tlvpp::TLV::get_int(tlv_tag_t tag) const {
    int32_t out;
    tlv_box_get_int(box, tag, &out);
    return out;
}
uint64_t tlvpp::TLV::get_uint64(tlv_tag_t tag) const {
    int64_t out;
    tlv_box_get_int64(box, tag, &out);
    return (uint64_t) out;
}
int64_t tlvpp::TLV::get_int64(tlv_tag_t tag) const {
    int64_t out;
    tlv_box_get_int64(box, tag, &out);
    return out;
}
float tlvpp::TLV::get_float(tlv_tag_t tag) const {
    float out;
    tlv_box_get_float(box, tag, &out);
    return out;
}
double tlvpp::TLV::get_double(tlv_tag_t tag) const {
    double out;
    tlv_box_get_double(box, tag, &out);
    return out;
}
void tlvpp::TLV::get_buffer(tlv_tag_t tag, uint8_t* data, tlv_size_t* data_length) const {
    tlv_box_get_bytes(box, tag, data, data_length);
}
tlvpp::TLV tlvpp::TLV::get_object(tlv_tag_t tag) const {
    tlvpp::TLV out;
    tlv_box_get_object(box, tag, &out.box);

    return out;
}
void tlvpp::TLV::get_string(tlv_tag_t tag, char* out, tlv_size_t* len) const {
    tlv_box_get_string(box, tag, out, len);
}

tlv_size_t tlvpp::TLV::get_value_length(tlv_tag_t tag) const {
    return tlv_box_value_length(box, tag);
}

void tlvpp::TLV::foreach (tlvpp::TLV::for_each_fn fn) {
    key_list_foreach(box->m_list, node) {
        const tlv_t* tlv = node->value;
        fn(tlv->tag, tlv->length, tlv->value);
    }
}

void tlvpp::TLV::foreach_indexed(tlvpp::TLV::for_each_idx_fn fn) {
    tlv_size_t i = 0;
    key_list_foreach(box->m_list, node) {
        const tlv_t* tlv = node->value;
        fn(i, tlv->tag, tlv->length, tlv->value);
    }
}
