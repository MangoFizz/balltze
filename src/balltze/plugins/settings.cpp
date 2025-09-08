// SPDX-License-Identifier: GPL-3.0-only

#include <windows.h>
#include <impl/debug/stacktrace.h>
#include <balltze/memory.hpp>
#include "../config/config.hpp"
#include "plugin.hpp"
#include "loader.hpp"

namespace Balltze::Plugins {
    void load_plugin_commands_settings(Plugin *plugin) {
        CONTEXT context;
        RtlCaptureContext(&context);
        HMODULE caller_module_handle = Memory::get_module_from_address(stacktrace_get_caller_address(&context));
        auto directory = plugin->directory();
        auto config = Config::Config(directory / "settings.json");
        for(auto &command : commands) {
            auto command_key = std::string("commands.") + command->m_name;
            if(command->m_plugin && command->m_plugin == reinterpret_cast<void *>(plugin)) {
                if(config.exists(command_key)) {
                    auto command_value = config.get<std::string>(command_key);
                    auto arguments = split_arguments(command_value.value());
                    
                    auto arguments_alloc(std::make_unique<const char *[]>(arguments.size()));
                    for(std::size_t i = 0; i < arguments.size(); i++) {
                        arguments_alloc[i] = arguments[i].data();
                    }

                    bool res = command->call(arguments.size(), arguments_alloc.get());
                    if(res == COMMAND_RESULT_FAILED_ERROR) {
                        logger.error("Command {} failed to load from config", command->m_name);
                    }
                }
            }
        }
    }
}
