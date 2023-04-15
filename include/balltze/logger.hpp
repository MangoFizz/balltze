// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__LOGGER_HPP
#define BALLTZE_API__LOGGER_HPP

#include <string>
#include <sstream>
#include <filesystem>
#include <mutex>
#include <optional>
#include <windows.h>
#include <fmt/color.h>
#include <fmt/format.h>
#include "api.hpp"
#include "utils.hpp"

namespace Balltze {
    /**
     * A logger class that can be used to log messages to the console, a file and the in-game console.
     * 
     * Based on the LiteLoader logger class <3
     * https://github.com/LiteLDev/SDK-CPP/blob/main/include/llapi/LoggerAPI.h
     */
    class BALLTZE_API Logger {
    public:
        enum LogLevel {
            LOG_LEVEL_DEBUG,
            LOG_LEVEL_INFO,
            LOG_LEVEL_WARNING,
            LOG_LEVEL_ERROR,
            LOG_LEVEL_FATAL
        };

        class LoggerStream {
            friend class Logger;
        private:
            LogLevel m_level;
            std::string m_prefix;
            std::string m_console_format;
            std::string m_file_format;
            std::string m_ingame_format;
            fmt::text_style m_console_style;
            std::ostringstream m_stream;
            Logger *m_logger;

            LoggerStream(Logger *logger, LogLevel level, std::string prefix, std::string console_format, std::string file_format, std::string ingame_format, fmt::text_style console_style) noexcept;
            LoggerStream() = default;

        public:
            template<typename T>
            LoggerStream &operator<<(T value) {
                m_stream << value;
                return *this;
            }

            LoggerStream &operator<<(void (*t)(LoggerStream&)) {
                t(*this);
                return *this;
            }

            template<typename ...Args>
            void operator()(std::string format, Args &&...args) {
                if constexpr (sizeof...(args) == 0) {
                    *this << format << endl;
                } 
                else {
                    std::string output = fmt::format(format, args...);
                    *this << output << endl;
                }
            }
        };

        LoggerStream debug;
        LoggerStream info;
        LoggerStream warning;
        LoggerStream error;
        LoggerStream fatal;

        std::string name() const noexcept;

        void lock();
        void try_lock();
        void unlock();

        Logger(std::string name = {}) noexcept;
        
        inline void set_file(std::optional<std::filesystem::path> file_path, bool append = true) {
            try {
                if(file_path) {
                    set_file_impl(get_current_module(), *file_path, append);
                }
                else {
                    m_file_path = std::nullopt;
                }
            }
            catch(...) {
                throw;
            }
        }
        
        inline static void endl(LoggerStream &ls) {
            try {
                endl_impl(get_current_module(), ls);
            }
            catch(...) {
                throw;
            }
        }
    
    private:
        std::string m_name;
        std::optional<std::filesystem::path> m_file_path;
        std::mutex m_mutex;

        void set_file_impl(HMODULE module, std::filesystem::path file_path, bool append);

        static void endl_impl(HMODULE module, LoggerStream &ls);
    };
}

#endif
