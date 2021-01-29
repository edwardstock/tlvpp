/*!
 * tlvpp.
 * ext_test.cpp.c
 *
 * \date 2021
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#include "tlvpp.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdint.h>
#include <string.h>
#include <string>

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

std::string hexStr(const uint8_t* data, size_t len) {
    std::stringstream ss;
    ss << std::hex;

    for (int i(0); i < len; ++i)
        ss << std::setw(2) << std::setfill('0') << (int) data[i];

    return ss.str();
}

int main() {

    tlvpp::TLV tlv;

    tlv.put(0x00, (uint8_t) 10);
    tlv.put(0x01, (char) 8);
    tlv.put(0x02, (uint16_t) 100);
    tlv.put(0x03, (int16_t) -10);
    tlv.put(0x04, (uint32_t) 1365);
    tlv.put(0x05, (int32_t) -20454);
    tlv.put(0x06, (uint64_t) 5000);
    tlv.put(0x07, (int64_t) -5000);
    tlv.put(0x08, (float) 3.141);
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

    std::cout << hexStr(payload, encoded_size) << std::endl;
}
