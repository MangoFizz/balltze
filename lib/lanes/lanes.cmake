# SPDX-License-Identifier: GPL-3.0-only

include_directories(lib/lanes/src)

aux_source_directory(lib/lanes/src LANES_SRC)
add_library(lanes STATIC ${LANES_SRC})

# mute all warnings X_x
target_compile_options(lanes PUBLIC -w) 
