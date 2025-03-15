// SPDX-License-Identifier: GPL-3.0-only

#include <impl/saved_games/player_profile.h>
#include <impl/network/network_game.h>
#include "../helpers/luacstruct.hpp"
#include "../helpers/enum.hpp"

namespace Balltze::Plugins::Lua {
    static void define_ringworld_saved_games_player_color_setting(lua_State *state) noexcept {
        luacs_newenum(state, NetworkGamePlayerColor);
        luacs_enum_declare_value(state, "white", NETWORK_GAME_PLAYER_COLOR_WHITE);
        luacs_enum_declare_value(state, "black", NETWORK_GAME_PLAYER_COLOR_BLACK);
        luacs_enum_declare_value(state, "red", NETWORK_GAME_PLAYER_COLOR_RED);
        luacs_enum_declare_value(state, "blue", NETWORK_GAME_PLAYER_COLOR_BLUE);
        luacs_enum_declare_value(state, "gray", NETWORK_GAME_PLAYER_COLOR_GRAY);
        luacs_enum_declare_value(state, "yellow", NETWORK_GAME_PLAYER_COLOR_YELLOW);
        luacs_enum_declare_value(state, "green", NETWORK_GAME_PLAYER_COLOR_GREEN);
        luacs_enum_declare_value(state, "pink", NETWORK_GAME_PLAYER_COLOR_PINK);
        luacs_enum_declare_value(state, "purple", NETWORK_GAME_PLAYER_COLOR_PURPLE);
        luacs_enum_declare_value(state, "cyan", NETWORK_GAME_PLAYER_COLOR_CYAN);
        luacs_enum_declare_value(state, "cobalt", NETWORK_GAME_PLAYER_COLOR_COBALT);
        luacs_enum_declare_value(state, "orange", NETWORK_GAME_PLAYER_COLOR_ORANGE);
        luacs_enum_declare_value(state, "teal", NETWORK_GAME_PLAYER_COLOR_TEAL);
        luacs_enum_declare_value(state, "sage", NETWORK_GAME_PLAYER_COLOR_SAGE);
        luacs_enum_declare_value(state, "brown", NETWORK_GAME_PLAYER_COLOR_BROWN);
        luacs_enum_declare_value(state, "tan", NETWORK_GAME_PLAYER_COLOR_TAN);
        luacs_enum_declare_value(state, "maroon", NETWORK_GAME_PLAYER_COLOR_MAROON);
        luacs_enum_declare_value(state, "salmon", NETWORK_GAME_PLAYER_COLOR_SALMON);
        luacs_enum_declare_value(state, "random", NETWORK_GAME_PLAYER_COLOR_RANDOM);
        publish_enum(state, "Engine", "savedGames", "playerColors", -1); 
        lua_pop(state, 1);
    }

    static void define_ringworld_saved_games_player_details(lua_State *state) noexcept {
        luacs_newstruct(state, PlayerProfileDetails);
        luacs_declare_field(state, LUACS_TBYTEARRAY, NULL, "playerName", 1, OFFSET_OF(PlayerProfileDetails, player_name), sizeof(((PlayerProfileDetails *)0)->player_name), 0);
        luacs_enum_field(state, PlayerProfileDetails, NetworkGamePlayerColor, player_color, 0);
        lua_pop(state, 1); 
    }

    static void define_ringworld_saved_games_player_controls_action(lua_State *state) noexcept {
        luacs_newenum(state, PlayerControlsAction);
        luacs_enum_declare_value(state, "jump", CONTROLS_ACTION_JUMP);
        luacs_enum_declare_value(state, "switchGrenade", CONTROLS_ACTION_SWITCH_GRENADE);
        luacs_enum_declare_value(state, "action", CONTROLS_ACTION_ACTION);
        luacs_enum_declare_value(state, "switchWeapon", CONTROLS_ACTION_SWITCH_WEAPON);
        luacs_enum_declare_value(state, "meleeAttack", CONTROLS_ACTION_MELEE_ATTACK);
        luacs_enum_declare_value(state, "flashlight", CONTROLS_ACTION_FLASHLIGHT);
        luacs_enum_declare_value(state, "throwGrenade", CONTROLS_ACTION_THROW_GRENADE);
        luacs_enum_declare_value(state, "fireWeapon", CONTROLS_ACTION_FIRE_WEAPON);
        luacs_enum_declare_value(state, "menuAccept", CONTROLS_ACTION_MENU_ACCEPT);
        luacs_enum_declare_value(state, "menuBack", CONTROLS_ACTION_MENU_BACK);
        luacs_enum_declare_value(state, "crouch", CONTROLS_ACTION_CROUCH);
        luacs_enum_declare_value(state, "scopeZoom", CONTROLS_ACTION_SCOPE_ZOOM);
        luacs_enum_declare_value(state, "showScores", CONTROLS_ACTION_SHOW_SCORES);
        luacs_enum_declare_value(state, "reload", CONTROLS_ACTION_RELOAD);
        luacs_enum_declare_value(state, "exchangeWeapon", CONTROLS_ACTION_EXCHANGE_WEAPON);
        luacs_enum_declare_value(state, "say", CONTROLS_ACTION_SAY);
        luacs_enum_declare_value(state, "sayToTeam", CONTROLS_ACTION_SAY_TO_TEAM);
        luacs_enum_declare_value(state, "sayToVehicle", CONTROLS_ACTION_SAY_TO_VEHICLE);
        luacs_enum_declare_value(state, "screenshot", CONTROLS_ACTION_SCREENSHOT);
        luacs_enum_declare_value(state, "moveForward", CONTROLS_ACTION_MOVE_FORWARD);
        luacs_enum_declare_value(state, "moveBackward", CONTROLS_ACTION_MOVE_BACKWARD);
        luacs_enum_declare_value(state, "moveLeft", CONTROLS_ACTION_MOVE_LEFT);
        luacs_enum_declare_value(state, "moveRight", CONTROLS_ACTION_MOVE_RIGHT);
        luacs_enum_declare_value(state, "lookUp", CONTROLS_ACTION_LOOK_UP);
        luacs_enum_declare_value(state, "lookDown", CONTROLS_ACTION_LOOK_DOWN);
        luacs_enum_declare_value(state, "lookLeft", CONTROLS_ACTION_LOOK_LEFT);
        luacs_enum_declare_value(state, "lookRight", CONTROLS_ACTION_LOOK_RIGHT);
        luacs_enum_declare_value(state, "showRules", CONTROLS_ACTION_SHOW_RULES);
        luacs_enum_declare_value(state, "showPlayerNames", CONTROLS_ACTION_SHOW_PLAYER_NAMES);
        publish_enum(state, "Engine", "savedGames", "playerControlsActions", -1); 
        lua_pop(state, 1);
    }

    void define_ringworld_player_controls_keyboard_settings(lua_State *state) noexcept {
        luacs_newstruct(state, PlayerControlsKeyboardSettings);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, escape, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, f1, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, f2, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, f3, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, f4, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, f5, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, f6, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, f7, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, f8, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, f9, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, f10, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, f11, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, f12, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, printscreen, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, scroll_lock, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, pause_break, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, grave, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, num_row_1, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, num_row_2, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, num_row_3, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, num_row_4, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, num_row_5, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, num_row_6, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, num_row_7, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, num_row_8, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, num_row_9, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, num_row_0, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, en_dash, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, equals, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, backspace, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, tab, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, q, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, w, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, e, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, r, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, t, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, y, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, u, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, i, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, o, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, p, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, bracket_l, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, bracket_r, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, backslash, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, caps_lock, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, a, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, s, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, d, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, f, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, g, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, h, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, j, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, k, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, l, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, semi_colon, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, apostrophe, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, enter, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, shift_l, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, z, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, x, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, c, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, v, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, b, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, n, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, m, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, comma, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, period, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, slash, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, r_shift, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, l_ctrl, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, l_win, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, l_alt, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, space, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, r_alt, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, r_win, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, menu, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, r_ctrl, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, up_arrow, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, down_arrow, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, left_arrow, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, right_arrow, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, insert, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, home, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, pg_up, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, del, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, end, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, pg_down, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, num_lock, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, kp_divide, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, kp_multiply, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, keypad_0, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, keypad_1, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, keypad_2, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, keypad_3, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, keypad_4, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, keypad_5, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, keypad_6, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, keypad_7, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, keypad_8, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, keypad_9, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, keypad_minus, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, keypad_plus, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, unknown_1, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, keypad_decimal, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, unknown_2, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, unknown_3, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, unknown_4, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, unknown_5, 0);
        luacs_enum_field(state, PlayerControlsKeyboardSettings, PlayerControlsAction, unknown_6, 0);
        publish_enum(state, "Engine", "savedGames", "playerControlsKeyboardSettings", -1);
        lua_pop(state, 1);
    }

    void define_ringworld_player_controls_mouse_settings(lua_State *state) noexcept {
        luacs_newstruct(state, PlayerControlsMouseSettings);
        luacs_enum_field(state, PlayerControlsMouseSettings, PlayerControlsAction, left_button, 0);
        luacs_enum_field(state, PlayerControlsMouseSettings, PlayerControlsAction, middle_button, 0);
        luacs_enum_field(state, PlayerControlsMouseSettings, PlayerControlsAction, right_button, 0);
        luacs_enum_field(state, PlayerControlsMouseSettings, PlayerControlsAction, button_4, 0);
        luacs_enum_field(state, PlayerControlsMouseSettings, PlayerControlsAction, button_5, 0);
        luacs_enum_field(state, PlayerControlsMouseSettings, PlayerControlsAction, button_6, 0);
        luacs_enum_field(state, PlayerControlsMouseSettings, PlayerControlsAction, button_7, 0);
        luacs_enum_field(state, PlayerControlsMouseSettings, PlayerControlsAction, button_8, 0);
        luacs_enum_field(state, PlayerControlsMouseSettings, PlayerControlsAction, horizontal_axis_minus, 0);
        luacs_enum_field(state, PlayerControlsMouseSettings, PlayerControlsAction, horizontal_axis_plus, 0);
        luacs_enum_field(state, PlayerControlsMouseSettings, PlayerControlsAction, vertical_axis_minus, 0);
        luacs_enum_field(state, PlayerControlsMouseSettings, PlayerControlsAction, vertical_axis_plus, 0);
        luacs_enum_field(state, PlayerControlsMouseSettings, PlayerControlsAction, wheel_minus, 0);
        luacs_enum_field(state, PlayerControlsMouseSettings, PlayerControlsAction, wheel_plus, 0);
        lua_pop(state, 1);
    }

    void define_ringworld_player_controls_gamepad_buttons_settings(lua_State *state) noexcept {
        luacs_newstruct(state, PlayerControlsGamepadButtonsSettings);
        luacs_enum_field(state, PlayerControlsGamepadButtonsSettings, PlayerControlsAction, button_0, 0);
        luacs_enum_field(state, PlayerControlsGamepadButtonsSettings, PlayerControlsAction, button_1, 0);
        luacs_enum_field(state, PlayerControlsGamepadButtonsSettings, PlayerControlsAction, button_2, 0);
        luacs_enum_field(state, PlayerControlsGamepadButtonsSettings, PlayerControlsAction, button_3, 0);
        luacs_enum_field(state, PlayerControlsGamepadButtonsSettings, PlayerControlsAction, button_4, 0);
        luacs_enum_field(state, PlayerControlsGamepadButtonsSettings, PlayerControlsAction, button_5, 0);
        luacs_enum_field(state, PlayerControlsGamepadButtonsSettings, PlayerControlsAction, button_6, 0);
        luacs_enum_field(state, PlayerControlsGamepadButtonsSettings, PlayerControlsAction, button_7, 0);
        luacs_enum_field(state, PlayerControlsGamepadButtonsSettings, PlayerControlsAction, button_8, 0);
        luacs_enum_field(state, PlayerControlsGamepadButtonsSettings, PlayerControlsAction, button_9, 0);
        luacs_enum_field(state, PlayerControlsGamepadButtonsSettings, PlayerControlsAction, button_10, 0);
        publish_enum(state, "Engine", "savedGames", "playerControlsGamepadButtonsSettings", -1);
        lua_pop(state, 1);
    }

    void define_ringworld_player_controls_gamepad_axis_settings(lua_State *state) noexcept {
        luacs_newstruct(state, PlayerControlsGamepadAxisSettings);
        luacs_enum_field(state, PlayerControlsGamepadAxisSettings, PlayerControlsAction, axis_1_pos, 0);
        luacs_enum_field(state, PlayerControlsGamepadAxisSettings, PlayerControlsAction, axis_1_neg, 0);
        luacs_enum_field(state, PlayerControlsGamepadAxisSettings, PlayerControlsAction, axis_2_pos, 0);
        luacs_enum_field(state, PlayerControlsGamepadAxisSettings, PlayerControlsAction, axis_2_neg, 0);
        luacs_enum_field(state, PlayerControlsGamepadAxisSettings, PlayerControlsAction, axis_3_pos, 0);
        luacs_enum_field(state, PlayerControlsGamepadAxisSettings, PlayerControlsAction, axis_3_neg, 0);
        luacs_enum_field(state, PlayerControlsGamepadAxisSettings, PlayerControlsAction, axis_4_pos, 0);
        luacs_enum_field(state, PlayerControlsGamepadAxisSettings, PlayerControlsAction, axis_4_neg, 0);
        luacs_enum_field(state, PlayerControlsGamepadAxisSettings, PlayerControlsAction, axis_5_pos, 0);
        luacs_enum_field(state, PlayerControlsGamepadAxisSettings, PlayerControlsAction, axis_5_neg, 0);
        luacs_enum_field(state, PlayerControlsGamepadAxisSettings, PlayerControlsAction, axis_6_pos, 0);
        luacs_enum_field(state, PlayerControlsGamepadAxisSettings, PlayerControlsAction, axis_6_neg, 0);
        lua_pop(state, 1);
    }

    void define_ringworld_player_controls_gamepad_dpad_settings(lua_State *state) noexcept {
        luacs_newstruct(state, PlayerControlsGamepadDPadSettings);
        luacs_enum_field(state, PlayerControlsGamepadDPadSettings, PlayerControlsAction, DirPadUp, 0);
        luacs_enum_field(state, PlayerControlsGamepadDPadSettings, PlayerControlsAction, DirPadRight, 0);
        luacs_enum_field(state, PlayerControlsGamepadDPadSettings, PlayerControlsAction, DirPadDown, 0);
        luacs_enum_field(state, PlayerControlsGamepadDPadSettings, PlayerControlsAction, DirPadLeft, 0);
        lua_pop(state, 1);
    }

    void define_ringworld_direct_input_buttons(lua_State *state) noexcept {
        luacs_newenum(state, DirectInputButtons);
        luacs_enum_declare_value(state, "button0", DINPUT_BUTTON_0);
        luacs_enum_declare_value(state, "button1", DINPUT_BUTTON_1);
        luacs_enum_declare_value(state, "button2", DINPUT_BUTTON_2);
        luacs_enum_declare_value(state, "button3", DINPUT_BUTTON_3);
        luacs_enum_declare_value(state, "button4", DINPUT_BUTTON_4);
        luacs_enum_declare_value(state, "button5", DINPUT_BUTTON_5);
        luacs_enum_declare_value(state, "button6", DINPUT_BUTTON_6);
        luacs_enum_declare_value(state, "button7", DINPUT_BUTTON_7);
        luacs_enum_declare_value(state, "button8", DINPUT_BUTTON_8);
        luacs_enum_declare_value(state, "button9", DINPUT_BUTTON_9);
        luacs_enum_declare_value(state, "button10", DINPUT_BUTTON_10);
        luacs_enum_declare_value(state, "button11", DINPUT_BUTTON_11);
        luacs_enum_declare_value(state, "button12", DINPUT_BUTTON_12);
        luacs_enum_declare_value(state, "button13", DINPUT_BUTTON_13);
        luacs_enum_declare_value(state, "button14", DINPUT_BUTTON_14);
        publish_enum(state, "Engine", "savedGames", "directInputButtons", -1);
        lua_pop(state, 1);
    }

    void define_ringworld_player_controls_gamepad_menu_settings(lua_State *state) noexcept {
        luacs_newstruct(state, PlayerControlsGamepadMenuSettings);
        luacs_enum_field(state, PlayerControlsGamepadMenuSettings, DirectInputButtons, menu_accept, 0);
        luacs_enum_field(state, PlayerControlsGamepadMenuSettings, DirectInputButtons, menu_back, 0);
        lua_pop(state, 1);
    }

    void define_ringworld_player_controls_gamepad_settings(lua_State *state) noexcept {
        luacs_newstruct(state, PlayerControlsGamepadSettings);
        luacs_nested_array_field(state, PlayerControlsGamepadSettings, PlayerControlsGamepadButtonsSettings, gamepad_buttons, 0);
        luacs_nested_array_field(state, PlayerControlsGamepadSettings, PlayerControlsGamepadMenuSettings, gamepad_menu_buttons, 0);
        luacs_nested_array_field(state, PlayerControlsGamepadSettings, PlayerControlsGamepadAxisSettings, gamepad_axis_set, 0);
        luacs_nested_array_field(state, PlayerControlsGamepadSettings, PlayerControlsGamepadDPadSettings, gamepad_dpad_buttons, 0);
        lua_pop(state, 1);
    }

    void define_ringworld_player_control_settings(lua_State *state) noexcept {
        luacs_newstruct(state, PlayerControlsSettings);
        luacs_unsigned_field(state, PlayerControlsSettings, invert_vertical_axis, 0);
        luacs_nested_field(state, PlayerControlsSettings, PlayerControlsKeyboardSettings, keyboard_bindings, 0);
        luacs_nested_field(state, PlayerControlsSettings, PlayerControlsMouseSettings, mouse_bindings, 0);
        luacs_nested_field(state, PlayerControlsSettings, PlayerControlsGamepadSettings, gamepad_bindings, 0);
        luacs_unsigned_field(state, PlayerControlsSettings, horizontal_mouse_sensitivity, 0);
        luacs_unsigned_field(state, PlayerControlsSettings, vertical_mouse_sensitivity, 0);
        luacs_unsigned_array_field(state, PlayerControlsSettings, gamepad_horizontal_sensitivities, 0);
        luacs_unsigned_array_field(state, PlayerControlsSettings, gamepad_vertical_sensitivities, 0);
        lua_pop(state, 1);
    }

    void define_ringworld_player_video_settings(lua_State *state) noexcept {
        luacs_newstruct(state, PlayerVideoSettings);
        luacs_unsigned_field(state, PlayerVideoSettings, resolution_width, 0);
        luacs_unsigned_field(state, PlayerVideoSettings, resolution_height, 0);
        luacs_unsigned_field(state, PlayerVideoSettings, refresh_rate, 0);
        luacs_unsigned_field(state, PlayerVideoSettings, frame_rate, 0);
        luacs_unsigned_field(state, PlayerVideoSettings, specular, 0);
        luacs_unsigned_field(state, PlayerVideoSettings, shadows, 0);
        luacs_unsigned_field(state, PlayerVideoSettings, decals, 0);
        luacs_unsigned_field(state, PlayerVideoSettings, particles, 0);
        luacs_unsigned_field(state, PlayerVideoSettings, texture_quality, 0);
        luacs_unsigned_field(state, PlayerVideoSettings, gamma, 0);
        lua_pop(state, 1);
    }

    void define_ringworld_player_audio_settings(lua_State *state) noexcept {
        luacs_newstruct(state, PlayerAudioSettings);
        luacs_unsigned_field(state, PlayerAudioSettings, master_volume, 0);
        luacs_unsigned_field(state, PlayerAudioSettings, effects_volume, 0);
        luacs_unsigned_field(state, PlayerAudioSettings, music_volume, 0);
        luacs_unsigned_field(state, PlayerAudioSettings, enable_eax, 0);
        luacs_unsigned_field(state, PlayerAudioSettings, enable_hardware_acceleration, 0);
        luacs_unsigned_field(state, PlayerAudioSettings, sound_quality, 0);
        luacs_unsigned_field(state, PlayerAudioSettings, audio_variety, 0);
        lua_pop(state, 1);
    }

    void define_ringworld_player_network_settings(lua_State *state) noexcept {
        luacs_newstruct(state, PlayerNetworkSettings);
        luacs_unsigned_field(state, PlayerNetworkSettings, max_players, 0);
        luacs_unsigned_field(state, PlayerNetworkSettings, connection_type, 0);
        luacs_unsigned_field(state, PlayerNetworkSettings, server_port, 0);
        luacs_unsigned_field(state, PlayerNetworkSettings, client_port, 0);
        lua_pop(state, 1);
    }

    void define_ringworld_player_gamepad_info(lua_State *state) noexcept {
        luacs_newstruct(state, PlayerGamepadInfo);
        luacs_unsigned_field(state, PlayerGamepadInfo, vendor_id, 0);
        luacs_unsigned_field(state, PlayerGamepadInfo, product_id, 0);
        luacs_unsigned_field(state, PlayerGamepadInfo, dupe_id, 0);
        lua_pop(state, 1);
    }

    void define_ringworld_player_profile(lua_State *state) noexcept {
        luacs_newstruct(state, PlayerProfile);
        luacs_nested_field(state, PlayerProfile, PlayerProfileDetails, player_details, 0);
        luacs_nested_field(state, PlayerProfile, PlayerControlsSettings, control_settings, 0);
        luacs_nested_field(state, PlayerProfile, PlayerVideoSettings, video_settings, 0);
        luacs_nested_field(state, PlayerProfile, PlayerAudioSettings, audio_settings, 0);
        luacs_nested_field(state, PlayerProfile, PlayerNetworkSettings, network_settings, 0);
        luacs_nested_array_field(state, PlayerProfile, PlayerGamepadInfo, gamepad_info, 0);
        luacs_unsigned_field(state, PlayerProfile, crc32_hash, 0);
        lua_pop(state, 1);
    }

    void push_ringworld_player_profile(lua_State *state, PlayerProfile *profile) noexcept {
        luacs_newobject(state, PlayerProfile, profile);
    }

    void define_ringworld_saved_games(lua_State *state) noexcept {
        define_ringworld_saved_games_player_color_setting(state);
        define_ringworld_saved_games_player_details(state);
        define_ringworld_saved_games_player_controls_action(state);
        define_ringworld_player_controls_keyboard_settings(state);
        define_ringworld_player_controls_mouse_settings(state);
        define_ringworld_player_controls_gamepad_buttons_settings(state);
        define_ringworld_player_controls_gamepad_axis_settings(state);
        define_ringworld_player_controls_gamepad_dpad_settings(state);
        define_ringworld_direct_input_buttons(state);
        define_ringworld_player_controls_gamepad_menu_settings(state);
        define_ringworld_player_controls_gamepad_settings(state);
        define_ringworld_player_control_settings(state);
        define_ringworld_player_video_settings(state);
        define_ringworld_player_audio_settings(state);
        define_ringworld_player_network_settings(state);
        define_ringworld_player_gamepad_info(state);
        define_ringworld_player_profile(state);
    }
}
