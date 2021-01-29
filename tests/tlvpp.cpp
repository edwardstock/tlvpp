/*!
 * tlvpp.
 * tlvpp.cpp.c
 *
 * \date 01/27/2021
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include <gtest/gtest.h>
#include <tlvpp.h>

std::string hexStr(const uint8_t* data, size_t len) {
    std::stringstream ss;
    ss << std::hex;

    for (int i(0); i < len; ++i)
        ss << std::setw(2) << std::setfill('0') << (int) data[i];

    return ss.str();
}

void strToHex(uint8_t* out, const char* src) {
    const auto l = strlen(src) / 2;
    uint8_t bytes_buffer[l];
    for (size_t i = 0, c = 0; i < l; i += 2, c++) {
        char v[2];
        v[0] = src[i];
        v[1] = src[i + 1];
        bytes_buffer[c] = (uint8_t) strtol(v, nullptr, 16);
    }

    memcpy(out, bytes_buffer, l);
}

void hexToString(char* out, const uint8_t* src, size_t size) {
    char hex_buffer[size * 2 + 1];
    for (int i = 0, c = 0; i < size; i++, c += 2) {
        sprintf(hex_buffer + c, "%02x", src[i]);
    }
    hex_buffer[size * 2 + 1] = '\0';
    memcpy(out, hex_buffer, size * 2 + 1);
}

uint8_t* datahex(const char* string) {

    if (string == NULL)
        return NULL;

    size_t slength = strlen(string);
    if ((slength % 2) != 0) // must be even
        return NULL;

    size_t dlength = slength / 2;

    uint8_t* data = (uint8_t*) malloc(dlength);
    memset(data, 0, dlength);

    size_t index = 0;
    while (index < slength) {
        char c = string[index];
        int value = 0;
        if (c >= '0' && c <= '9')
            value = (c - '0');
        else if (c >= 'A' && c <= 'F')
            value = (10 + (c - 'A'));
        else if (c >= 'a' && c <= 'f')
            value = (10 + (c - 'a'));
        else {
            free(data);
            return NULL;
        }

        data[(index / 2)] += value << (((index + 1) % 2) * 4);

        index++;
    }

    return data;
}

bool cmp_bytes(const uint8_t* a, const uint8_t* b, size_t len) {
    for (size_t i = 0; i < len; i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }

    return true;
}

TEST(TLVP, conversion) {
    tlvpp::TLV tlv;

    tlv.put(0x00, (uint8_t) 10);
}

TEST(TLVPP, all) {

    tlvpp::TLV tlv;

    tlv.put(0x00, (uint8_t) 10);
    tlv.put(0x01, (char) 8);
    tlv.put(0x02, (uint16_t) 100);
    tlv.put(0x03, (int16_t) -10);
    tlv.put(0x04, (uint32_t) 1365);
    tlv.put(0x05, (int32_t) -20454);
    tlv.put(0x06, (uint64_t) 5000);
    tlv.put(0x07, (int64_t) -5000);
    tlv.put(0x08, (float) 3.141f);
    tlv.put(0x09, (double) 3.141);

    // put byte array
    const uint8_t bytes[4] = {0xFF, 0x00, 0xFF, 0x7C};
    tlv.put(0x0A, bytes, 4);

    // put nested tlv
    tlvpp::TLV sub;
    sub.put(0x00, 100);
    sub.serialize();
    tlv.put(0x0B, sub);

    // put string
    tlv.put(0x0C, "hello TLV, it's me");

    const size_t encoded_size = tlv.serialize();
    const uint8_t* payload = tlv.get_serialized();

    ////////////////////////////////////

    const char* from_device = "0001000a010100080202006400030200f6ff040400550500000504001ab0ffff060800881300000000000007"
                              "080078ecffffffffffff0804002506494009080054e3a59bc42009400a0400ff00ff7c0b0700000400640000"
                              "000c120068656c6c6f20544c562c2069742773206d65";
    std::cout << "Encode result: \n" << hexStr(payload, encoded_size) << "\n" << from_device << std::endl;

    ASSERT_STREQ(from_device, hexStr(payload, encoded_size).c_str());
    //  uint8_t* from_device_buf2 = datahex(from_device);
    //  tlvpp::TLV from_dev_d(from_device_buf2, 110);

    //  from_dev_d.foreach([](tlv_tag_t tag, tlv_size_t length, const uint8_t* data) {
    //    const tlv_size_t sz = length;
    //    uint8_t cmp[sz];
    //    tlv.get_buffer(tag, cmp, &length);
    //
    //    if(!cmp_bytes(cmp, data, sz)) {
    //      std::cout << "Tag " << ((int)tag) << " are not equal" << std::endl;
    //    }
    //  });

    //  ASSERT_STREQ(from_device, hex_buffer);

    tlvpp::TLV dec(payload, encoded_size);

    ASSERT_EQ((uint8_t) 10, tlv.get_byte(0x00));
    ASSERT_EQ((char) 8, tlv.get_char(0x01));
    ASSERT_EQ((uint16_t) 100, tlv.get_ushort(0x02));
    ASSERT_EQ((int16_t) -10, tlv.get_short(0x03));
    ASSERT_EQ((uint32_t) 1365, tlv.get_uint(0x04));
    ASSERT_EQ((uint32_t) -20454, tlv.get_int(0x05));
    ASSERT_EQ((uint64_t) 5000, tlv.get_uint64(0x06));
    ASSERT_EQ((int64_t) -5000, tlv.get_int64(0x07));
    ASSERT_EQ((float) 3.141, tlv.get_float(0x08));
    ASSERT_EQ((double) 3.141, tlv.get_double(0x09));

    uint8_t dec_bytes[4];
    tlv_size_t dec_bytes_len = 4;
    tlv.get_buffer(0x0A, dec_bytes, &dec_bytes_len);

    ASSERT_EQ((uint8_t) 0xFF, dec_bytes[0]);
    ASSERT_EQ((uint8_t) 0x00, dec_bytes[1]);
    ASSERT_EQ((uint8_t) 0xFF, dec_bytes[2]);
    ASSERT_EQ((uint8_t) 0x7C, dec_bytes[3]);

    const tlvpp::TLV dec_sub = dec.get_object(0x0B);
    ASSERT_EQ((uint8_t) 100, dec_sub.get_byte(0x00));

    const char* expected_str = "hello TLV, it's me";
    tlv_size_t dec_str_len = 100;
    char dec_str[100];
    dec.get_string(0x0C, dec_str, &dec_str_len);
    ASSERT_STREQ(expected_str, dec_str);
    ASSERT_EQ(18, strlen(dec_str));
    ASSERT_EQ(19, dec_str_len);
}

TEST(TLVPP, foreach) {
    tlvpp::TLV tlv;

    tlv.put(0x00, (uint8_t) 10);
    tlv.put(0x01, (char) 8);
    tlv.put(0x02, (uint16_t) 100);
    tlv.put(0x03, (int16_t) -10);
    tlv.put(0x04, (uint32_t) 1365);
    tlv.put(0x05, (int32_t) -20454);
    tlv.put(0x06, (uint64_t) UINT64_MAX);
    tlv.put(0x07, (int64_t) INT64_MIN);
    tlv.put(0x08, (float) 3.141);
    tlv.put(0x09, (double) 3.141);

    tlv.foreach ([](tlv_tag_t tag, tlv_size_t data_len, const uint8_t* data) {
        if (tag == 0x00) {
            ASSERT_EQ(1, data_len);
            ASSERT_EQ((uint8_t) 10, data[0]);
        }
    });
}

TEST(TLVPP, decode1) {
    // 00 0100 01

    float v1 = 3.141f;
    uint8_t d1[4];
    to_bytes_float_le(d1, v1);
    float r1 = to_float_le(d1);

    std::cout << hexStr(d1, 4) << std::endl;
    printf("%.08f\n", r1);
}