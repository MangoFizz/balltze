// SPDX-License-Identifier: GPL-3.0-only

#include <windows.h>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <utility>
#include <optional>
#include <filesystem>
#include <algorithm>
#include <uchar.h>
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

std::optional<std::fstream> open_file(std::filesystem::path path) {  
    std::fstream file(path, std::ios::in | std::ios::out | std::ios::binary);
    if(!file.is_open()) {
        return std::nullopt;
    }
    return file;
}

void append_subtitles_to_tag(std::filesystem::path tag_file, std::wstring subtitle_data) {
    auto result = open_file(tag_file);
    if(!result) {
        std::cout << "Notice: Failed to open file " << tag_file.string() << std::endl;
        return;
    }

    auto subtitle_data_size = (subtitle_data.length() + 1) * 2;
    auto &file = result.value();
    auto *tag = read_struct_from_file<Balltze::HEK::TagDefinitions::Sound>(file, subtitle_data_size);
    if(tag->data.pitch_ranges.count > 0) {
        if(tag->data.pitch_ranges.count > 1) {
            std::cout << "Warning: More than one pitch range found, only appending subtitles to the first permutation." << std::endl;
        }

        auto *pitch_range = tag->data.pitch_ranges.element();
        if(pitch_range->permutations.count > 0) {
            if(pitch_range->permutations.count > 1) {
                std::cout << "Warning: More than one permutation found, only appending subtitles to the first one." << std::endl;
            }
        
            auto *permutations = pitch_range[0].permutations.element();
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
            VirtualFree(tag, 0, MEM_RELEASE);
            file.flush();

            std::cout << "Info: Subtitle data appended." << std::endl;

            return;
        }
    } 
    std::cout << "Info: No subtitle data appended." << std::endl;
}

std::vector<std::pair<std::wstring, std::wstring>> parse_subtitles_file(std::string path) {
    std::fstream subtitles_file(path, std::ios::in);
    if(!subtitles_file.is_open()) {
        std::cout << "Error: Failed to open subtitles file " << path << std::endl;
        exit(1);
    }

    // Read the file into a vector.
    std::vector<char16_t> subtitles_data;
    auto filesize = get_file_size(subtitles_file);
    subtitles_data.resize(filesize);
    subtitles_file.seekg(2, std::ios::beg); // Skip the BOM
    subtitles_file.read(reinterpret_cast<char *>(subtitles_data.data()), subtitles_data.size());

    // Split the subtitles into lines
    std::wstring line;
    std::wstringstream subtitles_stream(std::wstring(subtitles_data.begin(), subtitles_data.end()));
    std::vector<std::wstring> lines;
    while(getline(subtitles_stream, line)) {
        if(line.length() > 0) {
            line.erase(0, line.find_first_not_of(L" \t\r\n"));
            lines.push_back(line);
        }
    }

    std::cout << "Info: Found " << lines.size() << " subtitles lines." << std::endl;

    std::vector<std::pair<std::wstring, std::wstring>> subtitles;
    for(std::size_t line_number = 0; line_number < lines.size(); line_number++) {
        auto &line = lines[line_number];
        std::wstring tag_path;
        std::wstring subtitle;
        if(line[0] == L'"') {
            auto first_quote = line.find(L'"');
            auto second_quote = line.find(L'"', first_quote + 1);
            if(first_quote == std::wstring::npos || second_quote == std::wstring::npos) {
                std::wcout << "Error: Invalid subtitles file (1). Line " << line_number << ". (" << line << ")" << std::endl;
                exit(1);
            }
            if(first_quote == second_quote - 1) {
                continue;
            }
            tag_path = line.substr(first_quote + 1, second_quote - first_quote - 1);

            auto third_quote = line.find(L'"', second_quote + 1);
            auto fourth_quote = line.find(L'"', third_quote + 1);
            if(third_quote == std::wstring::npos || fourth_quote == std::wstring::npos) {
                std::wcout << "Error: Invalid subtitles file (2). Line " << line_number << ". (" << line << ")" << std::endl;
                exit(1);
            }
            if(third_quote == fourth_quote - 1) {
                std::cout << "Warning: Skipping empty subtitle (line " << line_number << ")." << std::endl;
                continue;
            }
            subtitle = line.substr(third_quote + 1, fourth_quote - third_quote - 1);
        }
        else {
            auto first_tab = line.find(L'\t');
            if(first_tab == std::wstring::npos) {
                std::wcout << "Error: Invalid subtitles file (3). Line " << line_number << ". (" << line << ")" << std::endl;
                exit(1);
            }
            tag_path = line.substr(0, first_tab);

            auto first_quote = line.find(L'"');
            auto second_quote = line.find(L'"', first_quote + 1);
            if(first_quote == std::wstring::npos || second_quote == std::wstring::npos) {
                std::wcout << "Error: Invalid subtitles file (4). Line " << line_number << ". (" << line << ")" << std::endl;
                exit(1);
            }
            if(first_quote == second_quote - 1) {
                std::cout << "Warning: Skipping empty subtitle (line " << line_number << ")." << std::endl;
                continue;
            }
            subtitle = line.substr(first_quote + 1, second_quote - first_quote - 1);
        }

        // replace every double backslash with a single backslash
        auto tag_path_it = tag_path.begin();
        while(tag_path_it != tag_path.end()) {
            if(*tag_path_it == L'\\' && tag_path_it + 1 != tag_path.end() && *(tag_path_it + 1) == L'\\') {
                tag_path_it = tag_path.erase(tag_path_it);
            }
            else {
                tag_path_it++;
            }
        }

        subtitles.push_back({tag_path, subtitle});
    }

    std::cout << "Notice: Parsed " << subtitles.size() << " subtitles." << std::endl;

    return subtitles;
}

int main(int argc, char* argv[]) {
    if(argc < 3) {
        std::cout << "Usage: " << argv[0] << " <subtitles-file> <tag-directory>" << std::endl;
        return 1;
    }

    if(!std::filesystem::exists(argv[1])) {
        std::cout << "Error: Subtitles file does not exist." << std::endl;
        return 1;
    }

    if(!std::filesystem::exists(argv[2]) || !std::filesystem::is_directory(argv[2])) {
        std::cout << "Error: Tag directory does not exist." << std::endl;
        return 1;
    }

    std::cout << "Info: Parsing subtitles file..." << std::endl;

    auto subtitles = parse_subtitles_file(argv[1]);
    auto tags_directory = std::filesystem::path(argv[2]);
    for(auto &[tag_path, subtitle] : subtitles) {
        auto tag_file = tags_directory / (tag_path + L".sound");
        if(std::filesystem::exists(tag_file)) {
            std::cout << "Info: Appending subtitle to " << tag_file << std::endl;
            append_subtitles_to_tag(tag_file, subtitle);
        }
        else {
            std::cout << "Warning: Tag file " << tag_file << " does not exist." << std::endl;
        }
    }

    return 0;
}
