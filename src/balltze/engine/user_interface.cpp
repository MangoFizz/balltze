// SPDX-License-Identifier: GPL-3.0-only

#include <functional>
#include <d3d9.h>
#include <cmath>
#include <invader/sound/sound_reader.hpp>
#include <balltze/memory.hpp>
#include <balltze/engine/multiplayer.hpp>
#include <balltze/engine/user_interface.hpp>
#include "../logger.hpp"

namespace Balltze::Engine {
    WidgetEventGlobals *get_widget_event_globals() {
        static auto *widget_event_globals_sig = Memory::get_signature("widget_event_globals");
        static auto *widget_event_globals = *reinterpret_cast<WidgetEventGlobals **>(widget_event_globals_sig->data());
        return widget_event_globals;
    }
    
    WidgetCursorGlobals *get_widget_cursor_globals() {
        static auto *cursor_globals_sig = Memory::get_signature("widget_cursor_globals");
        static auto *widget_cursor_globals = *reinterpret_cast<WidgetCursorGlobals **>(cursor_globals_sig->data());
        return widget_cursor_globals;
    }
    
    WidgetGlobals *get_widget_globals() {
        static auto *widget_globals_sig = Memory::get_signature("widget_globals");
        static auto *widget_globals = *reinterpret_cast<WidgetGlobals **>(widget_globals_sig->data());
        return widget_globals;
    }

    std::vector<Widget *> find_widgets(TagHandle widget_definition, bool first_match, Widget *widget_base) noexcept {
        if(!widget_base) {
            auto *widget_globals = get_widget_globals();
            widget_base = widget_globals->root_widget;
        }

        std::vector<Widget *> found_widgets;
            
        std::function<void(Widget *)> search_instances = [&](Widget *widget) {
            if(first_match && !found_widgets.empty()) {
                return;
            }
            
            if(widget->definition_tag_id == widget_definition) {
                found_widgets.push_back(widget);
                return;
            }

            if(widget->child_widget) {
                search_instances(widget->child_widget);
            }

            if(widget->next_widget) {
                search_instances(widget->next_widget);
            }
        };
        search_instances(widget_base);

        return std::move(found_widgets);
    }

    extern "C" {
        /**
         * Create a widget.
         * @param widget_definition     Tag ID of widget definition
         * @param widget_replace        Widget to replace; can be null (not sure about this description)
         * @return                      Pointer to the new widget
         */
        Widget *create_widget_asm(TagHandle widget_definition, Widget *widget_replace = nullptr) noexcept;

        /**
         * Open a widget and creates a history entry for current root widget.
         * If there isn't a widget, the widget will not be opened.
         * @param current_root_widget   Current root widget
         * @param widget_id             Tag ID of the widget to open
         * @return                      Pointer to the new widget
         */
        Widget *open_widget_asm(Widget *current_root_widget, TagHandle widget_definition) noexcept;

        /**
         * Close current root widget, return to the previous one in history
         * @param current_root_widget   Current root widget
         */
        void close_widget_asm(Widget *current_root_widget) noexcept;

        /**
         * Find a widget from a given widget definition
         * @param widget                Widget where to look
         * @param widget_definition     Tag ID of a widget definition
         * @return                      Widget if found, nullptr if not
         */
        Widget *find_widget_asm(Widget *widget, TagHandle widget_definition) noexcept;

        /**
         * Focus a child widget from a given widget
         * @param root_widget       Root widget
         * @param widget_to_focus   Child widget to focus
         */
        void focus_widget_asm(Widget *root_widget, Widget *widget_to_focus) noexcept;

        /**
         * Get index for widget list item
         * @param widget    Widget list item
         * @return          Index of the list item, -1 if widget is null.
         */
        std::uint16_t get_widget_list_item_index_asm(Widget *widget) noexcept; 

        /**
         * Release widget memory
         * @param widget    Pointer to widget to release
         */
        void free_widget_asm(Widget *widget) noexcept; 

        /**
         * Draw a bitmap sprite
         * @param position      Position to draw the sprite
         * @param color         Color to draw the sprite
         * @param bitmap_data   Pointer to bitmap data
         * @param sprite        Pointer to sprite element
         */
        void hud_draw_bitmap_sprite(Point2DInt position, ColorARGBInt color, TagDefinitions::BitmapData *bitmap_data, TagDefinitions::BitmapGroupSprite *sprite);

        /**
         * Get the name for a button input
         * @param input_device      Input device
         * @param button_index      Button index
         * @param buffer            Buffer to write the name to
         */
        void get_string_name_for_button(InputDevice input_device, std::size_t button_index, wchar_t *buffer);

        /**
         * Play a sound from a given tag
         * @param sound     Tag ID of the sound
         */
        void play_sound_asm(TagHandle sound);
    }

    Widget *find_widget(TagHandle widget_definition, Widget *widget_base) noexcept {
        if(!widget_base) {
            auto *widget_globals = get_widget_globals();
            widget_base = widget_globals->root_widget;
        }

        return find_widget_asm(widget_base, widget_definition);
    }

    Widget *open_widget(TagHandle widget_definition, bool push_history) noexcept {
        auto *widget_globals = get_widget_globals();
        auto *current_root_widget = widget_globals->root_widget;
        
        if(push_history && current_root_widget) {
            return open_widget_asm(current_root_widget, widget_definition);
        }
        else {
            if(current_root_widget) {
                free_widget_asm(current_root_widget);
            }
            return create_widget_asm(widget_definition);
        }
    }

    void close_widget() noexcept {
        auto *widget_globals = get_widget_globals();
        auto *current_root_widget = widget_globals->root_widget;

        // fool proof
        if(current_root_widget) {
            close_widget_asm(current_root_widget);
        }
    }

    Widget *replace_widget(Widget *widget, TagHandle widget_definition) noexcept {
        auto *new_widget = create_widget_asm(widget_definition, widget->parent_widget);
        auto *old_widget = widget;
        auto *old_widget_parent = widget->parent_widget;
        auto *old_widget_previous = widget->previous_widget;
        auto *old_widget_next = widget->next_widget;
        auto *old_widget_focused = widget->focused_child;

        new_widget->left_bound = old_widget->left_bound;
        new_widget->top_bound = old_widget->top_bound;

        new_widget->parent_widget = nullptr;
        new_widget->previous_widget = nullptr;
        new_widget->next_widget = nullptr;
        new_widget->focused_child = nullptr;

        if(old_widget_parent) {
            new_widget->parent_widget = old_widget_parent;

            if(old_widget_parent->child_widget == old_widget) {
                old_widget_parent->child_widget = new_widget;
            }

            if(old_widget_parent->focused_child == old_widget) {
                old_widget_parent->focused_child = new_widget;
            } 
            
            old_widget->parent_widget = nullptr;
        }

        if(old_widget_previous) {
            new_widget->previous_widget = old_widget_previous;
            old_widget_previous->next_widget = new_widget;
            old_widget->previous_widget = nullptr;
        }

        if(old_widget_next) {
            new_widget->next_widget = old_widget_next;
            old_widget_next->previous_widget = new_widget;
            old_widget->next_widget = nullptr;
        }

        if(new_widget->type == Widget::WIDGET_TYPE_COLUMN_LIST) {
            new_widget->focused_child = new_widget->child_widget;
        }

        auto *widget_globals = get_widget_globals();
        auto *root_widget = widget_globals->root_widget;
        if(widget == root_widget) {
            widget_globals->root_widget = new_widget;
        }

        // Free old widget
        free_widget_asm(old_widget);

        return new_widget;
    }

    Widget *reload_widget(Widget *widget) noexcept {
        // Save old widget focused item
        std::uint16_t focused_list_item_index = get_widget_list_item_index_asm(widget->focused_child);
        std::uint16_t focused_list_item_bitmap_index = widget->focused_child->bitmap_index;

        // Create new widget
        Widget *new_widget = replace_widget(widget, widget->definition_tag_id);

        if(focused_list_item_index != -1) {
            auto *child_widget = new_widget->child_widget;
            for(std::size_t i = 0; i < focused_list_item_index; i++) {
                if(!child_widget) {
                    child_widget = new_widget->child_widget;
                    break;
                }
                child_widget = child_widget->next_widget;
            }

            if(child_widget) {
                new_widget->focused_child = child_widget;
                child_widget->bitmap_index = focused_list_item_bitmap_index;
            }
        }

        return new_widget;
    }

    void focus_widget(Widget *widget) noexcept {
        // fool proof
        if(widget->parent_widget) {
            focus_widget_asm(widget->parent_widget, widget);
        }
    }

    void open_pause_menu() noexcept {
        if(get_widget_globals()->root_widget) {
            return;
        }
        auto server_type = get_server_type();
        const char *tag_path = nullptr;
        if(server_type == SERVER_NONE) {
            auto *singleplayer_pause_menu_tag_path_sig = Memory::get_signature("singleplayer_pause_menu_tag_path");
            tag_path = *reinterpret_cast<const char **>(singleplayer_pause_menu_tag_path_sig->data());
        }
        else {
            auto *multiplayer_pause_menu_tag_path_sig = Memory::get_signature("multiplayer_pause_menu_tag_path");
            tag_path = *reinterpret_cast<const char **>(multiplayer_pause_menu_tag_path_sig->data());
        }
        if(tag_path) {
            auto *tag = get_tag(tag_path, TAG_CLASS_UI_WIDGET_DEFINITION);
            if(tag) {
                open_widget(tag->id, false);
            }
        }
    }

    HudGlobals &get_hud_globals() {
        static auto *sig = Memory::get_signature("hud_icon_messages_tag_id");
        if(!sig) {
            throw std::runtime_error("Could not find signature for hud icon messages tag id");
        }
        static auto **hud_globals = *reinterpret_cast<HudGlobals ***>(sig->data());
        return **hud_globals;
    }

    std::pair<std::size_t, std::size_t> get_bitmap_sequence_size(Engine::TagDefinitions::Bitmap *bitmap, std::size_t sequence_index) {
        if(sequence_index >= bitmap->bitmap_group_sequence.count) {
            throw std::runtime_error("Invalid sequence index");
        }
        auto &sequence = bitmap->bitmap_group_sequence.offset[sequence_index];
        if(sequence.sprites.count == 0) {
            throw std::runtime_error("Sequence has no sprites");
        }
        auto &sprite = sequence.sprites.offset[0];
        auto &bitmap_data = bitmap->bitmap_data.offset[sprite.bitmap_index];

        auto sprite_width = (sprite.right - sprite.left) * bitmap_data.width;
        auto sprite_height = (sprite.bottom - sprite.top) * bitmap_data.height;

        return {sprite_width, sprite_height};
    }

    void draw_hud_message_sprite(Engine::TagDefinitions::Bitmap *bitmap, std::size_t sequence_index, std::size_t sprite_index, Point2DInt position, ColorARGBInt color) {
        if(sequence_index >= bitmap->bitmap_group_sequence.count) {
            throw std::runtime_error("Invalid sequence index");
        }
        auto &sequence = bitmap->bitmap_group_sequence.offset[sequence_index];
        if(sprite_index >= sequence.sprites.count) {
            throw std::runtime_error("Invalid sprite index");
        }
        auto &sprite = sequence.sprites.offset[sprite_index];
        auto &bitmap_data = bitmap->bitmap_data.offset[sprite.bitmap_index];

        auto offset = position;
        auto [sprite_width, sprite_height] = get_bitmap_sequence_size(bitmap, sequence_index);

        // align to text baseline
        offset.y += sprite_height * 0.75f;

        hud_draw_bitmap_sprite(offset, color, &bitmap_data, &sprite);
    }

    std::wstring get_button_name(InputDevice input_device, std::size_t button_index) {
        wchar_t button_name[64];
        get_string_name_for_button(input_device, button_index, button_name);
        return button_name;
    }

    void play_sound(TagHandle tag_sound) {
        if(tag_sound.is_null() || !get_tag(tag_sound)) {
            throw std::runtime_error("Invalid sound tag");
        }
        play_sound_asm(tag_sound);
    }

    std::chrono::milliseconds get_sound_permutation_samples_duration(TagDefinitions::Sound *sound, TagDefinitions::SoundPermutation *permutation) {
        if(!sound || !permutation) {
            throw std::runtime_error("Invalid sound or permutation");
        }
        
        bool found = false;
        for(std::size_t i = 0; i < sound->pitch_ranges.count; i++) {
            auto &pitch_range = sound->pitch_ranges.offset[i];
            if(pitch_range.permutations.count > 0) {
                if(permutation >= pitch_range.permutations.offset && permutation < pitch_range.permutations.offset + pitch_range.permutations.count) {
                    found = true;
                    break;
                }
            }
        }
        if(!found) {
            throw std::runtime_error("Permutation not found in sound");
        }

        std::size_t channel_count;
        switch(sound->channel_count) {
            case TagDefinitions::SoundChannelCount::MONO:
                channel_count = 1;
                break;

            case TagDefinitions::SoundChannelCount::STEREO:
                channel_count = 2;
                break;

            default:
                throw std::runtime_error("Invalid channel count in sound");
        }

        std::size_t sample_rate;
        switch(sound->sample_rate) {
            case TagDefinitions::SoundSampleRate::_22050__HZ:
                sample_rate = 22050;
                break;

            case TagDefinitions::SoundSampleRate::_44100__HZ:
                sample_rate = 44100;
                break;

            default:
                throw std::runtime_error("Invalid sample rate in sound");
        }

        if(!permutation->samples_pointer) {
            throw std::runtime_error("Samples not loaded yet");
        }

        auto *samples_pointer = reinterpret_cast<const std::byte *>(permutation->samples_pointer);
        Invader::SoundReader::Sound sound_data;
        switch(permutation->format) {
            case TagDefinitions::SoundFormat::_16_BIT_PCM: 
                sound_data = Invader::SoundReader::sound_from_16_bit_pcm_big_endian(samples_pointer, permutation->samples.size, channel_count, sample_rate);
                break;

            case TagDefinitions::SoundFormat::XBOX_ADPCM:
                sound_data = Invader::SoundReader::sound_from_xbox_adpcm(samples_pointer, permutation->samples.size, channel_count, sample_rate);
                break;

            case TagDefinitions::SoundFormat::OGG_VORBIS: 
                sound_data = Invader::SoundReader::sound_from_ogg(samples_pointer, permutation->samples.size);
                break;

            default:
                throw std::runtime_error("Invalid sound format");
        }

        return std::chrono::milliseconds(static_cast<std::size_t>(std::ceil(static_cast<float>(sound_data.pcm.size()) / (sound_data.bits_per_sample / 8.0f) / channel_count * 1000.0f / sound_data.sample_rate)));
    }

    std::string get_gamepad_button_name(GamepadButton button) noexcept {
        switch(button) {
            case GAMEPAD_BUTTON_A:
                return "a";
            case GAMEPAD_BUTTON_B: 
                return "b";
            case GAMEPAD_BUTTON_X:
                return "x";
            case GAMEPAD_BUTTON_Y:
                return "y";
            case GAMEPAD_BUTTON_BLACK:
                return "black";
            case GAMEPAD_BUTTON_WHITE:
                return "white";
            case GAMEPAD_BUTTON_LEFT_TRIGGER:
                return "left trigger";
            case GAMEPAD_BUTTON_RIGHT_TRIGGER:
                return "right trigger";
            case GAMEPAD_BUTTON_DPAD_UP:
                return "dpad up";
            case GAMEPAD_BUTTON_DPAD_DOWN:
                return "dpad down";
            case GAMEPAD_BUTTON_DPAD_LEFT:
                return "dpad left";
            case GAMEPAD_BUTTON_DPAD_RIGHT:
                return "dpad right";
            case GAMEPAD_BUTTON_START:
                return "start";
            case GAMEPAD_BUTTON_BACK:
                return "back";
            case GAMEPAD_BUTTON_LEFT_THUMB:
                return "left thumb";
            case GAMEPAD_BUTTON_RIGHT_THUMB:
                return "right thumb";
            default:
                return "unknown button " + std::to_string(button);
        }
    }

    std::string get_mouse_button_name(MouseButton button) noexcept {
        switch(button) {
            case MOUSE_BUTTON_LEFT:
                return "left";
            case MOUSE_BUTTON_MIDDLE:
                return "middle";
            case MOUSE_BUTTON_RIGHT:
                return "right";
            case MOUSE_BUTTON_DOUBLE_LEFT:
                return "double left";
            default:
                return "unknown mouse button " + std::to_string(button);
        }
    }

    std::string Widget::type_to_string(Widget::Type type) noexcept {
        switch(type) {
            case WIDGET_TYPE_CONTAINER:
                return "container";
            case WIDGET_TYPE_TEXT_BOX:
                return "text box";
            case WIDGET_TYPE_SPINNER_LIST:
                return "spinner list";
            case WIDGET_TYPE_COLUMN_LIST:
                return "column list";
            case WIDGET_TYPE_GAME_MODEL:
                return "game model";
            case WIDGET_TYPE_MOVIE:
                return "movie";
            case WIDGET_TYPE_CUSTOM:
                return "custom";
            default:
                return "unknown widget type " + std::to_string(type);
        }
    }

    std::string get_widget_navigation_sound_name(WidgetNavigationSound sound) noexcept {
        switch(sound) {
            case WIDGET_NAVIGATION_SOUND_CURSOR:
                return "cursor";
            case WIDGET_NAVIGATION_SOUND_FORWARD:
                return "forward";
            case WIDGET_NAVIGATION_SOUND_BACK:
                return "back";
            case WIDGET_NAVIGATION_SOUND_FLAG_FAILURE:
                return "flag failure";
            default:
                return "unknown navigation sound " + std::to_string(sound);
        }
    }
}
