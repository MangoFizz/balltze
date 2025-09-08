// SPDX-License-Identifier: GPL-3.0-only

#include <memory>
#include <vector>
#include <balltze/events/console.hpp>
#include <impl/terminal/terminal.h>
#include "../config/config.hpp"
#include "../plugins/plugin.hpp"
#include "../plugins/loader.hpp"
#include "../logger.hpp"
#include "command.hpp"

namespace Balltze {
    static std::vector<std::unique_ptr<Command>> commands;

    CommandBuilder &CommandBuilder::name(const std::string &name) {
        m_name = name;
        return *this;
    }

    CommandBuilder &CommandBuilder::category(const std::string &category) {
        m_category = category;
        return *this;
    }

    CommandBuilder &CommandBuilder::help(const std::string &help) {
        m_help = help;
        return *this;
    }

    CommandBuilder &CommandBuilder::param(HscDataType type, const std::string &name, bool optional) {
        m_params_help.append(optional ? "[" : "<");
        m_params_help.append(name);
        m_params_help.append(": ");
        m_params_help.append(hsc_type_name(type));
        m_params_help.append(optional ? "]" : ">");
        m_params_help.append(" ");
        if(optional) {
            m_max_args++;
        }
        else {
            m_min_args++;
            m_max_args++;
        }
        return *this;
    }

    CommandBuilder &CommandBuilder::function(CommandFunction function) {
        m_function = function;
        return *this;
    }

    CommandBuilder &CommandBuilder::autosave(bool autosave) {
        m_autosave = autosave;
        return *this;
    }

    CommandBuilder &CommandBuilder::can_call_from_console(bool can_call) {
        m_can_call_from_console = can_call;
        return *this;
    }

    CommandBuilder &CommandBuilder::is_public(bool is_public) {
        m_is_public = is_public;
        return *this;
    }

    void CommandBuilder::create(int source) const noexcept {
        if(!validate()) {
            throw std::runtime_error("Command is invalid or incomplete");
        }

        HMODULE caller_module_handle = Memory::get_caller_module_handle();
        CommandSource command_source = CommandSource(source);
        Plugins::Plugin *command_plugin = nullptr;
        if(caller_module_handle != get_current_module()) {
            auto *plugin = Plugins::get_dll_plugin(caller_module_handle);
            if(!plugin) {
                throw std::runtime_error("Could not find plugin for command " + m_name);
            }
            command_source = COMMAND_SOURCE_PLUGIN;
            command_plugin = plugin;
        }

        commands.emplace_back(std::make_unique<Command>(m_name, m_category, m_help,
            m_params_help.empty() ? std::nullopt : std::optional<std::string>(m_params_help), m_function, m_autosave,
            m_min_args, m_max_args, m_can_call_from_console, m_is_public, command_source, command_plugin));
    }

    bool CommandBuilder::validate() const {
        if(m_name.empty()) {
            logger.error("Command name is empty");
            return false;
        }
        if(m_category.empty()) {
            logger.error("Command category is empty");
            return false;
        }
        if(m_help.empty()) {
            logger.error("Command help is empty");
            return false;
        }
        if(m_function == nullptr) {
            logger.error("Command function is null");
            return false;
        }
        if(m_min_args > m_max_args) {
            logger.error("Command {} has a minimum argument count greater than the maximum argument count", m_name);
            return false;
        }
        return true;
    }

    const char *Command::name() const noexcept {
        return this->m_name.c_str();
    }

    CommandSource Command::source() const noexcept {
        return this->m_source;
    }

    std::optional<Plugins::Plugin *> Command::plugin() const noexcept {
        if(this->m_source != COMMAND_SOURCE_PLUGIN) {
            return std::nullopt;
        }
        return this->m_plugin;
    }

    const char *Command::full_name() const noexcept {
        return this->m_full_name.c_str();
    }

    const char *Command::category() const noexcept {
        return this->m_category.c_str();
    }

    const char *Command::help() const noexcept {
        return this->m_help.c_str();
    }

    const char *Command::params_help() const noexcept {
        if(m_params_help.has_value()) {
            return this->m_params_help->c_str();
        }
        return nullptr;
    }

    std::size_t Command::min_args() const noexcept {
        return this->m_min_args;
    }

    std::size_t Command::max_args() const noexcept {
        return this->m_max_args;
    }

    bool Command::autosave() const noexcept {
        return this->m_autosave;
    }

    bool Command::can_call_from_console() const noexcept {
        return this->m_can_call_from_console;
    }

    bool Command::is_public() const noexcept {
        return this->m_public;
    }

    CommandResult Command::call(const std::vector<std::string> &arguments) const noexcept {
        if(m_function == nullptr) {
            logger.debug("Command {} function has a null pointer", m_name);
            return COMMAND_RESULT_FAILED_ERROR;
        }
        if(arguments.size() > this->max_args()) {
            return COMMAND_RESULT_FAILED_TOO_MANY_ARGUMENTS;
        }
        else if(arguments.size() < this->min_args()) {
            return COMMAND_RESULT_FAILED_NOT_ENOUGH_ARGUMENTS;
        }
        else {
            return m_function(arguments) ? COMMAND_RESULT_SUCCESS : COMMAND_RESULT_FAILED_ERROR;
        }
    }

    static auto to_lower(std::string str) {
        std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
            return std::tolower(c);
        });
        return str;
    }

    Command::Command(std::string name, std::string category, std::string help, std::optional<std::string> params_help, 
        CommandFunction function, bool autosave, std::size_t min_args, std::size_t max_args, bool can_call_from_console, 
        bool is_public, CommandSource source, Plugins::Plugin *plugin) : 
        m_name(name), m_category(category), m_help(help), m_params_help(params_help), m_function(function), 
        m_autosave(autosave), m_min_args(min_args), m_max_args(max_args), m_can_call_from_console(can_call_from_console), 
        m_public(is_public), m_source(source), m_plugin(plugin) {
        
        m_name = to_lower(m_name);
        if(m_min_args > m_max_args) {
            throw std::runtime_error("Command " + m_name + " has a minimum argument count greater than the maximum argument count");
        }
        if(function == nullptr) {
            throw std::runtime_error("Command " + m_name + " has a null function");
        }
        generate_full_name();
    }

    void Command::generate_full_name() noexcept {
        std::string module_name;
        switch (this->m_source) {
            case COMMAND_SOURCE_BALLTZE:
                module_name = "balltze";
                break;
            
            case COMMAND_SOURCE_RINGWORLD:
                module_name = "ringworld";
                break;
            
            case COMMAND_SOURCE_PLUGIN:
                module_name = m_plugin->name();
                std::replace(module_name.begin(), module_name.end(), ' ', '_');
                break;
            
            default:
                logger.error("Command {} has an invalid source {}", this->m_name, static_cast<int>(this->m_source));
                module_name = "unknown";
                break;
        }
        m_full_name = to_lower(module_name + "_" + this->name());
    }

    const std::vector<std::unique_ptr<Command>> &get_commands() noexcept {
        return commands;
    }

    void add_command(const Command &command) noexcept {
        commands.emplace_back(std::make_unique<Command>(command));
    }

    void remove_commands_from_plugin(PluginHandle plugin) {
        for(auto it = commands.begin(); it != commands.end(); ) {
            if((*it)->plugin() == plugin) {
                it = commands.erase(it);
            }
            else {
                it++;
            }
        }
    }

    CommandResult execute_command(const std::string &command_input) {
        std::vector<std::string> arguments = split_arguments(command_input);
        std::string command_name = arguments[0];
        arguments.erase(arguments.begin());

        HMODULE caller_module_handle = Memory::get_caller_module_handle();
        if(caller_module_handle == nullptr) {
            logger.error("Could not get module handle from command execution context");
            return COMMAND_RESULT_FAILED_ERROR;
        }
        
        // Find the command
        const Command *command = nullptr;
        for(const auto &cmd : commands) {
            if(cmd->full_name() == command_name) {
                command = cmd.get();
                break;
            }
        }
        if(command == nullptr) {
            return COMMAND_RESULT_FAILED_ERROR_NOT_FOUND;
        }

        // If the command is from a plugin, ensure the caller is either from the same plugin or the command is public
        if(command->source() == COMMAND_SOURCE_PLUGIN) {
            if(!command->plugin()) {
                logger.error("Could not get plugin for command {}", command->full_name());
                return COMMAND_RESULT_FAILED_ERROR;
            }
            if(caller_module_handle != get_current_module()) {
                auto *plugin = Plugins::get_dll_plugin(caller_module_handle);
                if(!plugin) {
                    logger.warning("Module handle {} tried to call command {} from plugin, but is not a plugin", reinterpret_cast<std::uintptr_t>(caller_module_handle), command->full_name());
                    return COMMAND_RESULT_FAILED_ERROR;
                }
                if(plugin != command->plugin() && !command->is_public()) {
                    return COMMAND_RESULT_FAILED_ERROR_NOT_FOUND;
                }
            }
        }

        // Call the command
        CommandResult res = command->call(arguments);

        // Save if autosave is enabled
        if(res == COMMAND_RESULT_SUCCESS && command->autosave() && arguments.size() > 0) {
            if(command->source() != COMMAND_SOURCE_PLUGIN) {
                auto &config = Config::get_config();
                config.set(std::string("commands.") + command->full_name(), unsplit_arguments(arguments));
                config.save();
            }
            else {
                auto directory = command->plugin().value()->directory();
                auto config = Config::Config(directory / "settings.json");
                config.set(std::string("commands.") + command->name(), unsplit_arguments(arguments));
                config.save();
            }
        }

        switch(res) {
            case CommandResult::COMMAND_RESULT_FAILED_NOT_ENOUGH_ARGUMENTS:
                terminal_error_printf("Command %s failed: not enough arguments", command_name.c_str());
                break;
            case CommandResult::COMMAND_RESULT_FAILED_TOO_MANY_ARGUMENTS:
                terminal_error_printf("Command %s failed: too many arguments", command_name.c_str());
                break;
            case CommandResult::COMMAND_RESULT_FAILED_ERROR:
                terminal_error_printf("Command %s failed: error", command_name.c_str());
                break;
            default:
                break;
        }
        return res;
    }

    void set_up_commands() {
        set_up_commands_tab_completion();
        set_up_commands_help();
        Events::ConsoleCommandEvent::subscribe([](Events::ConsoleCommandEvent &event) {
            if(execute_command(event.console_input()) != COMMAND_RESULT_FAILED_ERROR_NOT_FOUND) {
                event.cancel();
            }
        }, Events::EVENT_PRIORITY_HIGHEST);
    }
}
