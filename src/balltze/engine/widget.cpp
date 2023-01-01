// SPDX-License-Identifier: GPL-3.0-only

#include <functional>
#include <d3d9.h>
#include <cmath>
#include <balltze/engine/resolution.hpp>
#include <balltze/memory.hpp>
#include <balltze/engine/widget.hpp>

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
            free_widget_asm(current_root_widget);
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
