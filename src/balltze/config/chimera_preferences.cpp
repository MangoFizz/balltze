// SPDX-License-Identifier: GPL-3.0-only

#include <cstring>
#include <filesystem>
#include <fstream>
#include <balltze/command.hpp>
#include "config.hpp"
#include "chimera_preferences.hpp"

namespace Balltze::Config {
    const std::vector<std::string> *ChimeraPreferences::get_settings_for_command(const char *command) const {
        for(auto &c : m_settings) {
            if(std::strcmp(c.first.data(), command) == 0) {
                return &c.second;
            }
        }
        return nullptr;
    }

    void ChimeraPreferences::set_settings_for_command(const char *command, const std::vector<std::string> &settings) {
        for(auto &c : m_settings) {
            if(std::strcmp(c.first.data(), command) == 0) {
                if(c.second == settings) {
                    return;
                }
                m_settings.erase(m_settings.begin() + (&c - m_settings.data()));
                break;
            }
        }
        m_settings.emplace_back(command, settings);
    }

    void ChimeraPreferences::load() {
        std::ifstream config(m_path, std::ios_base::in);
        std::string line;
        while(std::getline(config, line)) {
            auto slices = split_arguments(line);
            if(!slices.empty()) {
                m_settings.emplace_back(slices[0], std::vector<std::string>(slices.begin() + 1, slices.end()));
            }
        }
    }

    ChimeraPreferences::ChimeraPreferences(const char *path) : m_path(path) {
        load();
    }

    ChimeraPreferences::ChimeraPreferences(const std::filesystem::path path) : m_path(path.string()) {
        load();
    }

    ChimeraPreferences &get_chimera_preferences() noexcept {
        static ChimeraPreferences preferences(LegacyApi::Engine::get_path() / "chimera" / "preferences.txt");
        return preferences;
    }
}
