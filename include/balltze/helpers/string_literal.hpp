// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__HELPERS__STRING_LITERAL_HPP
#define BALLTZE_API__HELPERS__STRING_LITERAL_HPP

#include <algorithm>
#include <cstdint>

namespace Balltze {
    /**
     * A helper struct to store string literals as constexpr values.
     * https://ctrpeach.io/posts/cpp20-string-literal-template-parameters/
     */
    template<std::size_t N>
    struct StringLiteral {
        constexpr StringLiteral(const char (&str)[N]) {
            std::copy_n(str, N, value);
        }
        char value[N];
    };
}

#endif
