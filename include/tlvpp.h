/*!
 * tlv.
 * tlvpp.hpp.h
 *
 * \date 01/27/2021
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */
#ifndef TLVPP_HPP
#define TLVPP_HPP

#include "tlv_box.h"
#include <string.h>

#ifdef ARDUINO
#include <Arduino.h>
#endif

namespace tlvpp {

class TLV {
public:
  TLV();
  // this constructor will move data to internal variable and destroy original
  // (like std::move)
  TLV(tlv_box_t *other);
  TLV(uint8_t *data, tlv_size_t data_length);
  ~TLV();

  tlv_size_t serialize();
  uint8_t *get_buffer() const;

  void put(tlv_tag_t tag, uint8_t value);
  void put(tlv_tag_t tag, int8_t value);
  void put(tlv_tag_t tag, uint16_t value);
  void put(tlv_tag_t tag, int16_t value);
  void put(tlv_tag_t tag, uint32_t value);
  void put(tlv_tag_t tag, int32_t value);
  void put(tlv_tag_t tag, uint64_t value);
  void put(tlv_tag_t tag, int64_t value);
  void put(tlv_tag_t tag, float value);
  void put(tlv_tag_t tag, double value);
  void put(tlv_tag_t tag, uint8_t *data, tlv_size_t data_length);
  void put(tlv_tag_t tag, const TLV &data);
  void put(tlv_tag_t tag, const char *value);
#ifdef ARDUINO
  void put(tlv_tag_t tag, const String &value);
#endif

  uint8_t get_byte(tlv_tag_t tag) const;
  int8_t get_char(tlv_tag_t tag) const;
  uint16_t get_ushort(tlv_tag_t tag) const;
  int16_t get_short(tlv_tag_t tag) const;
  uint32_t get_uint(tlv_tag_t tag) const;
  int32_t get_int(tlv_tag_t tag) const;
  uint64_t get_ulong(tlv_tag_t tag) const;
  int64_t get_long(tlv_tag_t tag) const;
  float get_float(tlv_tag_t tag) const;
  double get_double(tlv_tag_t tag) const;
  void get_buffer(tlv_tag_t tag, uint8_t *data, tlv_size_t *data_length) const;
  TLV get_object(tlv_tag_t tag) const;
  void get_string(tlv_tag_t tag, char *out, tlv_size_t *len) const;
  tlv_size_t get_value_length(tlv_tag_t tag) const;

private:
  tlv_box_t *box;
};
} // namespace tlvpp

#endif // TLVPP_HPP
