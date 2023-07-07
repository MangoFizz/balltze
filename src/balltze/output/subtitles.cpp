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
    static std::size_t subtitle_padding;
    static std::size_t subtitle_margin;
    static std::size_t subtitle_line_margin;
    static std::size_t subtitle_width;
    static std::size_t subtitle_x_offset;
    static std::size_t subtitle_y_base_offset;
    static std::size_t bottom_margin;
    static std::size_t font_height;

    static std::vector<std::string> split_string(std::string text) {
        std::string buffer(text);
        std::string line;
        std::vector<std::string> lines;
        line.reserve(buffer.size());

        while(!buffer.empty()) {
            while(!buffer.empty() && text_pixel_length(line.c_str(), subtitle_font) < (subtitle_width - subtitle_padding * 2)) {
                line += buffer.front();
                buffer.erase(buffer.begin());
            }
            lines.push_back(line);
            line.clear();
        }

        return lines;
    }

    static std::vector<std::wstring> split_string(std::wstring text) {
        std::wstring buffer(text);
        std::wstring line;
        std::vector<std::wstring> lines;
        line.reserve(buffer.size());

        while(!buffer.empty()) {
            while(!buffer.empty() && text_pixel_length(line.c_str(), subtitle_font) < (subtitle_width - subtitle_padding * 2)) {
                line += buffer.front();
                buffer.erase(buffer.begin());
            }
            lines.push_back(line);
            line.clear();
        }

        return lines;
    }

    struct Subtitle {
        std::variant<std::vector<std::string>, std::vector<std::wstring>> lines;
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
            if(std::holds_alternative<std::vector<std::string>>(lines)) {
                auto lines_count = std::get<std::vector<std::string>>(lines).size();
                auto total_padding = subtitle_line_margin * (lines_count - 1);
                return lines_count * font_pixel_height(subtitle_font) + total_padding;
            } 
            else {
                auto lines_count = std::get<std::vector<std::wstring>>(lines).size();
                auto total_padding = subtitle_line_margin * (lines_count - 1);
                return lines_count * font_pixel_height(subtitle_font) + total_padding;
            }
        }

        Subtitle(std::string text, Engine::ColorARGB color, std::chrono::milliseconds duration) {
            lines = split_string(text);
            this->color = color;
            this->duration = duration;
            start_time = std::chrono::steady_clock::now();
        }

        Subtitle(std::wstring text, Engine::ColorARGB color, std::chrono::milliseconds duration) {
            lines = split_string(text);
            this->color = color;
            this->duration = duration;
            start_time = std::chrono::steady_clock::now();
        }
    };

    std::deque<Subtitle> subtitles;

    void add_subtitle(std::string text, Engine::ColorARGB color, std::chrono::milliseconds duration) {
        subtitles.push_front(Subtitle(text, color, duration));
    }

    void add_subtitle(std::wstring text, Engine::ColorARGB color, std::chrono::milliseconds duration) {
        subtitles.push_front(Subtitle(text, color, duration));
    }

    static void draw_subtitles(Event::FrameEvent const &) {
        std::size_t subtitles_offset = subtitle_y_base_offset;
        auto it = subtitles.begin();
        while(it != subtitles.end()) {
            auto &subtitle = *it;
            if(subtitle.time_elapsed() < subtitle.duration) {
                if(std::holds_alternative<std::vector<std::string>>(subtitle.lines)) {
                    auto lines = std::get<std::vector<std::string>>(subtitle.lines);
                    std::size_t line_offset = subtitles_offset;
                    for(size_t i = lines.size() - 1; i <= 0; i--) {
                        auto line = lines[i];
                        apply_text(line, subtitle_x_offset, line_offset - font_height, subtitle_width, font_height, subtitle.color, subtitle_font, FontAlignment::ALIGN_CENTER, TextAnchor::ANCHOR_TOP_LEFT, false);
                        line_offset -= font_height + subtitle_line_margin; 
                    }
                } 
                else {
                    auto lines = std::get<std::vector<std::wstring>>(subtitle.lines);
                    std::size_t line_offset = subtitles_offset;
                    for(size_t i = lines.size() - 1; i <= 0; i--) {
                        auto line = lines[i];
                        apply_text(line, subtitle_x_offset, line_offset - font_height, subtitle_width, font_height, subtitle.color, subtitle_font, FontAlignment::ALIGN_CENTER, TextAnchor::ANCHOR_TOP_LEFT, false);
                        line_offset -= font_height + subtitle_line_margin; 
                    }
                }
                subtitles_offset -= subtitle.lines_height() + subtitle_margin;
                it++;
            }
            else {
                it = subtitles.erase(it);
            }
        }
    }

    void set_up_subtitles() {
        subtitle_font = GenericFont::FONT_SMALL;
        subtitle_padding = 5;
        subtitle_margin = 7;
        subtitle_line_margin = 5;
        bottom_margin = 65;
        
        auto res = Engine::get_resolution();
        float screen_height = 480;
        float screen_width = static_cast<float>(res.width) / res.height * screen_height;
        float width = screen_width * 0.9;
        subtitle_width = std::round(width);
        subtitle_x_offset = std::round((screen_width - width) / 2);
        subtitle_y_base_offset = screen_height - bottom_margin;

        font_height = font_pixel_height(subtitle_font);

        logger.debug("res.width: {}", res.width);
        logger.debug("res.height: {}", res.height);
        logger.debug("screen_height: {}", screen_height);
        logger.debug("screen_width: {}", screen_width);
        logger.debug("subtitle_width: {}", subtitle_width);
        logger.debug("subtitle_x_offset: {}", subtitle_x_offset);
        logger.debug("subtitle_y_base_offset: {}", subtitle_y_base_offset);


        Event::FrameEvent::subscribe(draw_subtitles);
    }
}
