// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/engine/path.hpp>
#include <fstream>
#include <balltze/config.hpp>
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
        save();
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
        auto keys = split_key(key);
        nlohmann::json *slice = &config;
        for(std::string key : keys) {
            if(slice->contains(key)) {
                slice = &(*slice)[key];
            }
            else {
                return;
            }
        }
        *slice = nlohmann::json::object();
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

    std::filesystem::path get_balltze_directory() noexcept {
        auto path = Engine::get_path() / "balltze";
        std::filesystem::create_directories(path);
        return path;
    }
}
