// SPDX-License-Identifier: GPL-3.0-only

#include <cstdint>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <lua.hpp>
#include "../libraries/lua_memory_snapshot.hpp"

namespace Balltze::Lua {
    static std::ofstream snapshot_file;
    
    static std::string escape_csv_field(const char *str) {
        if(!str) {
            return "(null)";
        }
    
        std::string input(str);
        bool needs_quotes = input.find_first_of(",\"\n\r") != std::string::npos;
    
        if(!needs_quotes) {
            return input;
        }
    
        std::ostringstream escaped;
        escaped << '"';
        for(char ch : input) {
            if(ch == '"') {
                escaped << "\"\"";
            } 
            else {
                escaped << ch;
            }
        }
        escaped << '"';
        return escaped.str();
    }
    
    static void snapshot_open(const std::string &filename) {
        snapshot_file.open(filename, std::ios::out | std::ios::trunc);
        if(!snapshot_file.is_open()) {
            throw std::runtime_error("Failed to open snapshot file");
        }
        snapshot_file << "ptr,type,size,name,desc,value,parent\n";
    }
    
    static void snapshot_close() {
        if(snapshot_file.is_open()) {
            snapshot_file.close();
        }
    }
    
    static void snapshot_callback(const void *obj, int type, size_t size, const char *name, const char *desc, const char *value, const void *parent) {
        if(!snapshot_file.is_open()) {
            return;
        }
    
        const char *type_str = nullptr;
        switch(type) {
            case -1:
                type_str = "none"; 
                break;
            case 4:
                type_str = "string"; 
                break;
            case 5:
                type_str = "table"; 
                break;
            case 6:
                type_str = "function"; 
                break;
            case 7:
                type_str = "userdata"; 
                break;
            case 8:
                type_str = "proto"; 
                break;
            default:
                type_str = "unknown"; 
                break;
        }
    
        snapshot_file << std::hex << reinterpret_cast<std::uintptr_t>(obj) << ",";
        snapshot_file << type_str << ",";
        snapshot_file << std::dec << size << ",";
        snapshot_file << escape_csv_field(name) << ",";
        snapshot_file << escape_csv_field(desc) << ",";
        snapshot_file << escape_csv_field(value) << ",";
        snapshot_file << std::hex << reinterpret_cast<std::uintptr_t>(parent) << "\n";
    }

    void capture_lua_state_snapshot(lua_State *state, const std::string &output_file) {
        snapshot_open(output_file.c_str());
        lua_memory_snapshot(state, snapshot_callback);
        snapshot_close();
    }
}
