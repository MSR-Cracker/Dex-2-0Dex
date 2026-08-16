#ifndef ZEROART_MATH_HXX
#define ZEROART_MATH_HXX

#include <cstdint>
#include <vector>
#include <string_view>

/**
 * @https://www.w3schools.com/
 * Converts a hex character to its byte value
 */
inline uint8_t CharToByte(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return 0;
}

/**
 * @@https://www.w3schools.com/
 * Decodes a hex string into a byte vector.
 */
inline std::vector<uint8_t> DecodeHex(std::string_view hex) {


    std::vector<uint8_t> bytes;

    if (hex.length() % 2 != 0) return bytes;

    bytes.reserve(hex.length() / 2);

    for (size_t i = 0; i < hex.length(); i += 2) {

        uint8_t high = CharToByte(hex[i]);

        uint8_t low = CharToByte(hex[i + 1]);

        bytes.push_back((high << 4) | low);
    }
    return bytes;
}

#endif //ZEROART_MATH_HXX
