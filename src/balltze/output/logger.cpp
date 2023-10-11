// SPDX-License-Identifier: GPL-3.0-only

#include <windows.h>
#include <balltze/logger.hpp>
#include <balltze/engine/core.hpp>
#include <fmt/chrono.h>
#include <fmt/core.h>
#include <fmt/os.h>
#include <fmt/printf.h>
#include "../plugins/loader.hpp"

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

    static Engine::ColorARGB color_for_log_level(Logger::LogLevel level) noexcept {
        switch(level) {
            case Logger::LOG_LEVEL_DEBUG:
                return {0.65, 1.0, 1.0, 1.0};
            case Logger::LOG_LEVEL_INFO:
                return {1.0, 1.0, 1.0, 1.0};
            case Logger::LOG_LEVEL_WARNING:
                return {1.0, 1.0, 1.0, 0.125};
            case Logger::LOG_LEVEL_ERROR:
                return {1.0, 1.0, 0.25, 0.25};
            case Logger::LOG_LEVEL_FATAL:
                return {1.0, 1.0, 0.35, 0.35};
            default:
                return {1.0, 1.0, 1.0, 1.0};
        }
    }

    Logger::LoggerStream::LoggerStream(Logger *logger, LogLevel level, std::string prefix, std::string console_format, std::string file_format, std::string ingame_format, fmt::text_style console_style) noexcept
        : m_logger(logger), m_level(level), m_prefix(prefix), m_console_format(console_format), m_file_format(file_format), m_ingame_format(ingame_format), m_console_style(console_style) {
    }

    std::string Logger::name() const noexcept {
        return m_name;
    }

    void Logger::mute_debug(bool setting) noexcept {
        m_mute_debug = setting;
    }

    bool Logger::mute_debug() const noexcept {
        return m_mute_debug;
    }

    void Logger::mute_ingame(bool setting) noexcept {
        m_mute_ingame = setting;
    }

    bool Logger::mute_ingame() const noexcept {
        return m_mute_ingame;
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
            name = LoggerStream(this, Logger::level, name_for_log_level(Logger::level), "\r{} {} [{}] {}\n", "{} {} [{}] {}\n", "[{}] {}\n", style_for_log_level(Logger::level))

        CREATE_LOGGER_STREAM(debug, LOG_LEVEL_DEBUG);
        CREATE_LOGGER_STREAM(info, LOG_LEVEL_INFO);
        CREATE_LOGGER_STREAM(warning, LOG_LEVEL_WARNING);
        CREATE_LOGGER_STREAM(error, LOG_LEVEL_ERROR);
        CREATE_LOGGER_STREAM(fatal, LOG_LEVEL_FATAL);

        #undef CREATE_LOGGER_STREAM
    }

    void Logger::set_file_impl(HMODULE module, std::filesystem::path file_path, bool append) {
        auto balltze_module = get_current_module();
        if(module == balltze_module) {
            m_file_path = file_path;
        }
        else {
            auto *plugin = Plugins::get_dll_plugin(module);
            if(plugin) {
                if(plugin->path_is_valid(file_path)) {
                    try {
                        plugin->init_data_directory();
                    }
                    catch(std::exception &e) {
                        throw;
                    }
                    m_file_path = file_path;
                }
                else {
                    throw std::runtime_error("Invalid file path");
                }
            }
            else {
                m_file_path = file_path;
            }
        }
        m_append = append;

        m_file.open(*m_file_path, std::ios::out | (m_append ? std::ios::app : std::ios::trunc));
        if(!m_file.is_open()) {
            throw std::runtime_error("Failed to create/open file");
        }
    }

    static inline bool current_exe_is_cui() noexcept {
        std::byte *base = reinterpret_cast<std::byte *>(GetModuleHandle(NULL));
        auto *dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(base);
        auto *header = reinterpret_cast<PIMAGE_NT_HEADERS>(base + dos_header->e_lfanew);
        return header->OptionalHeader.Subsystem == IMAGE_SUBSYSTEM_WINDOWS_CUI;
    }

    void Logger::endl_impl(HMODULE module, LoggerStream &stream) {
        if(!(stream.m_level == LOG_LEVEL_DEBUG && stream.m_logger->m_mute_debug)) {
            static bool is_cui = current_exe_is_cui();
            if(is_cui) {
                // For some reason, it crash the game when the subsystem is GUI
                print_console(stream);
            }
            print_file(stream);
            print_ingame(stream);
        }

        stream.m_stream.str("");
        stream.m_stream.clear();
    }

    void Logger::print_console(LoggerStream &stream) {
        auto name = stream.m_logger->m_name;
        auto content = stream.m_stream.str();
        constexpr const char *time_format = "{:%H:%M:%S}";
        auto time = fmt::format(time_format, fmt::localtime(std::time(nullptr)));

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
            fmt::print(fmt::runtime(stream.m_console_format), output_time, output_level, output_prefix, output_content);
        }
        else {
            fmt::print("\n");
        }
    }

    void Logger::print_file(LoggerStream &stream) {
        auto *logger = stream.m_logger;

        if(logger->m_file.is_open()) {
            auto name = logger->m_name;
            auto content = stream.m_stream.str();
            auto time = fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(std::time(nullptr)));

            if(!content.empty()) {
                logger->m_file << fmt::format(fmt::runtime(stream.m_file_format), time, stream.m_prefix, name, content);
                logger->m_file.flush();
            }
        }
    }

    void Logger::print_ingame(LoggerStream &stream) {
        if(stream.m_logger->m_mute_ingame) {
            return;
        }

        auto name = stream.m_logger->m_name;
        auto content = stream.m_stream.str();

        if(!content.empty()) {
            auto color = color_for_log_level(stream.m_level);
            auto text = fmt::format(fmt::runtime(stream.m_ingame_format), name, content);
            Engine::console_print(text, color);
        }
    }
}
