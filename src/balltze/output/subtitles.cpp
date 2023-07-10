// SPDX-License-Identifier: GPL-3.0-only

#include <string>
#include <sstream>
#include <vector>
#include <deque>
#include <variant>
#include <chrono>
#include <balltze/engine/core.hpp>
#include <balltze/events/frame.hpp>
#include "../output/draw_text.hpp"
#include "../logger.hpp"

namespace Balltze {
    static std::variant<Engine::TagHandle, GenericFont> subtitle_font;
    static std::size_t screen_width;
    static std::size_t subtitle_margin;
    static std::size_t subtitle_line_margin;
    static std::size_t subtitle_width;
    static std::size_t subtitle_y_base_offset;
    static std::size_t bottom_margin;
    static std::size_t font_height;

    static std::vector<std::string> split_string(std::string text) {
        std::string buffer(text);
        std::string line;
        std::string word;
        std::vector<std::string> lines;
        line.reserve(buffer.size());

        for(auto c : text) {
            if (c == ' ') {
                if(text_pixel_length(line.c_str(), subtitle_font) < subtitle_width) {
                    if(!line.empty()) {
                        line += ' ';
                    }
                    line += word;
                    word.clear();
                } 
                else {
                    lines.push_back(line);
                    line = word;
                    word.clear();
                }
            } 
            else {
                word += c;
            }
        }

        if(!word.empty()) {
            if(!line.empty()) {
                line += ' ';
            }
            line += word;
        }

        if(!line.empty()) {
            lines.push_back(line);
        }

        return std::move(lines);
    }

    static std::vector<std::wstring> split_string(std::wstring text) {
        std::wstring buffer(text);
        std::wstring line;
        std::wstring word;
        std::vector<std::wstring> lines;
        line.reserve(buffer.size());

        for(auto c : text) {
            if (c == L' ') {
                if(text_pixel_length(line.c_str(), subtitle_font) < subtitle_width) {
                    if(!line.empty()) {
                        line += L' ';
                    }
                    line += word;
                    word.clear();
                } 
                else {
                    lines.push_back(line);
                    line = word;
                    word.clear();
                }
            } 
            else {
                word += c;
            }
        }

        if(!word.empty()) {
            if(!line.empty()) {
                line += L' ';
            }
            line += word;
        }

        if(!line.empty()) {
            lines.push_back(line);
        }

        return std::move(lines);
    }

    struct Subtitle {
        std::variant<std::string, std::wstring> text;
        std::size_t lines_count;
        std::chrono::steady_clock::time_point start_time;
        std::chrono::milliseconds duration;
        Engine::ColorARGB color;

        std::chrono::milliseconds time_elapsed() {
            return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time);
        }

        std::chrono::milliseconds time_remaining() {
            return duration - time_elapsed();
        }

        std::size_t lines_height() {
            return lines_count * font_pixel_height(subtitle_font);
        }

        Subtitle(std::string text, Engine::ColorARGB color, std::chrono::milliseconds duration) {
            auto lines = split_string(text);
            this->lines_count = lines.size();
            std::stringstream ss;
            for(auto line : lines) {
                ss << line << "|n";
            }
            this->text = ss.str();
            this->color = color;
            this->duration = duration;
            start_time = std::chrono::steady_clock::now();
        }

        Subtitle(std::wstring text, Engine::ColorARGB color, std::chrono::milliseconds duration) {
            auto lines = split_string(text);
            this->lines_count = lines.size();
            std::wstringstream wss;
            for(auto line : lines) {
                wss << line << L"|n";
            }
            this->text = wss.str();
            this->color = color;
            this->duration = duration;
            start_time = std::chrono::steady_clock::now();
        }
    };

    std::deque<Subtitle> subtitles;

    void add_subtitle(std::string text, Engine::ColorARGB color, std::chrono::milliseconds duration) {
        subtitles.emplace_front(text, color, duration);
    }

    void add_subtitle(std::wstring text, Engine::ColorARGB color, std::chrono::milliseconds duration) {
        subtitles.emplace_front(text, color, duration);
    }

    static void draw_subtitles(Event::FrameEvent const &) {
        std::size_t subtitles_offset = subtitle_y_base_offset;
        auto it = subtitles.begin();
        while(it != subtitles.end()) {
            auto &subtitle = *it;
            if(subtitle.time_elapsed() < subtitle.duration) {
                auto lines_height = subtitle.lines_height();
                apply_text(subtitle.text, 0, subtitles_offset - lines_height, screen_width, lines_height, subtitle.color, subtitle_font, FontAlignment::ALIGN_CENTER, TextAnchor::ANCHOR_TOP_LEFT, false);
                subtitles_offset -= lines_height + subtitle_margin;
                it++;
            }
            else {
                it = subtitles.erase(it);
            }
        }
    }

    void set_up_subtitles() {
        subtitle_font = GenericFont::FONT_SMALL;
        subtitle_margin = 7;
        subtitle_line_margin = 5;
        bottom_margin = 65;
        
        auto res = Engine::get_resolution();
        float screen_height = 480;
        float width = static_cast<float>(res.width) / res.height * screen_height;
        screen_width = std::round(width);
        subtitle_width = std::round(width * 0.8);
        subtitle_y_base_offset = screen_height - bottom_margin;
        font_height = font_pixel_height(subtitle_font);

        Event::FrameEvent::subscribe(draw_subtitles);
    }
}
