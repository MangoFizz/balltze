// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__MEMORY__MEMORY_HPP
#define BALLTZE__MEMORY__MEMORY_HPP

namespace Balltze::Memory {
    enum EngineType {
        ENGINE_TYPE_CUSTOM_EDITION_CLIENT,
        ENGINE_TYPE_CUSTOM_EDITION_DEDICATED_SERVER,
    };

    EngineType find_signatures();
}

#endif
