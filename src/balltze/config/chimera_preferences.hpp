// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__CONFIG__CHIMERA_PREFERENCES_HPP
#define BALLTZE__CONFIG__CHIMERA_PREFERENCES_HPP

#include <filesystem>
#include <vector>
#include <string>

namespace Balltze::Config {
    class ChimeraPreferences {
    public:
        /**
         * Get a pointer to the settings for the specified command
         * @param command command to retrieve settings
         * @return        pointer to settings or nullptr if no settings were found
         */
        const std::vector<std::string> *get_settings_for_command(const char *command) const;

        /**
         * Set the settings for the specified command
         * @param command  command to set settings
         * @param settings settings to set to
         */
        void set_settings_for_command(const char *command, const std::vector<std::string> &settings);

        /**
         * Open the config file
         * @param path path to config file
         */
        ChimeraPreferences(const char *path);
        ChimeraPreferences(const std::filesystem::path path);

    private:
        /** Path of the config file */
        std::string m_path;

        /** Settings (command, settings pair) */
        std::vector<std::pair<std::string, std::vector<std::string>>> m_settings;

        /** Load */
        void load();
    };

    /**
     * Get the Chimera preferences
     * @return Chimera preferences
     */
    ChimeraPreferences &get_chimera_preferences() noexcept;
}

#endif
