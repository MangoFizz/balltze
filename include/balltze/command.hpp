// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__COMMAND_HPP
#define BALLTZE_API__COMMAND_HPP

#include <cstring>
#include <string>
#include <vector>
#include <optional>
#include "utils.hpp"
#include "api.hpp"

namespace Balltze {
    #define BOOL_TO_STR(boolean) (boolean ? "true" : "false")
    #define STR_TO_BOOL(str) (std::strcmp(str, "1") == 0 || std::strcmp(str, "true") == 0)

    using PluginHandle = void *;

    /**
     * Result of a command
     */
    enum CommandResult {
        /** Command was a success. If invoked by console and the command can save, save here. */
        COMMAND_RESULT_SUCCESS = 0,

        /** Command failed. Do not save. */
        COMMAND_RESULT_FAILED_ERROR,

        /** Command not found. Do not save. */
        COMMAND_RESULT_FAILED_ERROR_NOT_FOUND,

        /** Command requires more arguments than given. Do not save. */
        COMMAND_RESULT_FAILED_NOT_ENOUGH_ARGUMENTS,

        /** Command requires fewer arguments than given. Do not save. */
        COMMAND_RESULT_FAILED_TOO_MANY_ARGUMENTS
    };

    /**
     * Function type for a command
     * @param arg_count Number of arguments
     * @param args      Arguments given; not valid if arg_count is 0
     * @return          true if success; false if failure
     */
    using CommandFunction = bool (*)(int arg_count, const char **args);

    /**
     * Command the user can call to execute functions
     */
    class BALLTZE_API Command {
    public:
        /**
         * Get the name of the command
         * @return pointer to the name of the command
         */
        const char *name() const noexcept {
            return this->m_name.c_str();
        }

        /**
         * Get the handle of the plugin that registered the command
         */
        std::optional<PluginHandle> plugin() const noexcept {
            return this->m_plugin;
        }

        /**
         * Get the full name of the command
         * @return pointer to the full name of the command
         */
        const char *full_name() const noexcept {
            return this->m_full_name->c_str();
        }

        /**
         * Get the category of the command
         * @return pointer to the category of the command
         */
        const char *category() const noexcept {
            return this->m_category.c_str();
        }

        /**
         * Get the help of the command
         * @return pointer to the help of the command
         */
        const char *help() const noexcept {
            return this->m_help.c_str();
        }

        /**
         * Get the help of the parameters of the command
         * @return pointer to the help of the parameters of the command
         */
        const char *params_help() const noexcept {
            if(m_params_help.has_value()) {
                return this->m_params_help->c_str();
            }
            return nullptr;
        }

        /**
         * Get the minimum arguments of the command
         * @return minimum arguments
         */
        std::size_t min_args() const noexcept {
            return this->m_min_args;
        }

        /**
         * Get the maximum arguments of the command
         * @return maximum arguments
         */
        std::size_t max_args() const noexcept {
            return this->m_max_args;
        }

        /**
         * Return whether the command automatically saves
         * @return true if command should automatically save
         */
        bool autosave() const noexcept {
            return this->m_autosave;
        }

        /**
         * Return whether the command can be called from console
         * @return true if command can be called from console
         */
        bool can_call_from_console() const noexcept {
            return this->m_can_call_from_console;
        }

        /**
         * Return whether the command is public
         * @return true if command is public
         */
        bool is_public() const noexcept {
            return this->m_public;
        }

        /**
         * Call the function with the given arguments
         * @param  arg_count number of arguments to pass
         * @param  args      array of arguments
         * @return           result of command
         */
        virtual CommandResult call(std::size_t arg_count, const char **args) const noexcept;

        /**
         * Call the function with the given arguments
         * @param  args array of arguments
         * @return      result of command
         */
        CommandResult call(const std::vector<std::string> &arguments) const noexcept;

        /**
         * Instantiate a command
         * @param name     name of the command
         * @param category category of the command
         * @param help     help info of the command
         * @param function function pointer of the command
         * @param autosave auto saves if successful and at least 1 arg was passed
         * @param min_args minimum arguments
         * @param max_args maximum arguments
         */
        Command(std::string name, std::string category, std::string help, std::optional<std::string> params_help, CommandFunction function, bool autosave, std::size_t min_args, std::size_t max_args, bool can_call_from_console = true, bool is_public = false);

        /**
         * Instantiate a command
         * @param name     name of the command
         * @param category category of the command
         * @param help     help info of the command
         * @param function function pointer of the command
         * @param autosave auto saves if successful and at least 1 arg was passed
         * @param args     required number of arguments
         */
        Command(std::string name, std::string category, std::string help, std::optional<std::string> params_help, CommandFunction function, bool autosave, std::size_t args = 0, bool can_call_from_console = true, bool is_public = false);

        /**
         * Register the command to the command list
         */
        inline void register_command() {
            try {
                this->register_command_impl(get_current_module());
            }
            catch(...) {
                throw;
            }
        }

    protected:
        /** Name of the command */
        std::string m_name;

        /** Full name including prefix (if command is registered) */
        std::optional<std::string> m_full_name;

        /** Plugin that registered the command */
        std::optional<PluginHandle> m_plugin;

        std::string get_full_name() const noexcept;
        Command(std::string name, std::string category, std::string help, std::optional<std::string> params_help, bool autosave, std::size_t min_args, std::size_t max_args, bool can_call_from_console, bool is_public);

    private:
        /** Category of the command */
        std::string m_category;

        /** Help of the command */
        std::string m_help;

        /** Help of the parameters of the command */
        std::optional<std::string> m_params_help;

        /** Function to call for the command */
        CommandFunction m_function;

        /** Command automatically saves */
        bool m_autosave;

        /** Minimum required arguments for the command */
        std::size_t m_min_args;

        /** Maximum required arguments for the command */
        std::size_t m_max_args;

        /** Can be called from console */
        bool m_can_call_from_console;

        /** Is public? Can be called from other plugins */
        bool m_public;

        void register_command_impl(HMODULE module_handle);
        friend CommandResult execute_command(std::string command);
        static CommandResult execute_command_impl(HMODULE module_handle, std::string command);
        friend void load_commands_settings();
        static void load_commands_settings_impl(HMODULE module_handle);
    };

    /** 
     * Split arguments from a console command
     * @param command   Console command input
     * @return          Splitted arguments
     */
    BALLTZE_API std::vector<std::string> split_arguments(std::string command) noexcept;

    /**
     * Unsplit the arguments
     * @param  arguments arguments to unsplit
     * @return           combined arguments
     */
    BALLTZE_API std::string unsplit_arguments(const std::vector<std::string> &arguments) noexcept;

    /**
     * Register a command
     * @param name                  name of the command
     * @param category              category of the command
     * @param help                  help info of the command
     * @param function              function pointer of the command
     * @param autosave              auto saves if successful and at least 1 arg was passed
     * @param min_args              minimum arguments
     * @param max_args              maximum arguments
     * @param can_call_from_console can be called from console
     * @param is_public             is public? can be called from other plugins
     */
    inline void register_command(std::string name, std::string category, std::string help, std::optional<std::string> params_help, CommandFunction function, bool autosave, std::size_t min_args, std::size_t max_args, bool can_call_from_console = true, bool is_public = false) noexcept {
        Command command(name, category, help, params_help, function, autosave, min_args, max_args, can_call_from_console, is_public);
        command.register_command();
    }

    /**
     * Execute a command. Command must be registered.
     * @param command   command to execute
     */
    inline CommandResult execute_command(std::string command) {
        try {
            return Command::execute_command_impl(get_current_module(), command);
        }
        catch(...) {
            throw;
        }
    }

    /**
     * Load commands settings
     */
    inline void load_commands_settings() {
        try {
            Command::load_commands_settings_impl(get_current_module());
        }
        catch(...) {
            throw;
        }
    }    
}

#endif
