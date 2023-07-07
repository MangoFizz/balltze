# SPDX-License-Identifier: GPL-3.0-only

add_library(invader STATIC
    src/invader/sound/adpcm_xq/adpcm-lib.c
    src/invader/sound/sound_encoder.cpp
    src/invader/sound/sound_reader_16_bit_pcm_big_endian.cpp
    src/invader/sound/sound_reader_ogg.cpp
    src/invader/sound/sound_reader_xbox_adpcm.cpp
)

target_link_libraries(invader FLAC vorbisenc vorbisfile vorbis ogg)
