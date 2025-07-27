# SPDX-License-Identifier: GPL-3.0-only

cmake_minimum_required(VERSION 3.16)

include_directories(BEFORE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(balltze INTERFACE)
target_link_libraries(balltze INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/lib/libballtze.dll.a 
    ${CMAKE_CURRENT_LIST_DIR}/lib/libfmt.a 
)
