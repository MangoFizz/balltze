// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__COMMAND_HPP
#define BALLTZE_API__COMMAND_HPP

#include <cstring>
#include <string>
#include <vector>
#include <functional>
#include <impl/script/hsc.h>
#include "api.hpp"

namespace Balltze {
    #define BOOL_TO_STR(boolean) (boolean ? "true" : "false")
    #define STR_TO_BOOL(str) (std::strcmp(str, "1") == 0 || std::strcmp(str, "true") == 0)

    /**
     * Function type for a command
     * @param arguments arguments passed to the command
     * @return          true if success; false if failure
     */
    using CommandFunction = std::function<bool(const std::vector<std::string> &arguments)>;

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

    BALLTZE_API class CommandBuilder {
    public:
        /**
         * Set the name of the command.
         * This is required for the command to be valid.
         * 
         * @param name name of the command
         * @return     reference to the builder
         */
        CommandBuilder &name(const std::string &name);

        /**
         * Set the category of the command
         * This is required for the command to be valid.
         * 
         * @param category category of the command
         * @return        reference to the builder
         */
        CommandBuilder &category(const std::string &category);

        /**
         * Set the help of the command
         * This is required for the command to be valid.
         * 
         * @param help help of the command
         * @return     reference to the builder
         */
        CommandBuilder &help(const std::string &help);

        /**
         * Add a parameter to the command.
         * Parameters are added in the order they are specified.
         * 
         * @param type     type of the parameter
         * @param name     name of the parameter
         * @param optional whether the parameter is optional (default: false)
         * @return         reference to the builder
         */
        CommandBuilder &param(HscDataType type, const std::string &name, bool optional = false);

        /**
         * Set the function of the command
         * This is required for the command to be valid.
         * 
         * @param function function of the command
         * @return         reference to the builder
         */
        CommandBuilder &function(CommandFunction function);

        /**
         * Set whether the command automatically saves.
         * If this function is not called, by default, the command will not automatically save.
         * 
         * @param autosave true if command should automatically save
         * @return         reference to the builder
         */
        CommandBuilder &autosave(bool autosave = true);

        /**
         * Set whether the command can be called from console.
         * If this function is not called, by default, the command cannot be called from console.
         * 
         * @param can_call whether the command can be called from console; true by default
         * @return         reference to the builder
         */
        CommandBuilder &can_call_from_console(bool can_call = true);

        /**
         * Set whether the command is public
         * If this function is not called, by default, the command is not public.
         * 
         * @param is_public true if command is public
         * @return          reference to the builder
         */
        CommandBuilder &is_public(bool is_public);

        /**
         * Reset the builder to its initial state
         */
        void reset();

        /**
         * Create the command and register it in Balltze
         * 
         * @param r reserved for internal use
         * @throw std::runtime_error if the command is invalid, incomplete or could not be created
         */
        virtual void create(int r = 2) const noexcept;

    protected:
        std::string m_name;
        std::string m_category;
        std::string m_help;
        CommandFunction m_function = nullptr;
        bool m_autosave = false;
        bool m_can_call_from_console = true;
        bool m_is_public = false;
        std::string m_params_help;
        std::size_t m_min_args = 0;
        std::size_t m_max_args = 0;

        /**
         * Validate the command
         * 
         * @return true if the command is valid; false otherwise
         */
        bool validate() const;
    };

    /**
     * Execute a command. Command must be registered.
     * 
     * @param command   command to execute
     */
    CommandResult execute_command(const std::string &command);

    /**
     * Load commands settings
     */
    void load_commands_settings();
}

#endif
