// SPDX-License-Identifier: GPL-3.0-only

#include <fstream>
#include <nlohmann/json.hpp>
#include <impl/terminal/terminal.h>
#include <balltze/config.hpp>
#include <balltze/legacy_api/engine.hpp>
#include <balltze/legacy_api/plugin.hpp>
#include "../command/command.hpp"
#include "../logger.hpp"
#include "config.hpp"

namespace fs = std::filesystem;

namespace Balltze::Config {
    Config::Config(fs::path filepath, bool create) {
        this->filepath = filepath;
        try {
            if(!fs::exists(filepath) && !create) {
                throw std::runtime_error("Config file does not exist!");
            }
            create_file();
            load();
        }
        catch(std::runtime_error &e) {
            throw;
        }
    }

    Config::~Config() {
        //save();
    }

    void Config::create_file() {
        if(!fs::exists(filepath)) {
            fs::create_directories(filepath.parent_path());
            std::ofstream file(filepath);
            if(!file.is_open()) {
                throw std::runtime_error("Failed to create config file!");
            }
            file << "{}";
            file.close();
        }
    }

    std::vector<std::string> Config::split_key(std::string key) {
        std::vector<std::string> keys;
        std::string key_part;
        for(char c : key) {
            if(c == '.') {
                keys.push_back(key_part);
                key_part = "";
            }
            else {
                key_part += c;
            }
        }
        keys.push_back(key_part);
        return std::move(keys);
    }

    void Config::load() {
        std::ifstream file(filepath);
        if(!file.is_open()) {
            throw std::runtime_error("Failed to load config file!");
        }
        file >> config;
        file.close();
    }

    void Config::save() {
        std::ofstream file(filepath);
        if(!file.is_open()) {
            throw std::runtime_error("Failed to save config file!");
        }
        file << config.dump(4);
        file.close();
    }

    
    std::optional<std::string> Config::get(std::string key) {
        auto keys = split_key(key);
        nlohmann::json *slice = &config;
        for(std::string key : keys) {
            if(slice->contains(key)) {
                slice = &(*slice)[key];
            }
            else {
                return std::nullopt;
            }
        }
        if(slice->is_string()) {
            return std::move(slice->get<std::string>());
        }
        else {
            return std::nullopt;
        }
    }

    void Config::set(std::string key, std::string value) {
        auto keys = split_key(key);
        nlohmann::json *slice = &config;
        for(std::string key : keys) {
            if(slice->contains(key)) {
                slice = &(*slice)[key];
            }
            else {
                (*slice)[key] = nlohmann::json::object();
                slice = &(*slice)[key];
            }
        }
        *slice = value;
    }

    void Config::remove(std::string key) {
        auto key_slices = split_key(key);
        nlohmann::json *json = &config;
        auto it = key_slices.begin();
        while(it != key_slices.end()) {
            auto slice = *it;
            if(json->contains(slice)) {
                if(std::next(it) == key_slices.end()) {
                    json->erase(slice);
                    break;
                }
                else {
                    json = &(*json)[slice];
                }
            }
            else {
                throw std::runtime_error("Key does not exist!");
            }
            it++;
        }
    }

    bool Config::exists(std::string key) {
        auto keys = split_key(key);
        nlohmann::json *slice = &config;
        for(std::string key : keys) {
            if(slice->contains(key)) {
                slice = &(*slice)[key];
            }
            else {
                return false;
            }
        }
        return true;
    }

    std::filesystem::path get_balltze_directory() {
        try {
            auto path = LegacyApi::Engine::get_path() / "balltze";
            std::filesystem::create_directories(path);
            return path;
        }
        catch(...) {
            throw;
        }
    }

    Config &get_config() {
        static auto balltze_path = get_balltze_directory();
        std::filesystem::create_directories(balltze_path / "config");
        static auto config = Config(balltze_path / "config" / "settings.json");
        return config;
    }

    std::optional<Config> get_gamepad_config() {
        static std::optional<Config> gamepad_config;
        if(gamepad_config) {
            return gamepad_config;
        }
        auto config = get_config();
        auto gamepad = config.get("gamepad");
        if(gamepad) {
            try {
                auto balltze_path = get_balltze_directory();
                std::filesystem::create_directories(balltze_path / "config" / "gamepad");
                gamepad_config = Config(balltze_path / "config" / "gamepad" / (*gamepad + ".json"), false);
                return gamepad_config;
            }
            catch(std::runtime_error) {
                return std::nullopt;
            }
        }
        return std::nullopt;
    }

    void load_commands_settings() {
        auto &config = get_config();
        auto &commands = get_commands();
        terminal_mute_output(true);
        for(auto &command : commands) {
            auto command_key = std::string("commands.") + command->full_name();
            if(command->source() == COMMAND_SOURCE_PLUGIN || !config.exists(command_key)) {
                continue;
            }
            bool failed = false;
            auto command_value = config.get<std::string>(command_key);
            if(command_value) {
                auto arguments = split_arguments(command_value.value());
                bool res = command->call(arguments);
                if(res == COMMAND_RESULT_FAILED_ERROR) {
                    failed = true;
                }
            }
            else {
                failed = true;
            }

            if(failed) {
                logger.warning("Command {} failed to load from config", command->full_name());
            }
            else {
                logger.info("Command {} loaded from config", command->full_name());
            }
        }
        terminal_mute_output(false);
    }

    Ini get_chimera_ini() noexcept {
        static std::unique_ptr<Ini> ini;
        if(!ini) {
            if(std::filesystem::exists("chimera.ini")) {
                ini = std::make_unique<Ini>("chimera.ini");
            }
            else {
                ini = std::make_unique<Ini>();
            }
        }
        return *ini;
    }
}
