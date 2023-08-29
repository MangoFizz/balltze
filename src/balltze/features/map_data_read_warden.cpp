// SPDX-License-Identifier: GPL-3.0-only

#include <balltze/events/map_file_data_read.hpp>
#include <balltze/utils.hpp>
#include "../logger.hpp"

namespace Balltze::Features {
    static void on_read_map_file_data(Event::MapFileDataReadEvent &event) {
        if(event.time == Event::EVENT_TIME_BEFORE && event.args.output_buffer == nullptr) {
            logger.fatal("Game ran out of memory: trying to read map data to a buffer pointing to null! (GlobalAlloc failed?)");
            show_error_box("%s", "The game ran out of memory.\nTry disabling maps in RAM or reducing the buffer size in the chimera.ini file.");
            std::exit(EXIT_FAILURE);
        }
    }

    void set_up_map_data_read_warden() noexcept {
        Event::MapFileDataReadEvent::subscribe(on_read_map_file_data, Event::EVENT_PRIORITY_HIGHEST);
    }
}
