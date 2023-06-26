# SPDX-License-Identifier: GPL-3.0-only

add_executable(sss src/sound-subtitles-stapler/main.cpp)
set_target_properties(sss PROPERTIES LINK_FLAGS "-static -static-libgcc -static-libstdc++")
