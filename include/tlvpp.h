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
    typedef void (*for_each_fn)(tlv_tag_t, tlv_size_t, const uint8_t*);
    typedef void (*for_each_idx_fn)(tlv_size_t, tlv_tag_t, tlv_size_t, const uint8_t*);

    TLV();
    // this constructor will move data to internal variable and destroy original
    // (like std::move)
    explicit TLV(tlv_box_t* other);
    TLV(const uint8_t* data, tlv_size_t data_length);
    TLV(tlvpp::TLV&& other) noexcept;
    TLV(const tlvpp::TLV& other);
    TLV& operator=(const TLV& other);
    TLV& operator=(TLV&& other) noexcept;

    ~TLV();

    tlv_size_t serialize();
    const uint8_t* get_serialized() const;

    void put(tlv_tag_t tag, uint8_t value);
    void put(tlv_tag_t tag, int8_t value);
    void put(tlv_tag_t tag, char value);
    void put(tlv_tag_t tag, uint16_t value);
    void put(tlv_tag_t tag, int16_t value);
    void put(tlv_tag_t tag, uint32_t value);
    void put(tlv_tag_t tag, int32_t value);
    void put(tlv_tag_t tag, uint64_t value);
    void put(tlv_tag_t tag, int64_t value);
    void put(tlv_tag_t tag, float value);
    void put(tlv_tag_t tag, double value);
    void put(tlv_tag_t tag, const uint8_t* data, tlv_size_t data_length);
    void put(tlv_tag_t tag, const TLV& data);
    void put(tlv_tag_t tag, const char* value);
#ifdef ARDUINO
    void put(tlv_tag_t tag, const String& value);
#endif

    uint8_t get_byte(tlv_tag_t tag) const;
    int8_t get_char(tlv_tag_t tag) const;
    uint16_t get_ushort(tlv_tag_t tag) const;
    int16_t get_short(tlv_tag_t tag) const;
    uint32_t get_uint(tlv_tag_t tag) const;
    int32_t get_int(tlv_tag_t tag) const;
    uint64_t get_uint64(tlv_tag_t tag) const;
    int64_t get_int64(tlv_tag_t tag) const;
    float get_float(tlv_tag_t tag) const;
    double get_double(tlv_tag_t tag) const;
    void get_buffer(tlv_tag_t tag, uint8_t* data, tlv_size_t* data_length) const;
    TLV get_object(tlv_tag_t tag) const;
    void get_string(tlv_tag_t tag, char* out, tlv_size_t* len) const;
    tlv_size_t get_value_length(tlv_tag_t tag) const;

    /// \brief Iterate over all data
    /// \param fn callback arguments: fun(tlv_tag_t tag, tlv_size_t data_len, const uint8_t* data)
    void foreach (for_each_fn fn);

    /// \brief Iterate over all data with index
    /// \param fn callback arguments: fun(tlv_size_t index, tlv_tag_t tag, tlv_size_t data_len, const uint8_t* data)
    void foreach_indexed(for_each_idx_fn fn);

private:
    tlv_box_t* box = nullptr;
};
} // namespace tlvpp

#endif // TLVPP_HPP
