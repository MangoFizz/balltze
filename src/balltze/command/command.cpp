// SPDX-License-Identifier: GPL-3.0-only

#include <memory>
#include <map>
#include <cstring>
#include <balltze/command.hpp>
#include <balltze/memory.hpp>
#include <balltze/engine/core.hpp>
#include "../event/console_command.hpp"
#include "../plugins/loader.hpp"
#include "../version.hpp"
#include "../logger.hpp"

namespace Balltze {
    static std::map<HMODULE, std::vector<Command>> commands;

    CommandResult Command::call(std::size_t arg_count, const char **args) const noexcept {
        if(arg_count > this->max_args()) {
            return CommandResult::COMMAND_RESULT_FAILED_TOO_MANY_ARGUMENTS;
        }
        else if(arg_count < this->min_args()) {
            return CommandResult::COMMAND_RESULT_FAILED_NOT_ENOUGH_ARGUMENTS;
        }
        else {
            return this->m_function(arg_count, args) ? CommandResult::COMMAND_RESULT_SUCCESS : CommandResult::COMMAND_RESULT_FAILED_ERROR;
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

    Command::Command(const char *name, const char *category, const char *help, CommandFunction function, bool autosave, std::size_t min_args, std::size_t max_args, bool can_call_from_console, bool is_public) : 
        m_name(name), m_category(category), m_help(help), m_function(function), m_autosave(autosave), m_min_args(min_args), m_max_args(max_args), m_can_call_from_console(can_call_from_console), m_public(is_public) {}

    Command::Command(const char *name, const char *category, const char *help, CommandFunction function, bool autosave, std::size_t args, bool can_call_from_console, bool is_public) : \
        Command(name, category, help, function, autosave, args, args, can_call_from_console, is_public) {}

    void Command::register_command_impl(HMODULE module_handle) {
        if(commands.find(module_handle) == commands.end()) {
            commands[module_handle] = std::vector<Command>();
        }
        else {
            for(const Command &command : commands[module_handle]) {
                if(std::strcmp(command.name(), this->name()) == 0) {
                    throw std::runtime_error("Command " + std::string(this->name()) + " already registered!");
                }
            }
        }
        commands[module_handle].push_back(*this);
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

    static bool restore_unknown_command_message_print = false;

    static void dispatch_commands(Event::ConsoleCommandEvent &event) {
        static auto *unknown_command_message_print = Memory::get_signature("console_unknown_command_message_print_call");

        if(restore_unknown_command_message_print) {
            const_cast<Memory::Signature *>(unknown_command_message_print)->restore();
            restore_unknown_command_message_print = false;
        }

        if(event.time == Event::EVENT_TIME_BEFORE) {
            // Get the arguments
            std::vector<std::string> arguments = split_arguments(event.args.command);

            // Get the argument count
            std::size_t arg_count = arguments.size();

            // If no arguments were passed, just call it.
            if(arg_count == 0) {
                return;
            }

            // Make our array
            auto arguments_alloc(std::make_unique<const char *[]>(arg_count));
            for(std::size_t i = 0; i < arg_count; i++) {
                arguments_alloc[i] = arguments[i].data();
            }

            bool cancel = false;

            // Do it!
            for(const auto &[handle, commands] : commands) {
                std::string module_name;

                if(handle == get_current_module()) {
                    module_name = "balltze";
                }
                else {
                    auto plugin = Plugins::get_dll_plugin(handle);
                    if(!plugin) {
                        continue;
                    }
                    module_name = plugin->name();
                }

                for(const Command &command : commands) {
                    auto to_lower = [](std::string str) {
                        std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
                            return std::tolower(c);
                        });
                        return str;
                    };

                    auto full_command_name = to_lower(module_name + "_" + command.name());
                    auto input = to_lower(arguments_alloc[0]);
                    if(full_command_name == input) {
                        auto res = command.call(arg_count - 1, &arguments_alloc[1]);
                        if(res == CommandResult::COMMAND_RESULT_SUCCESS) {
                            cancel = true;
                        }
                    }
                }
            }

            if(cancel) {
                Memory::fill_with_nops(unknown_command_message_print->data(), 5);
                restore_unknown_command_message_print = true;
            }
        }
    }

    void set_up_commands() {
        register_command("version", "balltze", "Prints the version of Balltze", [](int arg_count, const char **args) -> bool {
            std::string version = BALLTZE_VERSION;
            Engine::console_printf("Balltze version %s", version.c_str());
            logger.info("Balltze version {}", version);
            return true;
        }, false, 0, 0);

        Event::ConsoleCommandEvent::subscribe(dispatch_commands, Event::EVENT_PRIORITY_DEFAULT);
    }
}
