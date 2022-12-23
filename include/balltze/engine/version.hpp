// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__ENGINE__VERSION_HPP
#define BALLTZE_API__ENGINE__VERSION_HPP

#include "../api.hpp"

namespace Balltze::Engine {
    /** Engine used */
    enum EngineEdition {
        /** Custom Edition */
        ENGINE_TYPE_CUSTOM_EDITION,

        /** Retail */
        ENGINE_TYPE_RETAIL,

        /** Demo */
        ENGINE_TYPE_DEMO
    };

    /**
     * Get the game engine used
     * @return game engine used
     */
    BALLTZE_API EngineEdition get_engine_edition();
}

#endif
