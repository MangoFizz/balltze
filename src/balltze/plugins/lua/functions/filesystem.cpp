// SPDX-License-Identifier: GPL-3.0-only

#include <string>
#include <filesystem>
#include <lua.hpp>
#include "../../../logger.hpp"
#include "../../loader.hpp"
#include "../helpers.hpp"

namespace fs = std::filesystem;

namespace Balltze::Plugins {
    fs::path get_plugin_directory(lua_State *state) {
        auto *plugin = get_lua_plugin(state);
        if(!plugin) {
            throw std::runtime_error("Could not get plugin from Lua state");
        }
        return plugin->directory();
    }

    static bool check_path(lua_State *state, fs::path path) noexcept {
        auto data_directory = get_plugin_directory(state);
        auto absolute_path = fs::absolute(data_directory / path);
        if(absolute_path.string().find(data_directory.string()) == 0) {
            return true;
        }
        return false;
    }

    static int lua_create_directory(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 1) {
                const char *path = luaL_checkstring(state, 1);
                if(check_path(state, path)) {
                    lua_pushboolean(state, fs::create_directories(get_plugin_directory(state) / path));
                    return 1;
                }
                else {
                    return luaL_error(state, "Invalid path in function Balltze.filesystem.createDirectory.");
                }
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function Balltze.filesystem.createDirectory.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin in function Balltze.filesystem.createDirectory.");
        }
    }

    static int lua_remove_directory(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 1) {
                const char *path = luaL_checkstring(state, 1);
                if(check_path(state, path)) {
                    auto data_directory = get_plugin_directory(state);
                    if(fs::is_directory(data_directory / path)) {
                        lua_pushboolean(state, fs::remove_all(data_directory / path));
                    }
                    else {
                        lua_pushboolean(state, false);
                    }
                    return 1;
                }
                else {
                    return luaL_error(state, "Invalid path in function Balltze.filesystem.removeDirectory.");
                }
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function Balltze.filesystem.removeDirectory.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin in function Balltze.filesystem.removeDirectory.");
        }
    }

    static int lua_list_directory(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 1) {
                const char *path = luaL_checkstring(state, 1);
                if(check_path(state, path)) {
                    auto data_directory = get_plugin_directory(state);
                    if(fs::is_directory(data_directory / path)) {
                        lua_newtable(state);
                        std::size_t table_index = 1;
                        for(auto &entry : fs::directory_iterator(data_directory / path)) {
                            auto filename = entry.path().filename().string();
                            if(fs::is_directory(entry.path())) {
                                filename += "\\";
                            }
                            lua_pushstring(state, filename.c_str());
                            lua_rawseti(state, -2, table_index++);
                        }
                    }
                    else {
                        lua_pushboolean(state, false);
                    }
                    return 1;
                }
                else {
                    return luaL_error(state, "Invalid path in function Balltze.filesystem.listDirectory.");
                }
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function Balltze.filesystem.listDirectory.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin in function Balltze.filesystem.listDirectory.");
        }
    }

    static int lua_directory_exists(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 1) {
                const char *path = luaL_checkstring(state, 1);
                if(check_path(state, path)) {
                    auto data_directory = get_plugin_directory(state);
                    if(fs::exists(data_directory / path)) {
                        lua_pushboolean(state, fs::is_directory(data_directory / path));
                    }
                    else {
                        lua_pushboolean(state, false);
                    }
                    return 1;
                }
                else {
                    return luaL_error(state, "Invalid path in function Balltze.filesystem.directoryExists.");
                }
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function Balltze.filesystem.directoryExists.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin in function Balltze.filesystem.directoryExists.");
        }
    }

    static int lua_write_file(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 2 || args == 3) {
                const char *path = luaL_checkstring(state, 1);
                if(check_path(state, path)) {
                    std::string content = luaL_checkstring(state, 2);
                    bool append_content = (args == 3 && (lua_isboolean(state, 3) && lua_toboolean(state, 3)));

                    std::ofstream file;
                    file.open(get_plugin_directory(state) / path, (append_content ? std::ios::app : std::ios::trunc));
                    if(file.is_open()) {
                        file << content;
                        lua_pushboolean(state, file.good());
                        file.close();
                    }
                    else {
                        lua_pushboolean(state, false);
                    }
                    return 1;
                }
                else {
                    return luaL_error(state, "Invalid path in function Balltze.filesystem.writeFile.");
                }
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function Balltze.filesystem.writeFile.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin in function Balltze.filesystem.writeFile.");
        }
    }

    static int lua_read_file(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 1) {
                const char *path = luaL_checkstring(state, 1);
                if(check_path(state, path)) {
                    std::ifstream file;
                    file.open(get_plugin_directory(state) / path);
                    if(file.is_open()) {
                        std::stringstream file_content_stream;
                        std::string line_buffer;
                        while(file.good() && std::getline(file, line_buffer)) {
                            file_content_stream << line_buffer << std::endl;
                        }
                        auto file_content = file_content_stream.str();
                        if(!line_buffer.empty()) {
                            // Remove last newline character
                            file_content.pop_back();
                        }
                        lua_pushstring(state, file_content.c_str());
                        file.close();
                    }
                    else {
                        lua_pushnil(state);
                    }
                    return 1;
                }
                else {
                    return luaL_error(state, "Invalid path in function Balltze.filesystem.readFile.");
                }
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function Balltze.filesystem.readFile.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin in function Balltze.filesystem.readFile.");
        }
    }

    static int lua_delete_file(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 1) {
                const char *path = luaL_checkstring(state, 1);
                if(check_path(state, path)) {
                    auto data_directory = get_plugin_directory(state);
                    if(fs::is_regular_file(data_directory / path)) {
                        lua_pushboolean(state, fs::remove(data_directory / path));
                    }
                    else {
                        lua_pushboolean(state, false);
                    }
                    return 1;
                }
                else {
                    return luaL_error(state, "Invalid path in function Balltze.filesystem.deleteFile.");
                }
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function Balltze.filesystem.deleteFile.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin in function Balltze.filesystem.deleteFile.");
        }
    }

    static int lua_file_exists(lua_State *state) noexcept {
        auto *plugin = get_lua_plugin(state);
        if(plugin) {
            int args = lua_gettop(state);
            if(args == 1) {
                const char *path = luaL_checkstring(state, 1);
                if(check_path(state, path)) {
                    auto data_directory = get_plugin_directory(state);
                    if(fs::is_regular_file(data_directory / path)) {
                        lua_pushboolean(state, fs::exists(data_directory / path));
                    }
                    else {
                        lua_pushboolean(state, false);
                    }
                    return 1;
                }
                else {
                    return luaL_error(state, "Invalid path in function Balltze.filesystem.fileExists.");
                }
            }
            else {
                return luaL_error(state, "Invalid number of arguments in function Balltze.filesystem.fileExists.");
            }
        }
        else {
            logger.warning("Could not get plugin for lua state.");
            return luaL_error(state, "Unknown plugin in function Balltze.filesystem.fileExists.");
        }
    }

    static const luaL_Reg filesystem_functions[] = {
        {"createDirectory", lua_create_directory},
        {"removeDirectory", lua_remove_directory},
        {"listDirectory", lua_list_directory},
        {"directoryExists", lua_directory_exists},
        {"writeFile", lua_write_file},
        {"readFile", lua_read_file},
        {"deleteFile", lua_delete_file},
        {"fileExists", lua_file_exists},
        {nullptr, nullptr}
    };

    void lua_set_filesystem_table(lua_State *state) noexcept {
        lua_create_functions_table(state, "features", filesystem_functions);
    }
}
