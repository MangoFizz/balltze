// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__HELPERS__STRING_HPP
#define BALLTZE_API__HELPERS__STRING_HPP

#include <string>
#include <stdexcept>
#include <windows.h>

inline std::string string_w1252_to_utf8(const char *input) {
    int input_len = MultiByteToWideChar(1252, 0, input, -1, NULL, 0);
    if(input_len == 0) {
        return "";
    }

    wchar_t buffer[input_len];
    MultiByteToWideChar(1252, 0, input, -1, buffer, input_len);

    int output_len = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, NULL, 0, NULL, NULL);
    if(output_len == 0) {
        throw std::runtime_error("failed to convert string to UTF-8");
    }

    char output[output_len];
    WideCharToMultiByte(CP_UTF8, 0, buffer, -1, output, output_len, NULL, NULL);

    return output;
}

#endif
