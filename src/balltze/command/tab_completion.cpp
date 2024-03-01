// SPDX-License-Identifier: GPL-3.0-only

#include <cstdint>
#include <vector>
#include <map>
#include <balltze/engine/script.hpp>
#include <balltze/hook.hpp>
#include <balltze/plugin.hpp>
#include <balltze/command.hpp>
#include "../logger.hpp"

namespace Balltze {
    using HscFunctionEntry = Engine::HscFunctionEntry;

    extern std::vector<std::shared_ptr<Command>> commands;

    static HscFunctionEntry ***entries = nullptr;
    static std::uint32_t *entry_count;
    static HscFunctionEntry **old_entries;
    static std::uint32_t old_entry_count;

    static std::vector<HscFunctionEntry *> new_entries_list;
    static std::vector<std::unique_ptr<HscFunctionEntry>> new_entries_added;

    static void on_tab_completion_start() {
        old_entry_count = *entry_count;
        old_entries = *entries;

        new_entries_list = std::vector<HscFunctionEntry *>(old_entries, old_entries + old_entry_count);
        for(const auto &command : commands) {
            auto &new_command = new_entries_added.emplace_back(std::make_unique<HscFunctionEntry>());
            new_command->return_type = Engine::HSC_DATA_TYPE_VOID;
            new_command->name = command->full_name();
            new_command->help_message = command->help();
            new_command->help_parameters = command->params_help();
            *reinterpret_cast<std::uint16_t *>(new_command->gap_71) = 0x15;
            new_entries_list.emplace_back(new_command.get());
        }

        Memory::overwrite(entry_count, static_cast<std::uint32_t>(new_entries_list.size()));
        Memory::overwrite(entries, new_entries_list.data());
    }

    static void on_tab_completion_end() {
        Memory::overwrite(entry_count, old_entry_count);
        Memory::overwrite(entries, old_entries);

        new_entries_list.clear();
        new_entries_added.clear();
    }

    void set_up_commands_tab_completion() {
        auto *console_tab_completion_function_call_sig = Memory::get_signature("console_tab_completion_function_call");
        if(!console_tab_completion_function_call_sig) {
            throw std::runtime_error("Could not find signature for tab completion");
        }

        try {
            std::uint8_t instruction = *reinterpret_cast<std::uint8_t *>(console_tab_completion_function_call_sig->data());
            if(Memory::already_hooked(console_tab_completion_function_call_sig->data())) {
                // Workaround for Chimera hook
                std::byte *ptr = Memory::follow_32bit_jump(console_tab_completion_function_call_sig->data()) + 9;
                Memory::hook_function(ptr, on_tab_completion_start, on_tab_completion_end);
            }
            else {
                Memory::hook_function(console_tab_completion_function_call_sig->data(), on_tab_completion_start, on_tab_completion_end);
            }
        }
        catch(const std::runtime_error &e) {
            throw std::runtime_error("Could not hook tab completion function: " + std::string(e.what()));
        }

        auto *command_list_address_custom_edition_sig = Memory::get_signature("command_list_address_custom_edition");
        entries = reinterpret_cast<HscFunctionEntry ***>(command_list_address_custom_edition_sig->data());
        entry_count = reinterpret_cast<std::uint32_t *>(command_list_address_custom_edition_sig->data() + 5);
    }
}
