// SPDX-License-Identifier: GPL-3.0-only

#include <windows.h>
#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>
#include <balltze/hek/tag_definitions/sound.hpp>

using namespace Balltze;

template<typename T>
struct TagFile {
    HEK::TagFileHeader header;
    T data;
};

std::size_t get_file_size(std::fstream &file) {
    file.seekg(0, std::ios::end);
    std::size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    return size;
};

template<typename T>
TagFile<T> *read_struct_from_file(std::fstream &file, std::size_t extra_space = 0) {
    auto filesize = get_file_size(file);
    auto buff = VirtualAlloc(NULL, filesize + extra_space, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    file.read(reinterpret_cast<char *>(buff), filesize);
    return reinterpret_cast<TagFile<T> *>(buff);
}

template<typename T>
void write_struct_to_file(std::fstream &file, T *data, std::size_t data_size) {
    file.seekg(0, std::ios::beg);
    file.write(reinterpret_cast<char *>(data), data_size);
}

std::fstream open_file(std::string path) {  
    std::fstream file(path, std::ios::in | std::ios::out | std::ios::binary);
    if(!file.is_open()) {
        std::cout << "Failed to open file: " << path << std::endl;
        exit(1);
    }
    return file;
}

void append_subtitle_data(std::string tag_path, std::wstring subtitle_data) {
    auto subtitle_data_size = (subtitle_data.length() + 1) * 2;
    auto file = open_file(tag_path);
    auto *tag = read_struct_from_file<Balltze::HEK::TagDefinitions::Sound>(file, subtitle_data_size);
    if(tag->data.pitch_ranges.count > 0) {
        if(tag->data.pitch_ranges.count > 1) {
            std::cout << "Warning: More than one pitch range found, only appending subtitles to the first permutation." << std::endl;
        }

        auto *pitch_range = tag->data.pitch_ranges.data();
        if(pitch_range->permutations.count > 0) {
            if(pitch_range->permutations.count > 1) {
                std::cout << "Warning: More than one permutation found, only appending subtitles to the first one." << std::endl;
            }
        
            auto *permutations = pitch_range[0].permutations.data();
            auto filesize = get_file_size(file);

            if(permutations[0].subtitle_data.file_offset != 0) {
                std::cout << "Error: Sound tag already has subtitle data." << std::endl;
                return;
            }
            
            // Copy the subtitle data to the end of the tag buffer
            auto end_of_tag = reinterpret_cast<char *>(tag) + filesize;
            memcpy(end_of_tag, subtitle_data.c_str(), subtitle_data_size);
            
            // Update the permutation data
            permutations[0].subtitle_data.file_offset = filesize;
            permutations[0].subtitle_data.size = subtitle_data_size;

            // Save file
            filesize += subtitle_data_size;
            write_struct_to_file(file, tag, filesize);
            file.flush();

            return;
        }
    } 
    std::cout << "Error: No subtitle data appended." << std::endl;
}


int main(int argc, char* argv[]) {
    if(argc < 3) {
        std::cout << "Usage: " << argv[0] << " <tag file> <subtitle>" << std::endl;
        return 1;
    }

    std::wstring subtitle = L"";
    for(int i = 2; i < argc; i++) {
        subtitle += std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(argv[i]);
    }
    
    append_subtitle_data(argv[1], subtitle);

    return 0;
}
