# SPDX-License-Identifier: GPL-3.0-only

# Source path
set(FMT_SRC lib/fmt)

add_library(fmt STATIC
    ${FMT_SRC}/format.cc
    ${FMT_SRC}/os.cc
)
