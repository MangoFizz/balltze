// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__HELPERS__STRING_LITERAL_HPP
#define BALLTZE_API__HELPERS__STRING_LITERAL_HPP

#include <algorithm>
#include <cstdint>

/**
 * A helper struct to store string literals as constexpr values.
 * https://ctrpeach.io/posts/cpp20-string-literal-template-parameters/
 */
template<std::size_t N>
struct StringLiteral {
    consteval StringLiteral(const char (&str)[N]) {
        std::copy_n(str, N, data);
    }
    consteval StringLiteral() = default;
    char data[N];
    std::size_t size = N;
};

consteval char char_to_upper(char c) {
    return (c >= 'a' && c <= 'z') ? c - ('a' - 'A') : c;
}

consteval char char_to_lower(char c) {
    return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
}

template <size_t N>
consteval auto snake_to_camel_case(const char (&str)[N]) {
    StringLiteral<N> result = {};
    std::size_t char_count = 0;
    bool capitalize_next = false;
    for(std::size_t i = 0; str[i] != '\0'; ++i) {
        if(i > 0 && str[i] == '_') {
            capitalize_next = true;
        } 
        else {
            result.data[char_count] = capitalize_next ? char_to_upper(str[i]) : char_to_lower(str[i]);
            char_count++;
            capitalize_next = false;
        }
    }
    result.data[char_count] = '\0';
    return result;
}

#endif
