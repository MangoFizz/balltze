// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__ENGINE__VERSION_HPP
#define BALLTZE__ENGINE__VERSION_HPP

namespace Balltze::Engine {
    /** Engine used */
    enum EngineType {
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
    EngineType get_engine_type();
}

#endif
