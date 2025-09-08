// SPDX-License-Identifier: GPL-3.0-only

#include <cstdint>
#include <vector>
#include <map>
#include <balltze/legacy_api/engine/script.hpp>
#include <balltze/hook.hpp>
#include <balltze/legacy_api/plugin.hpp>
#include <balltze/command.hpp>
#include "../legacy_api/event/console_command.hpp"
#include "../logger.hpp"
#include "command.hpp"

namespace Balltze {
    using HscFunctionEntry = LegacyApi::Engine::HscFunctionEntry;

    static HscFunctionEntry ***entries_ptr_1 = nullptr;
    static HscFunctionEntry ***entries_ptr_2 = nullptr;
    static HscFunctionEntry ***entries_ptr_3 = nullptr;
    static std::uint16_t *entry_count;
    static HscFunctionEntry **old_entries;
    static std::uint16_t old_entry_count;

    static std::vector<HscFunctionEntry *> new_entries_list;
    static std::vector<std::unique_ptr<HscFunctionEntry>> new_entries_added;

    static void on_console_command_event(LegacyApi::Event::ConsoleCommandEvent &event) {
        if(event.time == LegacyApi::Event::EVENT_TIME_BEFORE) {
            old_entry_count = *entry_count;
            old_entries = *entries_ptr_1;

            new_entries_list = std::vector<HscFunctionEntry *>(old_entries, old_entries + old_entry_count);
            auto &commands = get_commands();
            for(const auto &command : commands) {
                auto &new_command = new_entries_added.emplace_back(std::make_unique<HscFunctionEntry>());
                new_command->return_type = LegacyApi::Engine::HSC_DATA_TYPE_VOID;
                new_command->name = command->full_name();
                new_command->help_message = command->help();
                new_command->help_parameters = command->params_help();
                *reinterpret_cast<std::uint16_t *>(new_command->gap_72) = 0x15;
                new_entries_list.emplace_back(new_command.get());
            }

            Memory::overwrite(entry_count, static_cast<std::uint16_t>(new_entries_list.size()));
            Memory::overwrite(entries_ptr_1, new_entries_list.data());
            Memory::overwrite(entries_ptr_2, new_entries_list.data());
            Memory::overwrite(entries_ptr_3, new_entries_list.data());
        }
        else {
            Memory::overwrite(entry_count, old_entry_count);
            Memory::overwrite(entries_ptr_1, old_entries);
            Memory::overwrite(entries_ptr_2, old_entries);
            Memory::overwrite(entries_ptr_3, old_entries);

            new_entries_list.clear();
            new_entries_added.clear();
        }
    }

    void set_up_commands_help() {
        auto *command_list_address = Memory::get_signature("find_console_command_entry_function_command_list_address");
        auto *command_list_count = Memory::get_signature("find_console_command_entry_function_command_list_count");
        auto *command_list_address_2 = Memory::get_signature("help_command_function_command_list_address_1");
        auto *command_list_address_3 = Memory::get_signature("help_command_function_command_list_address_2");

        if(!command_list_address || !command_list_count || !command_list_address_2 || !command_list_address_3) {
            logger.warning("failed to set up commands help: could not find signatures");
            return;
        }

        entries_ptr_1 = reinterpret_cast<HscFunctionEntry ***>(command_list_address->data());
        entries_ptr_2 = reinterpret_cast<HscFunctionEntry ***>(command_list_address_2->data());
        entries_ptr_3 = reinterpret_cast<HscFunctionEntry ***>(command_list_address_3->data());
        entry_count = reinterpret_cast<std::uint16_t *>(command_list_count->data());

        LegacyApi::Event::ConsoleCommandEvent::subscribe(on_console_command_event);
    }
}
