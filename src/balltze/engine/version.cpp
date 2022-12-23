// SPDX-License-Identifier: GPL-3.0-only

#include <optional>
#include <stdexcept>
#include <cstring>
#include <balltze/engine/version.hpp>
#include <balltze/memory.hpp>

namespace Balltze::Engine {
    EngineEdition get_engine_edition() {
        static std::optional<EngineEdition> engine_type;
        if(!engine_type.has_value()) {
            auto *engine_edition_sig = Memory::get_signature("engine_type");
            if(!engine_edition_sig) {
                throw std::runtime_error("engine_edition signature not found");
            }
            std::string engine_edition_str = *reinterpret_cast<const char **>(engine_edition_sig->data());
            if(engine_edition_str == "halom") {
                engine_type = ENGINE_TYPE_CUSTOM_EDITION;
            }
            else if(engine_edition_str == "halor") {
                engine_type = ENGINE_TYPE_RETAIL;
            }
            else if(engine_edition_str == "halod") {
                engine_type = ENGINE_TYPE_DEMO;
            }
            else {
                throw std::runtime_error("Unknown engine edition " + engine_edition_str);
            }
        }
        return *engine_type;
    }
}
