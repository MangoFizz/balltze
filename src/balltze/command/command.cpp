// SPDX-License-Identifier: GPL-3.0-only

#include <memory>
#include <vector>
#include <cstring>
#include <balltze/command.hpp>
#include <balltze/memory.hpp>
#include <balltze/hook.hpp>
#include <balltze/engine/core.hpp>
#include "../config/config.hpp"
#include "../event/console_command.hpp"
#include "../plugins/loader.hpp"
#include "../version.hpp"
#include "../logger.hpp"
#include "command.hpp"

namespace Balltze {
    std::vector<std::shared_ptr<Command>> commands;

    CommandResult Command::call(std::size_t arg_count, const char **args) const noexcept {
        if(m_function == nullptr) {
            logger.debug("Command {} function has a null pointer", m_name);
            return COMMAND_RESULT_FAILED_ERROR;
        }
        if(arg_count > this->max_args()) {
            return COMMAND_RESULT_FAILED_TOO_MANY_ARGUMENTS;
        }
        else if(arg_count < this->min_args()) {
            return COMMAND_RESULT_FAILED_NOT_ENOUGH_ARGUMENTS;
        }
        else {
            return m_function(arg_count, args) ? COMMAND_RESULT_SUCCESS : COMMAND_RESULT_FAILED_ERROR;
        }
    }

    CommandResult Command::call(const std::vector<std::string> &arguments) const noexcept {
        // Get argument count
        std::size_t arg_count = arguments.size();

        // If no arguments were passed, just call it.
        if(arg_count == 0) {
            return this->call(0, nullptr);
        }

        // Make our array
        auto arguments_alloc(std::make_unique<const char *[]>(arg_count));
        for(std::size_t i = 0; i < arg_count; i++) {
            arguments_alloc[i] = arguments[i].data();
        }

        // Do it!
        return this->call(arg_count, arguments_alloc.get());
    }

    static auto to_lower(std::string str) {
        std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
            return std::tolower(c);
        });
        return str;
    }

    Command::Command(std::string name, std::string category, std::string help, std::optional<std::string> params_help, CommandFunction function, bool autosave, std::size_t min_args, std::size_t max_args, bool can_call_from_console, bool is_public) : 
        m_name(name), m_category(category), m_help(help), m_params_help(params_help), m_function(function), m_autosave(autosave), m_min_args(min_args), m_max_args(max_args), m_can_call_from_console(can_call_from_console), m_public(is_public) {
            m_name = to_lower(m_name);
            m_plugin = nullptr;
            if(m_min_args > m_max_args) {
                throw std::runtime_error("Command " + m_name + " has a minimum argument count greater than the maximum argument count");
            }
            if(function == nullptr) {
                throw std::runtime_error("Command " + m_name + " has a null function");
            }
        }

    Command::Command(std::string name, std::string category, std::string help, std::optional<std::string> params_help, bool autosave, std::size_t min_args, std::size_t max_args, bool can_call_from_console, bool is_public) : 
        m_name(name), m_category(category), m_help(help), m_params_help(params_help), m_function(nullptr), m_autosave(autosave), m_min_args(min_args), m_max_args(max_args), m_can_call_from_console(can_call_from_console), m_public(is_public) {}

    Command::Command(std::string name, std::string category, std::string help, std::optional<std::string> params_help, CommandFunction function, bool autosave, std::size_t args, bool can_call_from_console, bool is_public) : \
        Command(name, category, help, params_help, function, autosave, args, args, can_call_from_console, is_public) {}

    void Command::register_command_impl(HMODULE module_handle) {
        if(m_name.empty()) {
            throw std::runtime_error("Cannot register command with empty name");
        }
        for(const auto &command : commands) {
            if(m_name == command->name()) {
                throw std::runtime_error("Command " + m_name + " already registered!");
            }
        }
        auto *plugin = Plugins::get_dll_plugin(module_handle);
        m_plugin = reinterpret_cast<void *>(plugin);
        m_full_name = get_full_name();
        commands.emplace_back(std::make_shared<Command>(*this));
    }

    void Command::load_commands_settings_impl(HMODULE module_handle) {
        if(module_handle == get_current_module()) {
            auto &config = Config::get_config();
            for(auto &command : commands) {
                auto command_key = std::string("commands.") + command->m_name;
                if(command->m_plugin && command->m_plugin == nullptr) {
                    if(config.exists(command_key)) {
                        bool failed = false;
                        auto command_value = config.get<std::string>(command_key);
                        if(command_value) {
                            auto arguments = split_arguments(command_value.value());
                        
                            auto arguments_alloc(std::make_unique<const char *[]>(arguments.size()));
                            for(std::size_t i = 0; i < arguments.size(); i++) {
                                arguments_alloc[i] = arguments[i].data();
                            }

                            bool res = command->call(arguments.size(), arguments_alloc.get());
                            if(res == COMMAND_RESULT_FAILED_ERROR) {
                                failed = true;
                            }
                        }
                        else {
                            failed = true;
                        }

                        if(failed) {
                            logger.warning("Command {} failed to load from config", command->m_name);
                        }
                    }
                }
            }
        }
        else {
            Plugins::Plugin *plugin = Plugins::get_dll_plugin(module_handle);
            if(!plugin) {
                throw std::runtime_error("Could not get plugin for module handle " + std::to_string(reinterpret_cast<std::uintptr_t>(module_handle)));
            }

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

    std::string Command::get_full_name() const noexcept {
        std::string module_name;
        if(this->m_plugin == nullptr) {
            module_name = "balltze";
        }
        else {
            module_name = reinterpret_cast<Plugins::Plugin *>(*m_plugin)->name();

            // replace spaces with underscores
            std::replace(module_name.begin(), module_name.end(), ' ', '_');
        }
        return to_lower(module_name + "_" + this->name());
    }

    std::vector<std::string> split_arguments(std::string command) noexcept {
        // This is the vector to return.
        std::vector<std::string> arguments;

        // This value will be true if we are inside quotes, during which the word will not separate into arguments.
        bool in_quotes = false;

        // If using a backslash, add the next character to the string regardless of what it is.
        bool escape_character = false;

        // Regardless of if there were any characters, there was an argument.
        bool allow_empty_argument = false;

        // Get the command
        std::size_t command_size = command.size();

        // Get the argument
        std::string argument;
        for(std::size_t i = 0; i < command_size; i++) {
            if(escape_character) {
                escape_character = false;
            }
            // Escape character - this will be used to include the next character regardless of what it is
            else if(command[i] == '\\') {
                escape_character = true;
                continue;
            }
            // If a whitespace or octotothorpe is in quotations in the argument, then it is considered part of the argument.
            else if(command[i] == '"') {
                in_quotes = !in_quotes;
                allow_empty_argument = true;
                continue;
            }
            else if((command[i] == ' ' || command[i] == '\r' || command[i] == '\n' || command[i] == '#') && !in_quotes) {
                // Add argument if not empty.
                if(argument != "" || allow_empty_argument) {
                    arguments.push_back(argument);
                    argument = "";
                    allow_empty_argument = false;
                }

                // Terminate if beginning a comment.
                if(command[i] == '#') {
                    break;
                }
                continue;
            }
            argument += command[i];
        }

        // Add the last argument.
        if(argument != "" || allow_empty_argument) {
            arguments.push_back(argument);
        }

        return std::move(arguments);
    }

    std::string unsplit_arguments(const std::vector<std::string> &arguments) noexcept {
        // This is the string to return.
        std::string unsplit;

        for(std::size_t i = 0; i < arguments.size(); i++) {
            // This is a reference to the argument we're dealing with.
            const std::string &argument = arguments[i];

            // This will be the final string we append to the unsplit string.
            std::string argument_final;

            // Set this to true if we need to surround this argument with quotes.
            bool surround_with_quotes = false;

            // Go through each character and add them one-by-one to argument_final.
            for(const char &c : argument) {
                switch(c) {
                    // Backslashes and quotation marks should be escaped.
                    case '\\':
                    case '"':
                        argument_final += '\\';
                        break;

                    // If we're using spaces or octothorpes, the argument should be surrounded with quotation marks. We could escape those, but this is more readable.
                    case '#':
                    case ' ':
                        surround_with_quotes = true;
                        break;

                    default:
                        break;
                }
                argument_final += c;
            }

            if(surround_with_quotes) {
                argument_final = std::string("\"") + argument_final + "\"";
            }

            unsplit += argument_final;

            // Add the space to separate the next argument.
            if(i + 1 < arguments.size()) {
                unsplit += " ";
            }
        }

        return unsplit;
    }

    CommandResult Command::execute_command_impl(HMODULE module_handle, std::string command) {
        std::vector<std::string> arguments = split_arguments(command);
        std::size_t arg_count = arguments.size() - 1;
        std::string command_name = arguments[0];
        arguments.erase(arguments.begin());

        auto arguments_alloc(std::make_unique<const char *[]>(arg_count));
        for(std::size_t i = 0; i < arg_count; i++) {
            arguments_alloc[i] = arguments[i].data();
        }
        
        CommandResult res = COMMAND_RESULT_FAILED_ERROR_NOT_FOUND;
        for(const auto command : commands) {
            if(command->m_full_name == command_name) {
                if(!command->plugin()) {
                    logger.error("Could not get plugin for module handle {}", reinterpret_cast<std::uintptr_t>(module_handle));
                    break;
                }

                Plugins::Plugin *command_plugin = reinterpret_cast<Plugins::Plugin *>(*command->plugin());
                Plugins::Plugin *module_plugin = Plugins::get_dll_plugin(module_handle);
                if(module_handle == get_current_module() || command->is_public() || module_plugin == command_plugin) {
                    res = command->call(arg_count, arguments_alloc.get());

                    // Save if autosave is enabled
                    if(command->m_autosave && res == COMMAND_RESULT_SUCCESS) {
                        bool is_balltze_command = command_plugin == nullptr;
                        if(is_balltze_command) {
                            auto &config = Config::get_config();
                            config.set(std::string("commands.") + command->m_name, unsplit_arguments(arguments));
                            config.save();
                        }
                        else {
                            auto directory = command_plugin->directory();
                            auto config = Config::Config(directory / "settings.json");
                            config.set(std::string("commands.") + command->m_name, unsplit_arguments(arguments));
                            config.save();
                        }
                    }
                }
                else {
                    logger.warning("Plugin {} tried to call command {} from plugin {}", module_plugin->name(), command->m_name, command_plugin->name());
                }              
                break;
            }
        }

        switch(res) {
            case CommandResult::COMMAND_RESULT_FAILED_NOT_ENOUGH_ARGUMENTS:
                Engine::console_printf("Command %s failed: not enough arguments", command_name.c_str());
                break;
            case CommandResult::COMMAND_RESULT_FAILED_TOO_MANY_ARGUMENTS:
                Engine::console_printf("Command %s failed: too many arguments", command_name.c_str());
                break;
            case CommandResult::COMMAND_RESULT_FAILED_ERROR:
                Engine::console_printf("Command %s failed: error", command_name.c_str());
                break;
            default:
                break;
        }

        return res;
    }

    extern "C" {
        void *unknown_command_sapp_hook_override_return = nullptr;
        void unknown_command_sapp_hook_override(); // the hook of the hook of the hook
        bool restore_unknown_command_message_print = false;
    }

    static void set_up_sapp_hook_override() noexcept {
        static auto *sapp_loader_hook_sig = Memory::get_signature("execute_console_command_sapp_loader_hook");
        static Memory::Hook *sapp_loader_hook_override = nullptr;
        if(!sapp_loader_hook_override && Memory::already_hooked(sapp_loader_hook_sig->data())) {
            sapp_loader_hook_override = Memory::override_function(sapp_loader_hook_sig->data(), unknown_command_sapp_hook_override, &unknown_command_sapp_hook_override_return);
        }
    }

    static void dispatch_commands(Event::ConsoleCommandEvent &event) {
        static auto *unknown_command_message_print = Memory::get_signature("console_unknown_command_message_print_call");

        if(restore_unknown_command_message_print) {
            const_cast<Memory::Signature *>(unknown_command_message_print)->restore();
            restore_unknown_command_message_print = false;
        }

        if(event.time == Event::EVENT_TIME_BEFORE) {
            if(execute_command(event.context.command) != COMMAND_RESULT_FAILED_ERROR_NOT_FOUND) {
                Memory::fill_with_nops(unknown_command_message_print->data(), 5);
                restore_unknown_command_message_print = true;

                // This is a workaround for sapp's hook
                set_up_sapp_hook_override();
            }
        }
    }

    void set_up_commands() {
        register_command("version", "balltze", "Prints the current version of Balltze", std::nullopt, [](int arg_count, const char **args) -> bool {
            auto version = balltze_version.to_string();
            Engine::console_printf("Balltze version %s", version.c_str());
            return true;
        }, false, 0, 0);

        Event::ConsoleCommandEvent::subscribe(dispatch_commands, Event::EVENT_PRIORITY_DEFAULT);

        set_up_commands_tab_completion();
        set_up_commands_help();
    }

    void remove_commands_from_plugin(PluginHandle plugin) {
        for(auto it = commands.begin(); it != commands.end(); ) {
            if((*it)->plugin() == plugin) {
                it = commands.erase(it);
            }
            else {
                ++it;
            }
        }
    }
}
