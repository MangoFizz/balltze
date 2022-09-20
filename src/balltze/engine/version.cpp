// SPDX-License-Identifier: GPL-3.0-only

#include <optional>
#include <stdexcept>
#include <cstring>
#include <balltze/balltze.hpp>
#include <balltze/engine/version.hpp>

namespace Balltze::Engine {
    EngineType get_engine_type() noexcept {
        static std::optional<EngineType> engine_type;

        if(!engine_type.has_value()) {
            auto &balltze = Balltze::get();
            auto &sig_manager = balltze.signature_manager();
            auto *engine_type_sig = sig_manager.get("engine_type");
        
            if(!engine_type_sig) {
                throw std::runtime_error("engine_type signature not found");
            }

            std::string engine_type_str = *reinterpret_cast<const char **>(engine_type_sig->data());

            if(engine_type_str == "halom") {
                engine_type = EngineType::ENGINE_TYPE_CUSTOM_EDITION;
            }
            else if(engine_type_str == "halor") {
                engine_type = EngineType::ENGINE_TYPE_RETAIL;
            }
            else if(engine_type_str == "halod") {
                engine_type = EngineType::ENGINE_TYPE_DEMO;
            }
        }

        return *engine_type;
    }
}
