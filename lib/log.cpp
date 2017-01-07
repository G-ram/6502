#include <sstream>

#include "log.h"

std::string hexify(const std::string& input) {
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();

    std::string output;
    output.reserve(3 * len);
    for (size_t i = 0; i < len; ++i) {
        if(i % 16 == 0) {
            output.push_back('\n');
        }
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
        output.push_back(' ');
    }
    return output;
}

std::string hexify(const unsigned char& input) {
    static const char* const lut = "0123456789ABCDEF";
    std::string output;
    output.reserve(1);
    output.push_back(lut[input >> 4]);
    output.push_back(lut[input & 15]);
    return output;
}

std::string hexify(const unsigned short& input) {
    std::stringstream stream;
    stream << std::hex << input;
    return stream.str();
}

std::string binify(const unsigned char& input) {
    std::stringstream stream;
    std::bitset<8> x(input);
    stream << std::hex << x;
    return stream.str();
}

std::string binify(const unsigned short& input) {
    std::stringstream stream;
    std::bitset<16> x(input);
    stream << std::hex << x;
    return stream.str();
}
