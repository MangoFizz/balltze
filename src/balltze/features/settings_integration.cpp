

#include <optional>
#include <string>
#include <balltze/events/widget.hpp>
#include <balltze/features/tag_data_handling.hpp>

namespace Balltze::Features {
    using namespace Event;
    using namespace std::string_literals;

    static std::optional<Engine::TagHandle> sound_subtitles_spinner;

    static void on_widget_open(UIWidgetOpenEvent &ev) {
        if(ev.time == EVENT_TIME_AFTER) {
            auto widget = ev.args.widget;
            if(!sound_subtitles_spinner) {
                auto *tag = get_tag(widget->definition_tag_handle);
                auto definition_copy = clone_tag(widget->definition_tag_handle, "subtitles_spinner\\"s + tag->path);
                auto *tag_copy = get_tag(definition_copy);
                tag_copy->resolve_dependencies([](Engine::TagHandle handle) -> Engine::TagHandle {
                    auto *tag = get_tag(handle);
                    return clone_tag(handle, "subtitles_spinner\\"s + tag->path);
                });
                sound_subtitles_spinner = definition_copy;
            }

            // auto *widget = Engine::
        }
    }
}
