// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__CONFIG_HPP
#define BALLTZE_API__CONFIG_HPP

#include <string>
#include <vector>
#include <utility>
#include <istream>
#include <optional>
#include <filesystem>
#include <nlohmann/json.hpp>
#include "api.hpp"

namespace Balltze::Config {
    class BALLTZE_API Config {
    private:
        /** Path to the config file */
        std::filesystem::path filepath;

        /** JSON object for config */
        nlohmann::json config;

        /** 
         * Create config file if it doesn't exist 
         * @throws std::runtime_error if config file cannot be created
         */
        void create_file();

        /**
         * Split key into vector of keys
         * @param key Dotted key to split
         * @return Vector of keys
         */
        std::vector<std::string> split_key(std::string key);
        
    public:
        /**
         * Constructor for Config class
         * @param filepath Path to config file
         * @throws std::runtime_error if config file cannot be created
         */
        Config(std::filesystem::path filepath, bool create = true);

        /**
         * Default constructor for Config class
         */
        Config() = default;

        /**
         * Destructor for Config class
         * @throws std::runtime_error if config file cannot be saved
         */
        ~Config();

        /**
         * Load config file
         * @throws std::runtime_error if config file cannot be loaded
         */
        void load();

        /**
         * Save config file
         * @throws std::runtime_error if config file cannot be saved
         */
        void save();

        /**
         * Get value from config
         * @param key   Key to get value from
         * @return      Value if key exists, std::nullopt otherwise
         * @tparam T    Type of value
         */
        template<typename T>
        std::optional<T> get(std::string key) {
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
            if(slice->is_primitive()) {
                return slice->get<T>();
            }
            else {
                return std::nullopt;
            }
        }

        /**
         * Get value from config
         * @param key   Key to get value from
         * @return      Value if key exists, std::nullopt otherwise
         */
        std::optional<std::string> get(std::string key);

        /**
         * Get array from config
         * @param key Key to get array from
         * @return Array if key exists, std::nullopt otherwise
         * @tparam T Type of array
         */
        template<typename T>
        std::optional<std::vector<T>> get_array(std::string key) {
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
            if(slice->is_array()) {
                std::vector<T> array;
                for(auto &item : *slice) {
                    array.push_back(item.get<T>());
                }
                return std::move(array);
            }
            else {
                return std::nullopt;
            }
        }

        /**
         * Set value in config
         * @param key Key to set value for
         * @param value Value to set
         */
        void set(std::string key, std::string value);

        /**
         * Set array in config
         * @param key Key to set array for
         * @param array Array to set
         * @tparam T Type of array
         */
        template<typename T>
        void set_array(std::string key, std::vector<T> array) {
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
            *slice = nlohmann::json::array();
            for(T item : array) {
                (*slice).push_back(item);
            }
        }

        /**
         * Delete key from config
         * @param key Key to delete
         */
        void remove(std::string key);

        /**
         * Check if key exists in config
         * @param key Key to check
         * @return True if key exists, false otherwise
         */
        bool exists(std::string key);

        /**
         * Add value to array in config
         * @param key Key to add value to
         * @param value Value to add
         * @tparam T Type of value
         */
        template<typename T>
        void add_array_value(std::string key, T value) {
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
            if(slice->is_array()) {
                (*slice).push_back(value);
            }
            else {
                *slice = nlohmann::json::array();
                (*slice).push_back(value);
            }
        }
    };

    /**
     * From Chimera
     * https://github.com/SnowyMouse/chimera/blob/master/src/chimera/config/ini.hpp
     */
    class BALLTZE_API Ini {
    public:
        /**
         * Get the value by name
         * @param  key name of the value
         * @return     pointer to the value if found, or nullptr if not
         */
        const char *get_value(const char *key) const noexcept;

        /**
         * Get the value by name
         * @param  key name of the value
         * @return     string value of the key or nullopt if not set
         */
        std::optional<std::string> get_value_string(const char *key) const noexcept;

        /**
         * Get the value by name
         * @param  key name of the value
         * @return     boolean value of the key or nullopt if not set
         */
        std::optional<bool> get_value_bool(const char *key) const noexcept;

        /**
         * Get the value by name
         * @param  key name of the value
         * @return     boolean value of the key or nullopt if not set
         */
        std::optional<double> get_value_float(const char *key) const noexcept;

        /**
         * Get the value by name
         * @param  key name of the value
         * @return     boolean value of the key or nullopt if not set
         */
        std::optional<long> get_value_long(const char *key) const noexcept;

        /**
         * Get the value by name
         * @param  key name of the value
         * @return     boolean value of the key or nullopt if not set
         */
        std::optional<unsigned long long> get_value_size(const char *key) const noexcept;

        /**
         * Set the value
         * @param key        name of the value
         * @param new_value  new value to set to
         */
        void set_value(const char *key, const char *new_value) noexcept;

        /**
         * Set the value
         * @param key        name of the value
         * @param new_value  new value to set to
         */
        void set_value(std::pair<std::string, std::string> key_value) noexcept;

        /**
         * Delete the value
         * @param key name of the value
         */
        void delete_value(const char *key) noexcept;

        /**
         * Initialize an Ini from a path
         * @param path path to initialize from
         */
        Ini(const char *path);

        /**
         * Initialize an Ini from a stream
         * @param data data to initialize from
         */
        Ini(std::istream &stream);

        /**
         * Initialize an empty Ini.
         */
        Ini() = default;

        /**
         * Copy an Ini
         * @param copy ini file to copy
         */
        Ini(const Ini &copy) = default;

        /**
         * Move an Ini
         * @param move ini file to move from
         */
        Ini(Ini &&move) = default;

    private:
        /** Values of the Ini */
        std::vector<std::pair<std::string, std::string>> p_values;

        /**
         * Load from the stream
         * @param stream stream to load from
         */
        void load_from_stream(std::istream &stream);
    };
}

#endif
