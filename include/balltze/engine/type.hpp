// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__ENGINE__TYPE_HPP
#define BALLTZE__ENGINE__TYPE_HPP

#include <cstdint> 

namespace Balltze::Engine {
    union ElementID {
        std::uint32_t whole_id;
        struct {
            std::uint16_t index;
            std::uint16_t id;
        } index;

        ElementID(std::uint32_t id) {
            this->whole_id = id;
        }

        ElementID() = default;

        static ElementID null_id() noexcept {
            return { 0xFFFFFFFF };
        }

        bool is_null() const noexcept {
            return *this == null_id();
        }

        bool operator==(const ElementID &other) const noexcept {
            return this->whole_id == other.whole_id;
        }

        bool operator!=(const ElementID &other) const noexcept {
            return this->whole_id != other.whole_id;
        }
    };

    using PlayerID = ElementID;
}

#endif
