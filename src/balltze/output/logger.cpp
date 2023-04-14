// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/logger.hpp>
#include <fmt/chrono.h>
#include <fmt/core.h>
#include <fmt/os.h>
#include <fmt/printf.h>

namespace Balltze {
    static std::string name_for_log_level(Logger::LogLevel level) noexcept {
        switch(level) {
            case Logger::LOG_LEVEL_DEBUG:
                return "DEBUG";
            case Logger::LOG_LEVEL_INFO:
                return "INFO";
            case Logger::LOG_LEVEL_WARNING:
                return "WARN";
            case Logger::LOG_LEVEL_ERROR:
                return "ERROR";
            case Logger::LOG_LEVEL_FATAL:
                return "FATAL";
            default:
                return "UNKNOWN";
        }
    }

    static fmt::text_style style_for_log_level(Logger::LogLevel level) noexcept {
        switch(level) {
            case Logger::LOG_LEVEL_DEBUG:
                return fmt::fg(fmt::color::light_gray);
            case Logger::LOG_LEVEL_INFO:
                return fmt::fg(fmt::color::light_sea_green);
            case Logger::LOG_LEVEL_WARNING:
                return fmt::fg(fmt::color::yellow);
            case Logger::LOG_LEVEL_ERROR:
                return fmt::fg(fmt::color::red);
            case Logger::LOG_LEVEL_FATAL:
                return fmt::fg(fmt::color::red) | fmt::emphasis::bold;
            default:
                return fmt::fg(fmt::color::white);
        }
    }

    Logger::LoggerStream::LoggerStream(Logger *logger, LogLevel level, std::string prefix, std::string console_format, std::string file_format, std::string ingame_format, fmt::text_style console_style) noexcept
        : m_logger(logger), m_level(level), m_prefix(prefix), m_console_format(console_format), m_file_format(file_format), m_ingame_format(ingame_format), m_console_style(console_style) {
    }

    void Logger::lock() {
        m_mutex.lock();
    }

    void Logger::try_lock() {
        m_mutex.try_lock();
    }

    void Logger::unlock() {
        m_mutex.unlock();
    }

    Logger::Logger(std::string name) noexcept : m_name(name) {
        #define CREATE_LOGGER_STREAM(name, level) \
            name = LoggerStream(this, Logger::level, name_for_log_level(Logger::level), "{} {} {} {}\n", "[{:%Y-%m-%d %H:%M:%S} {}]{} {}\n", "[{}{}]{} {}\n", style_for_log_level(Logger::level))

        CREATE_LOGGER_STREAM(debug, LOG_LEVEL_DEBUG);
        CREATE_LOGGER_STREAM(info, LOG_LEVEL_INFO);
        CREATE_LOGGER_STREAM(warning, LOG_LEVEL_WARNING);
        CREATE_LOGGER_STREAM(error, LOG_LEVEL_ERROR);
        CREATE_LOGGER_STREAM(fatal, LOG_LEVEL_FATAL);

        #undef CREATE_LOGGER_STREAM
    }

    void Logger::set_file_impl(HMODULE module, std::string file_path, bool append) {
        
    }

    void Logger::endl_impl(HMODULE module, LoggerStream &stream) {
        auto name = stream.m_logger->m_name;
        auto content = stream.m_stream.str();
        auto time = fmt::format("{:%H:%M:%S}", fmt::localtime(std::time(nullptr)));
        
        if(!name.empty()) {
            name = "[" + name + "]";
        }

        auto apply_format = [&](fmt::text_style style, std::string text) {
            fmt::basic_memory_buffer<char> buffer;
            auto format = fmt::detail::to_string_view(text.c_str());
            bool formatted = false;

            if(style.has_emphasis()) {
                auto emphasis = fmt::detail::make_emphasis<char>(style.get_emphasis());
                buffer.append(emphasis.begin(), emphasis.end());
                formatted = true;
            }
            if(style.has_foreground()) {
                auto foreground = fmt::detail::make_foreground_color<char>(style.get_foreground());
                buffer.append(foreground.begin(), foreground.end());
                formatted = true;
            }
            if(style.has_background()) {
                auto background = fmt::detail::make_background_color<char>(style.get_background());
                buffer.append(background.begin(), background.end());
                formatted = true;
            }
            buffer.append(format.begin(), format.end());
            if(formatted) {
                fmt::detail::reset_color<char>(buffer);
            }
            return fmt::to_string(buffer);
        };

        if(!content.empty()) {
            auto output_time = apply_format(fg(fmt::color::light_blue), time);
            auto output_level = apply_format(stream.m_console_style, stream.m_prefix);
            auto output_prefix = apply_format(fg(fmt::color::white), name);
            auto output_content = apply_format(fg(fmt::color::white), content);
            fmt::print(stream.m_console_format, output_time, output_level, output_prefix, output_content);
        }
        else {
            fmt::print("\n");
        }

        stream.m_stream.str("");
        stream.m_stream.clear();
    }
}
