// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__ENGINE__WIDGET_HPP
#define BALLTZE_API__ENGINE__WIDGET_HPP

#include <string>
#include <cstdint>
#include <limits>
#include <utility>
#include "../memory.hpp"
#include "../math.hpp"
#include "../api.hpp"
#include "tag.hpp"

namespace Balltze::Engine {
    using HudGlobals = TagDefinitions::HudGlobals;

    /**
     * A WidgetEvent that indicates an analog stick has a non-zero axis value.
     */ 
    struct BALLTZE_API AnalogStickWidgetEvent {
        using Count = std::int16_t;

        /**
         * If an axis count reaches max_count or min_count, then the widget receives
         * the (left/right)_analog_stick_(up/down/left/right) events.
         */
        static constexpr Count MAX_COUNT = std::numeric_limits<Count>::max();
        static constexpr Count MIN_COUNT = std::numeric_limits<Count>::min();

        /** The measure of the analog stick along the vertical axis. */
        Count vertical;

        /** The measure of the analog stick along the horizontal axis. */
        Count horizontal;

        /** 
         * Tests if the analog stick is fully up.
         * @return `true` if the analog stick is fully up, otherwise `false`.
         */
        bool is_fully_up() const {
            return vertical == MAX_COUNT;
        }
        
        /** 
         * Tests if the analog stick is fully down.
         * @return `true` if the analog stick is fully down, otherwise `false`.
         */
        bool is_fully_down() const {
            return vertical == MIN_COUNT;
        }
        
        /** 
         * Tests if the analog stick is fully left.
         * @return `true` if the analog stick is fully left, otherwise `false`.
         */
        bool is_fully_left() const {
            return horizontal == MIN_COUNT;
        }
        
        /** 
         * Tests if the analog stick is fully right.
         * @return `true` if the analog stick is fully right, otherwise `false`.
         */
        bool is_fully_right() const {
            return horizontal == MAX_COUNT;
        }
    };

    enum InputDevice : std::uint16_t {
        INPUT_DEVICE_KEYBOARD = 1,
        INPUT_DEVICE_MOUSE,
        INPUT_DEVICE_GAMEPAD
    };

    enum GamepadButton : std::int8_t {
        GAMEPAD_BUTTON_A = 0,
        GAMEPAD_BUTTON_B,
        GAMEPAD_BUTTON_X,
        GAMEPAD_BUTTON_Y,
        GAMEPAD_BUTTON_BLACK,
        GAMEPAD_BUTTON_WHITE,
        GAMEPAD_BUTTON_LEFT_TRIGGER,
        GAMEPAD_BUTTON_RIGHT_TRIGGER,
        GAMEPAD_BUTTON_DPAD_UP,
        GAMEPAD_BUTTON_DPAD_DOWN,
        GAMEPAD_BUTTON_DPAD_LEFT,
        GAMEPAD_BUTTON_DPAD_RIGHT,
        GAMEPAD_BUTTON_START,
        GAMEPAD_BUTTON_BACK,
        GAMEPAD_BUTTON_LEFT_THUMB,
        GAMEPAD_BUTTON_RIGHT_THUMB
    };
    
    /**
     * A WidgetEvent that represents a pressed button on the gamepad.
     *
     * The PC version uses these events even for keypresses.
     * If inspecting the queues, assume that menu navigation is taking place on a controller.
     * For instance, the arrow keys will emit `button_dpad_*` events.
     *
     * Technically speaking, these events can trigger any widget event handler.
     * However, Halo only ever fills the gamepad button events.
     */
    struct GamepadButtonWidgetEvent {
        /** The gamepad button pressed. */
        GamepadButton pressed_button;

        /**
         * The duration #button has been pressed for.
         * This must be `1`, otherwise the event gets dropped during processing.
         */
        std::uint8_t duration;
    };

    enum MouseButton : std::int8_t {
        MOUSE_BUTTON_LEFT = 0,
        MOUSE_BUTTON_MIDDLE,
        MOUSE_BUTTON_RIGHT,
        MOUSE_BUTTON_DOUBLE_LEFT
    };
    
    /**
     * A WidgetEvent that represents a pressed or held button on the mouse.
     */
    struct MouseButtonWidgetEvent {
        /** Maximum duration for event */
        static constexpr auto duration_max = std::numeric_limits<std::uint8_t>::max();

        /** The mouse button pressed. */
        MouseButton button;

        /** The duration #button was held for, up to #duration_max. */
        std::uint8_t duration;
    };
    
    /** 
     * Describes an event for the widget system to process.
     *
     * The top-level widget receives these events and calls upon its handlers to process the events.
     * Depending on how the widget is set up, the events may be sent down to child widgets.
     */
    struct WidgetEvent {
        enum EventType : std::int16_t {
            TYPE_NONE = 0,
            TYPE_LEFT_ANALOG_STICK, 
            TYPE_RIGHT_ANALOG_STICK,
            TYPE_GAMEPAD_BUTTON,
            TYPE_MOUSE_BUTTON,
            TYPE_CUSTOM_ACTIVATION
        };

        union Event {
            /** Parameters for an analog stick event. */
            AnalogStickWidgetEvent analog;

            /** Parameters for a gamepad button event. */
            GamepadButtonWidgetEvent gamepad;

            /** Parameters for a mouse button event. */
            MouseButtonWidgetEvent mouse;

            /** Event raw value */
            std::int32_t value;
        };

        /** Indicates the variant of event. */
        EventType event_type;

        /** The player the event is for, or -1 for any player. */
        std::int16_t local_player_index;

        /** The event descriptor. The variant is determined by the event type. */
        Event event;
    }; 
    static_assert(sizeof(WidgetEvent) == 0x08);
    
    /**
     * Contains data necessary to store and process widget events.
     */
    struct WidgetEventGlobals {
        /**
         * A FIFO queue where the front of the queue is the last element in the array that has 
         * an `event_type` not equal to `type_none`.
         *
         * Pushing onto the queue involves a `memmove`, but Halo does not call it correctly and swaps
         * the destination and source operands.
         * As a result, when a widget event is pushed, Halo evicts the first element in the array.
         * Then it writes over the second-now-first element in the array.
         * Halo drops two events per push because of this bug.
         */
        using EventQueue = WidgetEvent[8];

        bool initialized;
        bool drop_events;

        /** The time of the last input, in milliseconds. */
        std::int32_t input_time;

        /** The time of the last update, in milliseconds. */
        std::int32_t update_time;

        /** The widget event queues, for each player. */
        EventQueue queues[4];
    };
    static_assert(sizeof(bool) == 0x1);
    
    /**
     * The widget cursor's positioning and movement.
     *
     * Widgets and the widget cursor in vanilla Halo work in a 640 by 480 grid.
     * Chimera upgrades this with the widescreen fix.
     * A couple of functions are provided to ease translation.
     */
    struct WidgetCursorGlobals {
        /** Lock to prevent recursion on cursor-related operations (speculation). */
        bool lock;
        
        /** If `true`, Halo will use `GetCursor()` to calculate changes in cursor position. */
        bool use_get_cursor;
        
        /** Halo sets this to `true` if the cursor has moved since its last update. */
        bool position_changed;

        /** The position of the cursor, in widget coordinates. */
        struct {
            /** The horizontal coordinate of the cursor, in widget coordinates. */
            std::int32_t x;

            /** The vertical coordinate of the cursor, in widget coordinates. */
            std::int32_t y;
        } position;
    };
    static_assert(sizeof(WidgetCursorGlobals) == 0x0C);

    /**
     * Widget memory pool
     */
    struct WidgetMemoryPool {
        /**
         * Handle for memory widget structure
         */
        struct ResourceHandle;

        /** Name of memory pool (widget_memory_pool) */
        const char *name;

        /** First memory pool resource */
        ResourceHandle *first_resource;
    };

    struct WidgetMemoryPool::ResourceHandle {
        enum ElementSize : std::uint16_t {
            ELEMENT_SIZE_WIDGET = 0x70,
            ELEMENT_SIZE_HISTORY_ENTRY = 0x20
        };

        /** Instance struct size */
        std::uint16_t size;

        /** 0x8000 constant */
        PADDING(0x2);

        /** Item index (?) */
        PADDING(0x4);

        /** Previous item */
        ResourceHandle *previous;

        /** Next item */
        ResourceHandle *next;

        /**
         * Get element
         */
        template<typename T> inline T &get_element() noexcept {
            return *reinterpret_cast<T *>(reinterpret_cast<std::uint32_t>(this) + sizeof(ResourceHandle));
        }
    };
    static_assert(sizeof(WidgetMemoryPool::ResourceHandle) == 0x10);

    struct Widget {
        enum Type : std::uint16_t {
            WIDGET_TYPE_CONTAINER = 0,
            WIDGET_TYPE_TEXT_BOX,
            WIDGET_TYPE_SPINNER_LIST,
            WIDGET_TYPE_COLUMN_LIST,
            WIDGET_TYPE_GAME_MODEL,
            WIDGET_TYPE_MOVIE,
            WIDGET_TYPE_CUSTOM
        };

        /** ID of the widget tag */
        TagHandle definition_tag_id;

        /** Name of the widget */
        const char *name;

        /** Sets if the widget is hidden */
        std::uint16_t hidden;

        /** Widget frame left bound */
        std::int16_t left_bound;

        /** Widget frame top bound */
        std::int16_t top_bound;

        /** Widget type */
        Type type;

        /** Unknown flags related to the widget history */
        std::uint16_t visible;
        PADDING(0x2);
        PADDING(0x4);

        /** A widget instance related to the history */
        PADDING(0x4);

        /** Milliseconds to close widgets */
        std::uint32_t ms_to_close;

        /** Widget close fade time in milliseconds */
        std::uint32_t ms_to_close_fade_time;
        
        /** Widget opacity (from 0 to 1) */
        float opacity;

        /** Previous widget of the list. Null on first list item. */
        Widget *previous_widget;

        /** Next widget of the list. Null on last list item. */
        Widget *next_widget;

        /** Parent widget. Null on root widget. */
        Widget *parent_widget;

        /** Child widget. Null if there is no child items. */
        Widget *child_widget;

        /** focused child widget. Null in non-list widgets. */
        Widget *focused_child;

        /** Text box content. Null in non-text box widgets. */
        const wchar_t *text;

        /** Last widget list element focused by cursor */
        std::uint16_t cursor_index;
        PADDING(0x2);

        PADDING(0x4);
        PADDING(0x4);
        PADDING(0x4);

        PADDING(0x4);
        PADDING(0x4);

        /** Background bitmap index */
        std::uint16_t bitmap_index;
        PADDING(0x2);

        PADDING(0x4);

        /** 
         * Get header for this instance
         */
        inline WidgetMemoryPool::ResourceHandle &get_handle() noexcept {
            return *reinterpret_cast<WidgetMemoryPool::ResourceHandle *>(reinterpret_cast<std::uint32_t>(this) - sizeof(WidgetMemoryPool::ResourceHandle));
        }

        /**
         * Get string for a widget type
         * @param type      Code of widget type
         * @return          Stringified widget type
         */
        BALLTZE_API static std::string type_to_string(Type type) noexcept;
    }; 
    static_assert(sizeof(Widget) == 0x60);

    struct WidgetHistoryEntry {
        /** Previous menu root widget */
        Widget *previous_menu;

        /** Previous menu list widget */
        Widget *previous_menu_list;

        /** Previous menu list foccused item index */
        std::uint16_t focused_item_index;
        PADDING(0x2);

        /** Previous history entry */
        WidgetHistoryEntry *previous;
    };
    static_assert(sizeof(WidgetHistoryEntry) == 0x10);
    
    /**
     * Describes the general state of widgets and widget display.
     */ 
    struct WidgetGlobals {
        /** An error that is in queue. */
        struct EnqueuedErrorDescriptor {
            /** Index of the error in the error strings tag. */
            std::int16_t error_string;

            /** Index of the local player the error is for. */
            std::int16_t local_player;

            /** Are a modal error? */
            bool display_modal;

            /** Pauses the game? */
            bool display_paused;
        };
        static_assert(sizeof(EnqueuedErrorDescriptor) == 0x06);

        /** An error that is waiting for the current cinematic to end before being displayed. */
        struct DeferredErrorDescriptor {
            /** Index of the error in the error strings tag. */
            std::int16_t error_string;

            /** Are a modal error? */
            bool display_modal;

            /** Pauses the game? */
            bool display_paused;
        };
        static_assert(sizeof(DeferredErrorDescriptor) == 0x04);

        /** The root widget instance for current menu. */
        Widget *root_widget;

        /** Last widget history entry */
        WidgetHistoryEntry *history_top_entry;

        /** Current time in milliseconds */
        std::int32_t current_time;

        /** Ticks remaining for popup (i think) */
        std::int32_t popup_display_time;
        
        std::int16_t error_message_index;
        std::int16_t widget_pause_counter;

        PADDING(0x4); // float

        /** Errors queue for each local player */
        EnqueuedErrorDescriptor enqueued_errors[1];

        /** takes precedence over enqueued_errors, always displays modal, non-paused */
        DeferredErrorDescriptor priority_warning;

        /** Deferred errors for each local player */
        DeferredErrorDescriptor deferred_for_cinematic_errors[1];

        /** no path sets this, real type is HANDLE* */
        void *initialization_thread;

        /**
         * 1 = all progress will be lost, 2 = insert another quarter
         * only used on the widget update after initialization_thread exits
         * does anyone know if an arcade version of Halo 1 was planned?
         */
        std::int16_t demo_error;

        /** Is this struct initialized? */
        bool initialized;

        PADDING(0x01); // bool
        PADDING(0x01); // bool
        PADDING(0x01); // bool
        PADDING(0x01); // bool
        PADDING(0x01); // bool
    };
    static_assert(sizeof(WidgetGlobals) == 0x34);

    enum WidgetNavigationSound {
        WIDGET_NAVIGATION_SOUND_CURSOR = 0,
        WIDGET_NAVIGATION_SOUND_FORWARD,
        WIDGET_NAVIGATION_SOUND_BACK,
        WIDGET_NAVIGATION_SOUND_FLAG_FAILURE
    };

    /**
     * Bounds field from ui widget definition tag
     */
    struct WidgetDefinitionBounds {
        std::uint16_t top;
        std::uint16_t left;
        std::uint16_t bottom;
        std::uint16_t right;
    };
    static_assert(sizeof(WidgetDefinitionBounds) == 0x08);

    /**
     * This structure is passed as value to the function that handles 
     * the DX9 bitmap render stuff. It represents the rectangle where
     * the background bitmap of the widget will be drawn.
     */
    struct WidgetRenderArea {
        struct Corner {
            float x;
            float y;
            PADDING(0x8);
            float unknown[2];
        };

        Corner top_left;
        Corner top_right;
        Corner bottom_right;
        Corner bottom_left;
    };

    /**
     * Get the widget event globals
     * @return reference to the widget event globals
     */
    BALLTZE_API WidgetEventGlobals *get_widget_event_globals();

    /**
     * Get the widget cursor globals
     * @return reference to the widget cursor globals
     */
    BALLTZE_API WidgetCursorGlobals *get_widget_cursor_globals();

    /**
     * Get the widget globals
     * @return reference to the widget globals
     */
    BALLTZE_API WidgetGlobals *get_widget_globals();

    /**
     * Get string for a gamepad button
     * @param button    Gamepad button code
     * @return          Stringified code
     */
    BALLTZE_API std::string get_gamepad_button_name(GamepadButton button) noexcept;

    /**
     * Get string for a mouse button
     * @param button    Mouse button code
     * @return          Stringified button
     */
    BALLTZE_API std::string get_mouse_button_name(MouseButton button) noexcept;

    /**
     * Get string for a widget navigation sound
     * @param sound     Code of a navigation sound
     * @return          Sound string
     */
    BALLTZE_API std::string get_widget_navigation_sound_name(WidgetNavigationSound sound) noexcept;
    
    /**
     * Find a widget from a given widget definition.
     * This is the function used by the game; it only returns the first coincidence.
     * @param widget_definition     Widget definition tag ID of the widget to find
     * @param widget_base           Widget where to look
     * @return                      Pointer to widget if found, nullptr if not
     */
    BALLTZE_API Widget *find_widget(TagHandle widget_definition, Widget *widget_base = nullptr) noexcept;

    /**
     * Find widgets from a given widget definition.
     * @param widget_definition     Widget definition tag ID of the widget to find
     * @param widget_base           Widget where to look
     * @return                      Vector of widgets
     */
    BALLTZE_API std::vector<Widget *> find_widgets(TagHandle widget_definition, bool first_match = true, Widget *widget_base = nullptr) noexcept;

    /**
     * Open a widget
     * @param widget_definition     Tag ID of widget definition
     * @param push_history          Push or not the current root widget to menu history.
     * @return                      Pointer to the new widget
     */
    BALLTZE_API Widget *open_widget(TagHandle widget_definition, bool push_history = true) noexcept;

    /**
     * Close current root widget; return to the previous one in history.
     */
    BALLTZE_API void close_widget() noexcept;

    /**
     * Replace a widget
     * @param widget                Widget to be replaced
     * @param widget_definition     Tag ID of the definition for the widget replace 
     * @return                      Pointer to the new widget
     */
    BALLTZE_API Widget *replace_widget(Widget *widget, TagHandle widget_definition) noexcept;

    /**
     * Reload a widget.
     * @param widget    Widget to reload
     * @return          Pointer to the new widget
     */
    BALLTZE_API Widget *reload_widget(Widget *widget) noexcept;

    /**
     * Focus a widget.
     * @param widget    Widget to be focused
     */
    BALLTZE_API void focus_widget(Widget *widget) noexcept;

    /**
     * Open the pause menu
     */
    BALLTZE_API void open_pause_menu() noexcept;

    /**
     * Gets the HUD globals tag.
     * @return  The HUD globals tag.
     */
    BALLTZE_API HudGlobals &get_hud_globals();

    /**
     * Gets the size of a sprite in a bitmap.
     * @param bitmap                Bitmap tag data.
     * @param sequence_index        Sequence index of the bitmap.
     * @return                      The size of the sprite.
     * @throws std::runtime_error   If the bitmap tag handle is invalid.
     * @throws std::runtime_error   If the sequence index is invalid.
     * @throws std::runtime_error   If the sprite index is invalid.
     */
    BALLTZE_API std::pair<std::size_t, std::size_t> get_bitmap_sequence_size(Engine::TagDefinitions::Bitmap *bitmap, std::size_t sequence_index);

    /**
     * Draws a icon bitmap on a HUD message.
     * @param bitmap                Pointer to bitmap tag data.
     * @param sequence_index        Sequence index of the bitmap.
     * @param sprite_index          The index of the sprite from the sequence.
     * @param position              The position of the bitmap on the HUD.
     * @param color                 The color of the bitmap.
     * @throws std::runtime_error   If the bitmap tag handle is invalid.
     * @throws std::runtime_error   If the sequence index is invalid.
     * @throws std::runtime_error   If the sprite index is invalid.
     */
    BALLTZE_API void draw_hud_message_sprite(Engine::TagDefinitions::Bitmap *bitmap, std::size_t sequence_index, std::size_t sprite_index, Point2DInt position, ColorARGBInt color);

    /**
     * Get the name of a button.
     * @param input_device          The input device.
     * @param button_index          The button index.
     * @return                      The name of the button.
     */
    BALLTZE_API std::wstring get_button_name(Engine::InputDevice input_device, std::size_t button_index);
}

#endif
