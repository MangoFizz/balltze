# SPDX-License-Identifier: GPL-3.0-only

cmake_minimum_required(VERSION 3.16)

# Set project info
project(balltze
    VERSION 1.0.0
    LANGUAGES ASM C CXX
)

# Add include path
include_directories(include/)

add_library(balltze MODULE
    src/balltze/engine/window.cpp
    src/balltze/memory/codefinder.cpp
    src/balltze/memory/memory.cpp
    src/balltze/memory/signature.cpp
    src/balltze/output/message_box.cpp
    src/balltze/balltze.cpp
    src/balltze/main.cpp
)

# Set output name
set_target_properties(balltze PROPERTIES PREFIX "")
set_target_properties(balltze PROPERTIES OUTPUT_NAME "balltze")
set_target_properties(balltze PROPERTIES LINK_FLAGS "-static -static-libgcc -static-libstdc++")

# Install Balltze
install(TARGETS balltze DESTINATION "${CMAKE_INSTALL_PREFIX}")
