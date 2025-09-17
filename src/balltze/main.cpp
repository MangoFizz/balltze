// Ported from monolith.c from Chimera

#include <cstring>
#include <windows.h>
#include <winbase.h>
#include <winver.h>
#include <shlwapi.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <filesystem>
#include "logger.hpp"
#include "balltze.hpp"

struct LoadedDLL {
    HMODULE module = NULL;
};

extern "C" {
    extern void instantiate_chimera();
    extern void destroy_chimera();
    extern int find_signatures();
    extern void print_signature_errors();
    extern int halo_type();
}

namespace Balltze {
    static LoadedDLL *loaded_dlls = nullptr;
    static int dll_count = 0;
    static int dll_capacity = 16;
    
    static void unload_dll_mods() {
        if(!loaded_dlls) {
            return;
        }
        for(int i = 0; i < dll_count; i++) {
            FreeLibrary(loaded_dlls[i].module);
        }
        VirtualFree(loaded_dlls, sizeof(*loaded_dlls) * dll_capacity, MEM_DECOMMIT | MEM_RELEASE);
        loaded_dlls = nullptr;
    }
    
    static void load_dll_mods() {
        dll_count = 0;
        loaded_dlls = static_cast<LoadedDLL*>(VirtualAlloc(nullptr, sizeof(*loaded_dlls) * dll_capacity, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
    
        // Now try to load everything in mods
        namespace fs = std::filesystem;
        const fs::path mods_dir("mods");
        if(fs::exists(mods_dir) && fs::is_directory(mods_dir)) {
            for(auto &entry : fs::directory_iterator(mods_dir)) {
                if(!entry.is_regular_file()) {
                    continue;
                }
                auto path = entry.path();
                auto ext = path.extension().string();
                // case-insensitive .dll check
                if(stricmp(".dll", ext.c_str()) != 0) {
                    continue;
                }
    
                std::string rel = (mods_dir / path.filename()).string();
                // Skip chimera and balltze
                if(stricmp(rel.c_str(), "mods\\chimera.dll") || stricmp(rel.c_str(), "mods\\balltze.dll")) {
                    continue;
                }
    
                logger.info("Loading {}...", rel);
                loaded_dlls[dll_count].module = LoadLibraryA(rel.c_str());
                if(loaded_dlls[dll_count].module == NULL) {
                    logger.error("Failed to load {}", rel);
                    continue;
                }
    
                if(++dll_count == dll_capacity) {
                    int new_dll_capacity = dll_capacity * 2;
                    LoadedDLL *new_loaded_dlls = static_cast<LoadedDLL*>(VirtualAlloc(nullptr, sizeof(*loaded_dlls) * new_dll_capacity, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
                    for(int i = 0; i < dll_count; i++) {
                        new_loaded_dlls[i].module = loaded_dlls[i].module;
                    }
                    VirtualFree(loaded_dlls, sizeof(*loaded_dlls) * dll_capacity, MEM_DECOMMIT | MEM_RELEASE);
                    loaded_dlls = new_loaded_dlls;
                    dll_capacity = new_dll_capacity;
                }
            }
        }
    }
}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved) {
    switch(reason) {
        case DLL_PROCESS_ATTACH: {
            // Get the exe path
            char exe_path[MAX_PATH] = {0};
            GetModuleFileNameA(NULL, exe_path, sizeof(exe_path));

            // Check the exe version
            DWORD handle;
            DWORD ver_info_size = GetFileVersionInfoSizeA(exe_path, &handle);
            const char *ve = "Version check error";
            if(ver_info_size == 0) {
                MessageBoxA(NULL, "Executable does not have valid version information.", ve, MB_OK | MB_ICONERROR);
                ExitProcess(137);
            }

            BYTE ver_info[ver_info_size];
            if(!GetFileVersionInfoA(exe_path, 0, ver_info_size, ver_info)) {
                MessageBoxA(NULL, "Could not get file version information from executable.", ve, MB_OK | MB_ICONERROR);
                ExitProcess(137);
            }

            LPVOID ver_buf = nullptr;
            UINT ver_buf_size = 0;
            if(!VerQueryValueA(ver_info, "\\StringFileInfo\\040904B0\\FileVersion", &ver_buf, &ver_buf_size)) {
                MessageBoxA(NULL, "Executable file version was not found.", ve, MB_OK | MB_ICONERROR);
                ExitProcess(137);
            }

            char *exe_version = static_cast<char*>(ver_buf);
            if(ver_buf_size != 14 || *(exe_version + 13) != '\0') {
                MessageBoxA(NULL, "Executable file version is not a valid halo version string.", ve, MB_OK | MB_ICONERROR);
                ExitProcess(137);
            }

            const char *demo_version = "01.00.00.0578";
            const char *full_version = "01.00.10.0621";
            if(strcmp(exe_version, full_version) != 0 && strcmp(exe_version, demo_version) != 0) {
                char message[256];
                memset(message, 0, sizeof(message));
                snprintf(message, sizeof(message), "Current game version is %s.\nOnly the following versions can be used with Chimera:\n\n%s (Halo Trial)\n%s (Retail Halo PC/Halo Custom Edition)", exe_version, demo_version, full_version);
                MessageBoxA(NULL, message, ve, MB_OK | MB_ICONERROR);
                ExitProcess(137);
            }

            // Get the current directory
            char cd_path[MAX_PATH] = {0};
            DWORD cd_len = GetCurrentDirectoryA(sizeof(cd_path), cd_path);

            bool in_cd;
            if(strncmp(cd_path, exe_path, cd_len) == 0) {
                in_cd = true;
                cd_len++;
                while(exe_path[cd_len] == '\\' && exe_path[cd_len] == '/') {
                    cd_len++;
                }
                for(const char *i = exe_path + cd_len; *i; i++) {
                    if(*i == '\\' || *i == '/') { 
                        in_cd = false; 
                        break; 
                    }
                }
            } 
            else {
                in_cd = false;
            }

            if(!in_cd) {
                ExitProcess(135);
            }

            static WSADATA wsa_data = {};
            WSAStartup(MAKEWORD(2,2), &wsa_data);

            SetProcessDEPPolicy(PROCESS_DEP_ENABLE);

            instantiate_chimera();
            if(find_signatures() == 0) {
                print_signature_errors();
                destroy_chimera();
                ExitProcess(133);
            }
        
            Balltze::initialize();
            Balltze::load_dll_mods();

            if(GetModuleHandleA("keystone.dll")) {
                ExitProcess(197);
            }
        
            break;
        } 

        case DLL_PROCESS_DETACH: {
            Balltze::unload_dll_mods();
            WSACleanup();
            break;
        }
    }

    return TRUE;
}
