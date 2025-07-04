// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/legacy_api/events/map_load.hpp>
#include <balltze/utils.hpp>
#include "../logger.hpp"

namespace Balltze::Features {
    static void on_read_map_file_data(LegacyApi::Event::MapFileDataReadEvent &event) {
        if(event.time == LegacyApi::Event::EVENT_TIME_BEFORE && event.context.output_buffer == nullptr) {
            logger.fatal("Game ran out of memory: trying to read map data to a buffer pointing to null! (GlobalAlloc failed?)");
            show_error_box("%s", "The game ran out of memory.\nTry disabling maps in RAM or reducing the buffer size in the chimera.ini file.");
            std::exit(EXIT_FAILURE);
        }
    }

    void set_up_map_data_read_warden() noexcept {
        LegacyApi::Event::MapFileDataReadEvent::subscribe(on_read_map_file_data, LegacyApi::Event::EVENT_PRIORITY_HIGHEST);
    }
}
