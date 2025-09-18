// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__COMMAND__COMMAND_HPP
#define BALLTZE__COMMAND__COMMAND_HPP

#include <balltze/command.hpp>
#include "../plugins/plugin.hpp"

namespace Balltze {
    enum CommandSource {
        COMMAND_SOURCE_BALLTZE = 0,
        COMMAND_SOURCE_RINGWORLD,
        COMMAND_SOURCE_CHIMERA,
        COMMAND_SOURCE_PLUGIN
    };

    class Command {
        friend CommandBuilder;
    public:
        /**
         * Get the name of the command
         * 
         * @return pointer to the name of the command
         */
        const char *name() const noexcept;

        /**
         * Get the source of the command
         * 
         * @return source of the command
         */
        CommandSource source() const noexcept;

        /**
         * Get the handle of the plugin that registered the command
         * 
         * @return handle of the plugin that registered the command, or std::nullopt if is not from a plugin
         */
        std::optional<Plugins::Plugin *> plugin() const noexcept;

        /**
         * Get the full name of the command
         * 
         * @return pointer to the full name of the command
         */
        const char *full_name() const noexcept;

        /**
         * Get the category of the command
         * 
         * @return pointer to the category of the command
         */
        const char *category() const noexcept;

        /**
         * Get the help of the command
         * 
         * @return pointer to the help of the command
         */
        const char *help() const noexcept;

        /**
         * Get the help of the parameters of the command
         * 
         * @return pointer to the help of the parameters of the command
         */
        const char *params_help() const noexcept;

        /**
         * Get the minimum arguments of the command
         * 
         * @return minimum arguments
         */
        std::size_t min_args() const noexcept;

        /**
         * Get the maximum arguments of the command
         * 
         * @return maximum arguments
         */
        std::size_t max_args() const noexcept;

        /**
         * Return whether the command automatically saves
         * 
         * @return true if command should automatically save
         */
        bool autosave() const noexcept;

        /**
         * Return whether the command can be called from console
         * 
         * @return true if command can be called from console
         */
        bool can_call_from_console() const noexcept;

        /**
         * Return whether the command is public
         * 
         * @return true if command is public
         */
        bool is_public() const noexcept;

        /**
         * Get the default value of the command
         * 
         * @return default value of the command
         */
        const std::string &default_value() const noexcept;

        /**
         * Call the function with the given arguments
         * 
         * @param  args array of arguments
         * @return      result of command
         */
        virtual CommandResult call(const std::vector<std::string> &arguments) const noexcept;

        /**
         * Instantiate a command
         * 
         * @param name     name of the command
         * @param category category of the command
         * @param help     help info of the command
         * @param function function pointer of the command
         * @param autosave auto saves if successful and at least 1 arg was passed
         * @param min_args minimum arguments
         * @param max_args maximum arguments
         * @param can_call_from_console whether the command can be called from console
         * @param is_public whether the command is public
         * @param default_value default value of the command (only if it has parameters)
         * @param source  source of the command (balltze, ringworld, plugin)
         * @param plugin  handle of the plugin that registered the command (only if source is plugin)
         */
        Command(std::string name, std::string category, std::string help, std::optional<std::string> params_help, 
            CommandFunction function, bool autosave, std::size_t min_args, std::size_t max_args, bool can_call_from_console = true, 
            bool is_public = false, std::string default_value = "", CommandSource source = COMMAND_SOURCE_BALLTZE, Plugins::Plugin *plugin = nullptr);

    protected:
        std::string m_name;
        std::string m_full_name;
        CommandSource m_source = COMMAND_SOURCE_BALLTZE;
        Plugins::Plugin *m_plugin = nullptr;

        /**
         * Get the full name of the command including prefix
         * @return full name of the command
         */
        void generate_full_name() noexcept;

    private:
        std::string m_category;
        std::string m_help;
        std::optional<std::string> m_params_help;
        CommandFunction m_function;
        bool m_autosave = false;
        std::string m_default_value;
        std::size_t m_min_args = 0;
        std::size_t m_max_args = 0;
        bool m_can_call_from_console = true;
        bool m_public = false;
    };

    /** 
     * Split arguments from a console command
     * @param command   Console command input
     * @return          Splitted arguments
     */
    std::vector<std::string> split_arguments(std::string command) noexcept;

    /**
     * Unsplit the arguments
     * @param  arguments arguments to unsplit
     * @return           combined arguments
     */
    std::string unsplit_arguments(const std::vector<std::string> &arguments) noexcept;

    /**
     * Get all registered commands
     * 
     * @return reference to the vector of registered commands
     */
    const std::vector<std::unique_ptr<Command>> &get_commands() noexcept;

    /**
     * Add a command to the list of registered commands
     * 
     * @param command command to add
     */
    void add_command(const Command &command) noexcept;

    /**
     * Remove all commands registered by a plugin
     * @param plugin handle of the plugin
     */
    void remove_commands_from_plugin(PluginHandle plugin);

    /**
     * Set up commands system
     */
    void set_up_commands();

    /**
     * Set up hooks for console commands tab completion
     */
    void set_up_commands_tab_completion();

    /**
     * Set up hooks for commands help
     */
    void set_up_commands_help();
}

#endif
