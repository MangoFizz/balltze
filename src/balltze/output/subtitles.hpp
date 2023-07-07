// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__OUTPUT__SUBTITLES_HPP
#define BALLTZE__OUTPUT__SUBTITLES_HPP

#include <string>
#include <balltze/engine/data_types.hpp>

namespace Balltze {
    void set_up_subtitles();
    void add_subtitle(std::string text, Engine::ColorARGB color, std::chrono::milliseconds duration);
    void add_subtitle(std::wstring text, Engine::ColorARGB color, std::chrono::milliseconds duration);
}

#endif
