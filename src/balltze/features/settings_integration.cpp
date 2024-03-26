// SPDX-License-Identifier: GPL-3.0-only

#include <optional>
#include <string>
#include <balltze/engine/tag.hpp>
#include <balltze/engine/tag_definitions.hpp>
#include <balltze/events/map_load.hpp>
#include <balltze/events/widget.hpp>
#include <balltze/features/tag_data_handling.hpp>
#include "../logger.hpp"

namespace Balltze::Features {
    using namespace Event;
    using namespace std::string_literals;

    static const auto audio_options_menu_tag_path = "ui\\shell\\main_menu\\settings_select\\player_setup\\player_profile_edit\\audio_settings\\audio_options_menu"s;
    static const auto software_buffers_spinner_tag_path = "ui\\shell\\main_menu\\settings_select\\player_setup\\player_profile_edit\\audio_settings\\op_software_buffers"s;
    static const auto sound_subtitles_spinner_name = "sound_subtitles_spinner"s;
    static const auto sound_subtitles_spinner_label = L"SUBTITLES"s;
    static std::vector<Engine::TagDefinitions::ChildWidgetReference> audio_options_menu_children;
    static std::vector<Engine::TagDefinitions::UnicodeStringListString> audio_options_menu_strings;
    static std::optional<Engine::TagHandle> sound_subtitles_spinner;

    static void on_map_load(MapLoadEvent &ev) {
        if(ev.time == EVENT_TIME_AFTER) {
            auto *audio_variety_spinner_tag = get_tag(software_buffers_spinner_tag_path, Engine::TAG_CLASS_UI_WIDGET_DEFINITION);
            auto *audio_options_menu_tag = get_tag(audio_options_menu_tag_path, Engine::TAG_CLASS_UI_WIDGET_DEFINITION);
            if(!audio_variety_spinner_tag || !audio_options_menu_tag) {
                return;
            }

            // Create spinnerr for sound subtitles
            auto tag_copy_handle = clone_tag(audio_variety_spinner_tag->handle, sound_subtitles_spinner_name);
            auto *sound_subtitles_spinner_tag = get_tag(tag_copy_handle);
            sound_subtitles_spinner_tag->resolve_dependencies([](Engine::TagHandle handle) -> Engine::TagHandle {
                auto *tag = get_tag(handle);
                if(tag->primary_class == Engine::TAG_CLASS_BITMAP) {
                    return handle;
                }
                return clone_tag(handle, sound_subtitles_spinner_name);
            });
            sound_subtitles_spinner = sound_subtitles_spinner_tag->handle;

            // Change spinner label text
            auto *sound_subtitles_spinner_tag_data = reinterpret_cast<Engine::TagDefinitions::UiWidgetDefinition *>(sound_subtitles_spinner_tag->data);
            auto sound_subtitles_spinner_label_tag_handle = sound_subtitles_spinner_tag_data->child_widgets.offset[0].widget_tag.tag_handle;
            auto *sound_subtitles_spinner_label_tag = get_tag(sound_subtitles_spinner_label_tag_handle);
            auto *sound_subtitles_spinner_label_tag_data = reinterpret_cast<Engine::TagDefinitions::UiWidgetDefinition *>(sound_subtitles_spinner_label_tag->data);
            auto *string_list_tag = get_tag(sound_subtitles_spinner_label_tag_data->text_label_unicode_strings_list.tag_handle);
            auto *string_list_tag_data = reinterpret_cast<Engine::TagDefinitions::UnicodeStringList *>(string_list_tag->data);
            audio_options_menu_strings.clear();
            audio_options_menu_strings.reserve(string_list_tag_data->strings.count + 1);
            for(auto i = 0; i < string_list_tag_data->strings.count; i++) {
                audio_options_menu_strings.push_back(string_list_tag_data->strings.offset[i]);
            }
            auto subtitles_label = audio_options_menu_strings[0];
            subtitles_label.string.pointer = reinterpret_cast<std::byte *>(const_cast<wchar_t *>(sound_subtitles_spinner_label.data()));
            subtitles_label.string.size = (sound_subtitles_spinner_label.size() + 1) * sizeof(wchar_t);
            audio_options_menu_strings.push_back(subtitles_label);
            string_list_tag_data->strings.count = audio_options_menu_strings.size();
            string_list_tag_data->strings.offset = audio_options_menu_strings.data();
            sound_subtitles_spinner_label_tag_data->string_list_index = audio_options_menu_strings.size() - 1;            

            // Create new child widgets list for audio options menu
            auto *audio_options_menu_tag_data = reinterpret_cast<Engine::TagDefinitions::UiWidgetDefinition *>(audio_options_menu_tag->data);
            audio_options_menu_children.clear();
            audio_options_menu_children.reserve(audio_options_menu_tag_data->child_widgets.count + 1);
            for(auto i = 0; i < audio_options_menu_tag_data->child_widgets.count; i++) {
                audio_options_menu_children.push_back(audio_options_menu_tag_data->child_widgets.offset[i]);
            }

            // Create sound subtitles spinner tag reference
            Engine::TagDefinitions::ChildWidgetReference sound_subtitles_spinner_ref = audio_options_menu_children[audio_options_menu_children.size() - 3];
            std::strncpy(sound_subtitles_spinner_ref.name.string, sound_subtitles_spinner_name.c_str(), sizeof(sound_subtitles_spinner_ref.name.string));
            sound_subtitles_spinner_ref.widget_tag.tag_handle = sound_subtitles_spinner->handle;

            // Update new item position
            auto &last_item = audio_options_menu_children[audio_options_menu_children.size() - 2];
            auto diff = last_item.vertical_offset - sound_subtitles_spinner_ref.vertical_offset;
            sound_subtitles_spinner_ref.vertical_offset = last_item.vertical_offset + diff;

            // Update extended description position
            auto extended_description_tag_handle = audio_options_menu_tag_data->extended_description_widget.tag_handle;
            auto *extended_description_tag = get_tag(extended_description_tag_handle);
            if(extended_description_tag) {
                auto *extended_description_tag_data = reinterpret_cast<Engine::TagDefinitions::UiWidgetDefinition *>(extended_description_tag->data);
                extended_description_tag_data->bounds.top += diff;
                extended_description_tag_data->bounds.bottom += diff;
            }

            // Update audio options menu tag data   
            audio_options_menu_children.insert(audio_options_menu_children.end() - 1, sound_subtitles_spinner_ref);
            audio_options_menu_tag_data->child_widgets.count = audio_options_menu_children.size();
            audio_options_menu_tag_data->child_widgets.offset = audio_options_menu_children.data();
        }
    }

    void set_up_settings_integration() {
        MapLoadEvent::subscribe(on_map_load, EVENT_PRIORITY_LOWEST);
    }
}
