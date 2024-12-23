// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__ENGINE__WIDGET_HPP
#define BALLTZE_API__ENGINE__WIDGET_HPP

#include <string>
#include <cstdint>
#include <limits>
#include <utility>
#include <chrono>
#include <windows.h>
#include <dinput.h>
#include "../memory.hpp"
#include "../math.hpp"
#include "../api.hpp"
#include "core.hpp"
#include "rasterizer.hpp"
#include "tag_definitions/hud_globals.hpp"
#include "tag_definitions/bitmap.hpp"
#include "tag_definitions/ui_widget_definition.hpp"
#include "tag_definitions/sound.hpp"
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

    struct InputGlobals {
        // seems to be some sort of timestamp
        struct UnknownStructure {
            std::int32_t milliseconds;
            std::int16_t unknown;
        };
        static_assert(sizeof(UnknownStructure) == 0x08);

        /**
         * Stores a keypress as seen from WM_CHAR, WM_SYSCHAR, WM_KEYDOWN, or WM_SYSKEYDOWN.
         */
        struct BufferedKey {
            enum Modifier {
                MODIFIER_SHIFT = 1,
                MODIFIER_CTRL = 2,
                MODIFIER_ALT = 4
            };

            /** ALT/CONTROL/SHIFT (LSB) */
            std::int8_t modifiers;

            /**
             * The lower byte of wParam
             * If the event was generated by WM_KEYDOWN or WM_SYSKEYDOWN: -1
             */
            std::int8_t character;

            /** internally translated, need work on this */
            std::int16_t keycode;
        };
        static_assert(sizeof(BufferedKey) == 0x04 && alignof(BufferedKey) == 0x02);

        /**
         * A buffer translated from the mouse device state.
         */
        struct MouseState {
            /** Horizontal mouse movement. */
            std::int32_t horizontal;

            /** Vertical mouse movement. */
            std::int32_t vertical;

            /** Comes divided by granularity, if granularity is non-zero. */
            std::int32_t wheel;

            /**
             * Indicates how long a mouse button has been held for.
             *
             * For every update for which the button is held, its value is incremented by 1, up to 255.
             * If the button is not down, then the value is set to 0.
             *
             * Device state query rate is (generally) tied to framerate.
             * For high framerates, these button values reach the maximum quickly.
             */
            std::uint8_t button_state[8];

            /**
             * Indicates that a mouse button has been released.
             * A button vaue is true if and only if the button transitioned from a pressed to released state.
             * Effectively, this indicates the corresponding button state value has gone from non-zero to 0.
             */
            bool button_changed[8];
        }; static_assert(sizeof(MouseState) == 0x1C);

        /** Indicates that the #enumerated_devices are acquired. */
        bool devices_acquired; 

        /**
         * A switch that forces Halo to use various virtual input state buffers.
         *
         * This is never set true by Halo, but all the mechanisms that change input
         * behavior are still implemented. 
         * For instance, if this value is true, then enumerated_devices.direct_mouse_state
         * is ignored and mouse input is pulled from enumerated_devices.virtual_mouse_state.
         *
         * This value is set to false by default and when the input devices are unacquired.
         */
        bool use_virtual_input;

        /** The DirectInput interface. */
        LPDIRECTINPUT8 direct_input;

        /** Possibly last time of device update for each local player */
        UnknownStructure unknown0[4];
        
        PADDING(0x6D);
        PADDING(0x6D);

        /** 
          * Halo uses this for virtual keyboards and not for controls.
          * This structure gets used in the custom chat of Chimera.
          */
        struct {
            /** The index within keys to start reading input from. */
            std::int16_t read_index; 

            /** The number of inputs stored in keys. This does not change as the read index is incremented. */
            std::int16_t count;

            /** Stored key presses. */
            BufferedKey  keys[0x40];

        } buffered_keys;

        struct {
            /**
             * An interface for the system keyboard device.
             *
             * The data format for this interface is c_dfDIKeyboard.
             */
            LPDIRECTINPUTDEVICE direct_keyboard;

            /** 
             * An interface for the system mouse device.
             *
             * The data format for this interface is c_dfDIMouse2, for which
             * DIMOUSESTATE2 is appropriate for querying this device's buffered state.
             */
            LPDIRECTINPUTDEVICE direct_mouse;

            /** Granularity for the z-axis (mouse wheel). */
            std::uint32_t       direct_mouse_z_granularity;

            /** The mouse state as *translated* from direct_mouse->GetDeviceState(). */
            MouseState direct_mouse_state;

            /** 
             * Never written to by Halo, see #use_virtual_input. 
             * Presumably this used to be part of some harness for providing programmed control over the game.
             */
            MouseState virtual_mouse_state;

            /** The index of the next unoccupied joystick slot in #direct_joysticks. */
            std::int16_t        next_joystick;

            /** Interfaces for the registered gamepad/joystick devices. */
            LPDIRECTINPUTDEVICE direct_joysticks[8]; 

            /** Joystick state */
            std::uint8_t        direct_joysticks_states[0x240][8];
        } enumerated_devices;
        
        /** 
         * Get the input device and state globals
         * @return reference to the input globals
         */
        static InputGlobals &get();
    };
    static_assert(sizeof(InputGlobals) == 0x1470);
    
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
        /** Handle of the widget tag */
        TagHandle definition_tag_handle;

        /** Name of the widget */
        const char *name;

        /** Player controller index */
        std::uint16_t controller_index;

        /** Widget position */
        Engine::Point2DInt position;

        /** Widget type */
        TagDefinitions::UIWidgetType type;

        /** Some flags */
        bool visible;
        bool render_regardless_of_controller_index;
        bool never_receive_events;
        bool pauses_game_time; 

        bool deleted;
        PADDING(0x3);

        /** Ok I trust open sauce */
        std::uint32_t creation_process_start_time;

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

        union {
            struct {
                /** Text box content */
                const wchar_t *text;

                /** Last widget list element focused by cursor */
                std::uint16_t cursor_index;
                PADDING(0x2);
            } text_box;

            struct {
                PADDING(0x4);
                uint16_t element_index;
                PADDING(0x2);
                void *elements;
                uint16_t element_count;
                PADDING(0x2);
            } list;
        };

        /** Extended description widget */
        Widget *extended_description;

        void *userdata; // no idea about this
        PADDING(0x4);

        /** Background bitmap index */
        std::uint16_t bitmap_index;
        PADDING(0x2);

        PADDING(0x4);
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

    struct Controls {
        std::uint8_t jump;
        std::uint8_t switch_grenade;
        std::uint8_t action;
        std::uint8_t switch_weapon;

        std::uint8_t melee;
        std::uint8_t flashlight;
        std::uint8_t secondary_fire;
        std::uint8_t primary_fire;

        std::uint8_t menu_forward;
        std::uint8_t menu_back;
        std::uint8_t crouch;
        std::uint8_t zoom;

        std::uint8_t scores;
        std::uint8_t reload;
        std::uint8_t exchange_weapons;
        std::uint8_t all_chat;

        std::uint8_t team_chat;
        std::uint8_t vehicle_chat;
        PADDING(0x1);
        PADDING(0x1);

        PADDING(0x4);

        PADDING(0x1);
        PADDING(0x1);
        PADDING(0x1);
        std::uint8_t rules;

        std::uint8_t show_player_names;
        PADDING(0x3);

        float move_forward;
        float move_left;
        float aim_left;
        float aim_up;

        std::uint8_t controller_aim;
        PADDING(0x3);
    };

    enum struct KeyboardKey : std::uint8_t {
        ESCAPE, F1, F2, F3, F4, F5, 
        F6, F7, F8, F9, F10, F11, F12,
        PRINT_SCREEN,
        SCROLL_LOCK,
        PAUSE_BREAK,
        TILDE,
        TOP_1, TOP_2, TOP_3, TOP_4, TOP_5,
        TOP_6, TOP_7, TOP_8, TOP_9, TOP_0,
        TOP_MINUS,
        TOP_EQUALS,
        BACKSPACE,
        TAB,
        Q, W, E, R, T, Y, U, I, O, P,
        LEFT_BRACKET,
        RIGHT_BRACKET,
        BACK_SLASH,
        CAPS_LOCK, 
        A, S, D, F, G, H, J, K, L,
        SEMICOLON,
        APOSTROPHE,
        ENTER,
        LEFT_SHIFT, 
        Z, X, C, V, B, N, M,
        COMMA,
        PERIOD,
        FORWARD_SLASH,
        RIGHT_SHIFT,
        LEFT_CONTROL,
        WINDOWS_KEY,
        LEFT_ALT,
        SPACE,
        RIGHT_ALT,
        UNKNOWN,
        MENU,
        RIGHT_CONTROL,
        UP_ARROW,
        DOWN_ARROW,
        LEFT_ARROW,
        RIGHT_ARROW,
        INS,
        HOME,
        PAGE_UP,
        DEL,
        END,
        PAGE_DOWN,
        NUM_LOCK,
        NUM_STAR,
        NUM_FORWARD_SLASH,
        NUM_0, NUM_1, NUM_2, NUM_3, NUM_4, 
        NUM_5, NUM_6, NUM_7, NUM_8, NUM_9,
        NUM_MINUS,
        NUM_PLUS,
        NUM_ENTER,
        NUM_DECIMAL
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
     * Get the name of a give input device
     * @param device    Input device
     */
    BALLTZE_API std::string get_input_device_name(InputDevice device) noexcept;

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
     * Find a widget from a given widget definition
     * This is the function used by the game; it only returns the first coincidence
     * @param widget_definition     Widget definition tag handle of the widget to find
     * @param widget_base           Widget where to look
     * @return                      Pointer to widget if found, nullptr if not
     */
    BALLTZE_API Widget *find_widget(TagHandle widget_definition, Widget *widget_base = nullptr) noexcept;

    /**
     * Find widgets from a given widget definition
     * @param widget_definition     Widget definition tag handle of the widget to find
     * @param widget_base           Widget where to look
     * @return                      Vector of widgets
     */
    BALLTZE_API std::vector<Widget *> find_widgets(TagHandle widget_definition, bool first_match = true, Widget *widget_base = nullptr) noexcept;

    /**
     * Create a widget
     * @param widget_definition     Widget definition tag handle
     * @return                      Pointer to widget
     */
    BALLTZE_API Widget *create_widget(TagHandle widget_definition) noexcept;

    /**
     * Open a widget
     * @param widget_definition     Tag handle of widget definition
     * @param push_history          Push or not the current root widget to menu history
     * @return                      Pointer to the new widget
     */
    BALLTZE_API Widget *open_widget(TagHandle widget_definition, bool push_history = true) noexcept;

    /**
     * Close current root widget; return to the previous one in history
     */
    BALLTZE_API void close_widget() noexcept;

    /**
     * Replace a widget
     * @param widget                Widget to be replaced
     * @param widget_definition     Tag handle of the definition for the widget replace 
     * @return                      Pointer to the new widget
     */
    BALLTZE_API Widget *replace_widget(Widget *widget, TagHandle widget_definition) noexcept;

    /**
     * Reload a widget; replaces the widget with a new one with the same definition and state
     * @param widget    Widget to reload
     * @return          Pointer to the new widget
     */
    BALLTZE_API Widget *reload_widget(Widget *widget) noexcept;

    /**
     * Focus a widget
     * @param widget    Widget to be focused
     */
    BALLTZE_API void focus_widget(Widget *widget) noexcept;

    /**
     * Get the current root widget
     * @return  Pointer to the root widget
     */
    BALLTZE_API Widget *get_root_widget() noexcept;

    /**
     * Open the pause menu
     */
    BALLTZE_API void open_pause_menu() noexcept;

    /**
     * Gets the HUD globals
     * @return  The HUD globals tag data
     */
    BALLTZE_API HudGlobals &get_hud_globals();

    /**
     * Gets the size of a sprite in a bitmap.
     * @param bitmap                Bitmap tag data.
     * @param sequence_index        Sequence index of the bitmap.
     * @param sprite_index          The index of the sprite from the sequence.
     * @return                      Resolution of the sprite.
     * @throws std::runtime_error   If the bitmap tag handle is invalid.
     * @throws std::runtime_error   If the sequence index is invalid.
     * @throws std::runtime_error   If the sprite index is invalid.
     */
    BALLTZE_API Resolution get_bitmap_sprite_resolution(Engine::TagDefinitions::Bitmap *bitmap, std::size_t sequence_index, std::size_t sprite_index = 0);

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

    /**
     * Play a sound from a given tag
     * @param sound     Tag handle of the sound
     */
    BALLTZE_API void play_sound(TagHandle tag_sound);

    /**
     * Draw a bitmap in a rectangle
     * @param bitmap            Pointer to bitmap data to draw
     * @param bitmap_bounds     Bounds of the bitmap
     * @param rect              Screen rectangle where to draw the bitmap
     * @param color_mask        Color mask
     */
    BALLTZE_API void draw_bitmap_in_rect(TagDefinitions::BitmapData *bitmap, const Rectangle2D &rect, const Rectangle2D &bitmap_bounds, ColorARGBInt color_mask);

    /**
     * Draw a bitmap in a rectangle
     * @param bitmap            Pointer to bitmap data to draw
     * @param rect              Screen rectangle where to draw the bitmap
     * @param color_mask        Color mask
     */
    BALLTZE_API void draw_bitmap_in_rect(TagDefinitions::BitmapData *bitmap, const Rectangle2D &rect, ColorARGBInt color_mask);

    /**
     * Get the master volume.
     * @return Return the master volume.
     */
    BALLTZE_API std::uint8_t get_master_volume() noexcept;

    /**
     * Get the duration of a sound permutation
     * @param sound                 Pointer to sound tag data
     * @param permutation           Pointer to sound permutation 
     * @return                      Duration of the sound permutation in milliseconds
     */
    BALLTZE_API std::chrono::milliseconds get_sound_permutation_samples_duration(TagDefinitions::SoundPermutation *permutation);

    /**
     * Get the controls bindings
     * @return controls
     */
    BALLTZE_API Controls &get_controls() noexcept;

    /**
     * Get the keyboard keys
     * @return Pointer to keyboard keys array
     */
    BALLTZE_API bool *get_keyboard_keys() noexcept;

    /**
     * Set the console key binding
     */
    BALLTZE_API void set_console_key_binding(KeyboardKey key);
}

#endif
