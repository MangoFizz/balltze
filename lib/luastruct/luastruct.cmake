# SPDX-License-Identifier: GPL-3.0-only

include_directories(lib/luastruct/include)

add_library(luastruct STATIC
    lib/luastruct/src/struct.c
    lib/luastruct/src/debug.c
    lib/luastruct/src/enum.c
    lib/luastruct/src/array.c
    lib/luastruct/src/object.c
)

target_include_directories(luastruct PRIVATE
    lib/luastruct/include/luastruct
)
