# SPDX-License-Identifier: GPL-3.0-only

add_library(invader STATIC
    lib/invader/crc/crc32.c
    lib/invader/sound/adpcm_xq/adpcm-lib.c
    lib/invader/sound/sound_encoder.cpp
    lib/invader/sound/sound_reader_16_bit_pcm_big_endian.cpp
    lib/invader/sound/sound_reader_ogg.cpp
    lib/invader/sound/sound_reader_xbox_adpcm.cpp
)

target_link_libraries(invader vorbisfile vorbis ogg)
