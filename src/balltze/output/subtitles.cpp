// SPDX-License-Identifier: GPL-3.0-only

#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <deque>
#include <variant>
#include <chrono>
#include <balltze/math.hpp>
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

    static Math::QuadraticBezier curve;
    static auto fade_in_duration_ms = std::chrono::milliseconds(120);
    static auto fade_out_duration_ms = std::chrono::milliseconds(250);

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
            return (duration + fade_in_duration_ms + fade_out_duration_ms) - time_elapsed();
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
        }
    };

    static std::deque<Subtitle> subtitles;
    static std::queue<Subtitle> subtitles_queue;

    void add_subtitle(std::string text, Engine::ColorARGB color, std::chrono::milliseconds duration) {
        subtitles_queue.emplace(text, color, duration);
    }

    void add_subtitle(std::wstring text, Engine::ColorARGB color, std::chrono::milliseconds duration) {
        subtitles_queue.emplace(text, color, duration);
    }

    static void draw_subtitles(Event::FrameEvent const &) {
        if(subtitles.empty() && subtitles_queue.empty()) {
            return;
        }

        std::size_t subtitles_offset = subtitle_y_base_offset;
        bool is_fading_in = false;
        std::size_t slide_offset = 0;
        float fade_in_alpha = 1.0f;
        auto it = subtitles.begin();
        
        if(!subtitles_queue.empty() && (subtitles.empty() || it->time_elapsed() > fade_in_duration_ms)) {
            it = subtitles.insert(it, subtitles_queue.front());
            subtitles_queue.pop();
            it->start_time = std::chrono::steady_clock::now();
        }

        if(it->time_elapsed() < fade_in_duration_ms) {
            is_fading_in = true;
            auto fade_in_progress = static_cast<float>(it->time_elapsed().count()) / fade_in_duration_ms.count();
            fade_in_alpha -= curve.get_point(fade_in_progress).y;
            
            // Slide only when there's more than one subtitle
            if(subtitles.size() > 1) {
                slide_offset = std::round(curve.get_point(fade_in_progress, true).y * ((font_height + subtitle_margin) * 0.5f));
                subtitles_offset += slide_offset;
            }
        }

        while(it != subtitles.end()) {
            auto &subtitle = *it;
            if(subtitle.time_remaining() > std::chrono::milliseconds(0)) {
                auto color = subtitle.color;
                if(it == subtitles.begin() && is_fading_in) {
                    color.alpha -= fade_in_alpha;
                }
                else if(subtitle.time_remaining() <= fade_out_duration_ms) {
                    auto fade_out_elapsed = fade_out_duration_ms - subtitle.time_remaining();
                    auto fade_out_progress = static_cast<float>(fade_out_elapsed.count()) / fade_out_duration_ms.count();
                    color.alpha -= curve.get_point(fade_out_progress).y; 
                }

                if(color.alpha < 0.0f) {
                    color.alpha = 0.0f;
                }
                else if(color.alpha > 1.0f) {
                    color.alpha = 1.0f;
                }

                auto lines_height = subtitle.lines_height();
                apply_text(subtitle.text, 0, subtitles_offset - lines_height, screen_width, lines_height, color, subtitle_font, FontAlignment::ALIGN_CENTER, TextAnchor::ANCHOR_TOP_LEFT, false);
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

        curve = Math::QuadraticBezier::ease_out();

        Event::FrameEvent::subscribe(draw_subtitles);
    }
}
