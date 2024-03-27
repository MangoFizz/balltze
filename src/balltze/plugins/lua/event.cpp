// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/event.hpp>
#include <balltze/engine/tag_definitions.hpp>
#include <balltze/utils.hpp>
#include "../../logger.hpp"
#include "../plugin.hpp"
#include "../loader.hpp"
#include "helpers.hpp"

namespace Balltze::Event {
    extern std::string ip_address_int_to_string(std::uint32_t address_int) noexcept;
}

namespace Balltze::Plugins {
    using namespace Event;
    using handle_t = std::size_t;

    static std::size_t tick_event_count = 0;

    static void set_up_events_registry_table(lua_State *state) noexcept {
        auto balltze_module = Balltze::get_current_module();
        lua_pushlightuserdata(state, balltze_module);
        lua_gettable(state, LUA_REGISTRYINDEX);
        if(lua_isnil(state, -1)) {
            lua_pop(state, 1);
            logger.error("Could not find balltze Lua registry table");
        }

        lua_newtable(state);
        lua_setfield(state, -2, "events");

        lua_pop(state, 1);
    }

    static int lua_get_events_registry_table(lua_State *state) noexcept {
        auto balltze_module = Balltze::get_current_module();
        lua_pushlightuserdata(state, balltze_module);
        lua_gettable(state, LUA_REGISTRYINDEX);
        if(lua_isnil(state, -1)) {
            return 0;
        }
        lua_getfield(state, -1, "events");
        lua_remove(state, -2);
        return 1;
    }

    static void set_up_event_table(lua_State *state, const char *name, lua_CFunction add, lua_CFunction remove, lua_CFunction remove_all) noexcept {
        lua_newtable(state);
        lua_pushcfunction(state, add);
        lua_setfield(state, -2, "subscribe");
        lua_pushcfunction(state, remove);
        lua_setfield(state, -2, "removeListener");
        lua_pushcfunction(state, remove_all);
        lua_setfield(state, -2, "removeAllListeners");
        lua_setfield(state, -2, name);

        lua_get_events_registry_table(state);
        lua_newtable(state);
        lua_setfield(state, -2, name);
        lua_pop(state, 1);
    }

    static int add_event_listener(lua_State *state, const char *name, int function_index, EventPriority priority, lua_CFunction remove_function) noexcept {
        if(lua_isfunction(state, function_index)) {
            lua_get_events_registry_table(state);
            lua_getfield(state, -1, name);
            lua_remove(state, -2);
            
            auto priority_str = event_priority_to_string(priority);

            // Create priority table if it doesn't exist
            lua_getfield(state, -1, priority_str.c_str());
            if(lua_isnil(state, -1)) {
                lua_pop(state, 1);

                lua_newtable(state);
                lua_setfield(state, -2, priority_str.c_str());

                lua_getfield(state, -1, priority_str.c_str());
            }

            // Push function to the priority table
            std::string handle = std::to_string(++tick_event_count);
            lua_pushvalue(state, function_index);
            lua_setfield(state, -2, handle.c_str());

            // Pop priority table and event table
            lua_pop(state, 2);

            // Create listener handle
            lua_newtable(state);
            lua_pushinteger(state, tick_event_count);
            lua_setfield(state, -2, "_handle");
            lua_pushinteger(state, static_cast<int>(priority));
            lua_setfield(state, -2, "_priority");
            lua_pushstring(state, name);
            lua_setfield(state, -2, "_event");
            lua_pushcfunction(state, remove_function);
            lua_setfield(state, -2, "remove");

            return 1;
        }
        else {
            return luaL_error(state, "Invalid listener argument in function events.%s.subscribe.", name);
        }
    }

    static void remove_event_listener(lua_State *state, const char *name, int handle_index) noexcept {
        if(lua_istable(state, handle_index)) {
            lua_getfield(state, handle_index, "_handle");
            int handle = luaL_checkinteger(state, -1);
            lua_pop(state, 1);

            lua_getfield(state, handle_index, "_priority");
            auto priority = static_cast<EventPriority>(luaL_checkinteger(state, -1));
            lua_pop(state, 1);

            lua_getfield(state, handle_index, "_event");
            auto *event_name = luaL_checkstring(state, -1);
            lua_pop(state, 1);

            if(std::strcmp(name, event_name) == 0) {
                lua_get_events_registry_table(state);
                lua_getfield(state, -1, name);
                lua_remove(state, -2);

                auto priority_str = event_priority_to_string(priority);
                lua_getfield(state, -1, priority_str.c_str());
                lua_remove(state, -2);

                lua_pushnil(state);
                lua_setfield(state, -2, std::to_string(handle).c_str());

                lua_pop(state, 1);
            }
            else {
                luaL_error(state, "Invalid listener handle in function events.%s.remove_listener: tried to remove listener for event %s, but handle is for event %s.", name, name, event_name);
            }
        }
        else {
            luaL_error(state, "Invalid listener handle in function events.%s.remove_listener.", name);
        }
    }

    static void remove_all_event_listeners(lua_State *state, const char *name) noexcept {
        lua_get_events_registry_table(state);
        lua_pushnil(state);
        lua_setfield(state, -2, name);
        lua_pop(state, 1);
    }

    template<typename Event>
    static void create_event_data_table(lua_State *state, Event &context) noexcept {
        // Create context table in current state
        lua_newtable(state);
        lua_pushlightuserdata(state, const_cast<Event *>(&context));
        lua_setfield(state, -2, "_context");
        lua_pushboolean(state, context.cancelled());
        lua_setfield(state, -2, "cancelled");
        lua_pushcfunction(state, +[](lua_State *state) -> int {
            if(lua_istable(state, 1)) {
                lua_getfield(state, 1, "_context");
                auto *context = static_cast<Event *>(lua_touserdata(state, -1));
                lua_pop(state, 1);
                if(context->cancellable()) {
                    context->cancel();
                    lua_pushboolean(state, true);
                    lua_setfield(state, 1, "cancelled");
                    return 0;
                }
                else {
                    return luaL_error(state, "Attempted to cancel non-cancellable event.");
                }
            }
            else {
                return luaL_error(state, "Invalid context argument in function events.tick.cancel.");
            }
        });
        lua_setfield(state, -2, "cancel");
        lua_pushstring(state, event_time_to_string(context.time).c_str());
        lua_setfield(state, -2, "time");
    }

    void call_events_by_priority(lua_State *state, std::string name, EventPriority priority, int event_table_index) noexcept {
        lua_get_events_registry_table(state);
        lua_getfield(state, -1, name.c_str());
        lua_remove(state, -2);

        auto priority_str = event_priority_to_string(priority);
        lua_getfield(state, -1, priority_str.c_str());
        if(lua_isnil(state, -1)) {
            lua_pop(state, 2);
            return;
        }

        if(lua_istable(state, -1)) {
            lua_newtable(state);

            // Get all event listeners
            lua_pushnil(state);
            while(lua_next(state, -3)) {
                if(lua_isfunction(state, -1)) {
                    lua_rawseti(state, -3, lua_rawlen(state, -3) + 1);
                }
                else {
                    logger.error("Invalid event listener in events.tick: expected function, got {}.", lua_typename(state, lua_type(state, -1)));
                    lua_pop(state, 1);
                }
            }

            // Call all event listeners
            bool cancelled = false;
            int size = lua_rawlen(state, -1);
            for(int i = 1; i <= size; i++) {
                lua_rawgeti(state, -1, i);
                lua_pushvalue(state, event_table_index - 4);
                int res = lua_pcall(state, 1, 0, 0);
                if(res != LUA_OK) {
                    logger.error("Error in event listener in events.tick: {}.", lua_tostring(state, -1));
                    lua_pop(state, 1);
                }
            }
            
            lua_pop(state, 1);
        }
        else {
            logger.debug("Invalid event listener in events.tick: expected table, got {}.", lua_typename(state, lua_type(state, -1)));
        }

        lua_pop(state, 2);
    };

    static int lua_camera_event_args__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<CameraEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "camera") {
            lua_push_meta_engine_camera_data(state, *data->camera, true);
        }
        else if(field == "type") {
            lua_pushstring(state, camera_type_to_string(data->type).c_str());
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
        return 1;
    }

    static int lua_camera_event_args__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<CameraEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "camera") {
            return luaL_error(state, "Field camera is read-only");
        }
        else if(field == "type") {
            return luaL_error(state, "Field type is read-only");
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
    }

    static int lua_game_input_event_args__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<GameInputEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "device") {
            lua_pushstring(state, input_device_to_string(data->device).c_str());
        } 
        else if(field == "button") {
            switch(data->device) {
                case Engine::INPUT_DEVICE_MOUSE:
                    lua_pushstring(state, Engine::get_mouse_button_name(static_cast<Engine::MouseButton>(data->button.key_code)).c_str());
                    break;

                case Engine::INPUT_DEVICE_GAMEPAD:
                    lua_pushstring(state, Engine::get_gamepad_button_name(data->button.gamepad_button).c_str());
                    break;

                default:
                    lua_pushinteger(state, data->button.key_code);
                    break;
            }
        }
        else if(field == "mapped") {
            lua_pushboolean(state, data->mapped);
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
        return 1;
    }

    static int lua_game_input_event_args__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<GameInputEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "device") {
            return luaL_error(state, "Field device is read-only");
        }
        else if(field == "button") {
            return luaL_error(state, "Field button is read-only");
        }
        else if(field == "mapped") {
            return luaL_error(state, "Field mapped is read-only");
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
    }

    static std::string hud_hold_for_action_message_slice_to_string(HudHoldForActionMessageSlice slice) {
        switch(slice) {
            case HudHoldForActionMessageSlice::MESSAGE:
                return "message";
            case HudHoldForActionMessageSlice::BUTTON_NAME_LEFT_QUOTE:
                return "button_name_left_quote";
            case HudHoldForActionMessageSlice::BUTTON_NAME_RIGHT_QUOTE:
                return "button_name_right_quote";
            case HudHoldForActionMessageSlice::BUTTON_NAME:
                return "button_name";
            case HudHoldForActionMessageSlice::WEAPON_ICON:
                return "weapon_icon";
            default:
                return "unknown";
        }
    }

    static std::string button_type_to_string(HudHoldToActionMessageButton::Type type) {
        switch(type) {
            case HudHoldToActionMessageButton::Type::BUTTON:
                return "button";
            case HudHoldToActionMessageButton::Type::AXIS:
                return "axis";
            default:
                return "unknown";
        }
    }

    static std::string button_axis_direction_to_string(HudHoldToActionMessageButton::AxisDirection direction) {
        switch(direction) {
            case HudHoldToActionMessageButton::AxisDirection::POSITIVE:
                return "positive";
            case HudHoldToActionMessageButton::AxisDirection::NEGATIVE:
                return "negative";
            default:
                return "unknown";
        }
    }

    static int lua_push_hud_hold_to_action_message_button(lua_State *state, const HudHoldToActionMessageButton &button) noexcept {
        lua_newtable(state);
        lua_pushstring(state, input_device_to_string(button.device).c_str());
        lua_setfield(state, -2, "device");
        lua_pushstring(state, button_type_to_string(button.type).c_str());
        lua_setfield(state, -2, "type");
        lua_pushinteger(state, button.index);
        lua_setfield(state, -2, "index");
        lua_pushstring(state, button_axis_direction_to_string(button.axis_direction).c_str());
        lua_setfield(state, -2, "axisDirection");
        return 1;
    }

    static int lua_hud_hold_for_action_message_event_args__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<HudHoldForActionMessageArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "slice") {
            lua_pushstring(state, hud_hold_for_action_message_slice_to_string(data->slice).c_str());
        }
        else if(field == "offset") {
            lua_push_meta_engine_point2_d_int(state, data->offset);
        }
        else if(field == "color") {
            lua_push_meta_engine_color_a_r_g_b_int(state, const_cast<Engine::ColorARGBInt &>(data->color), true);
        }
        else if(field == "text") {
            lua_pushlightuserdata(state, data->text.data());
        }
        else if(field == "button") {
            if(data->button.has_value()) {
                lua_push_hud_hold_to_action_message_button(state, data->button.value());
            }
            else {
                lua_pushnil(state);
            }
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
        return 1;
    }

    static int lua_hud_hold_for_action_message_event_args__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<HudHoldForActionMessageArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "slice") {
            return luaL_error(state, "Field slice is read-only");
        }
        else if(field == "offset") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "color") {
            return luaL_error(state, "Field color is read-only");
        }
        else if(field == "text") {
            return luaL_error(state, "Invalid operation. Write the text using a write operation on the string address.");
        }
        else if(field == "button") {
            return luaL_error(state, "Field button is read-only");
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
    }

    static int lua_map_file_load_event_args__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<MapFileLoadEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "map_name") {
            lua_pushstring(state, data->map_name.c_str());
        }
        else if(field == "map_path") {
            lua_pushstring(state, data->map_path.c_str());
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
        return 1;
    }

    static int lua_map_file_load_event_args__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<MapFileLoadEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "map_name") {
            return luaL_error(state, "Field map_name is read-only");
        }
        else if(field == "map_path") {
            return luaL_error(state, "Field map_path is read-only");
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
    }

    static int lua_map_load_event_args__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<MapLoadEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "name") {
            lua_pushstring(state, data->name.c_str());
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
        return 1;
    }

    static int lua_map_load_event_args__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<MapLoadEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "name") {
            return luaL_error(state, "Field name is read-only");
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
    }

    static std::string chat_message_type_to_string(Engine::NetworkGameMessages::HudChatType type) {
        switch(type) {
            case Engine::NetworkGameMessages::HudChatType::NONE:
                return "none";
            case Engine::NetworkGameMessages::HudChatType::ALL:
                return "all";
            case Engine::NetworkGameMessages::HudChatType::TEAM:
                return "team";
            case Engine::NetworkGameMessages::HudChatType::VEHICLE:
                return "vehicle";
            default: 
                return "unknown";
        }
    }

    static int lua_network_game_chat_message_event_args__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<NetworkGameChatMessageEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "type") {
            lua_pushstring(state, chat_message_type_to_string(data->chat_message->msg_type).c_str());
        }
        else if(field == "player_id") {
            lua_pushinteger(state, data->chat_message->player_id);
        }
        else if(field == "message") {
            lua_pushlightuserdata(state, data->chat_message->message);
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
        return 1;
    }

    static int lua_network_game_chat_message_event_args__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<NetworkGameChatMessageEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "type") {
            return luaL_error(state, "Field type is read-only");
        }
        else if(field == "player_id") {
            return luaL_error(state, "Field player_id is read-only");
        }
        else if(field == "message") {
            return luaL_error(state, "Field message is read-only");
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
    }

    static int lua_object_damage_event_args__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<ObjectDamageEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "object") {
            lua_push_engine_object_handle(state, data->object);
        }
        else if(field == "damage_effect") {
            lua_push_engine_tag_handle(state, data->damage_effect);
        }
        else if(field == "multiplier") {
            lua_pushnumber(state, data->multiplier);
        }
        else if(field == "causer_player") {
            lua_push_engine_player_handle(state, data->causer_player);
        }
        else if(field == "causer_object") {
            lua_push_engine_object_handle(state, data->causer_object);
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
        return 1;
    }

    static int lua_object_damage_event_args__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<ObjectDamageEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "object") {
            return luaL_error(state, "Field object is read-only");
        }
        else if(field == "damage_effect") {
            return luaL_error(state, "Field damage_effect is read-only");
        }
        else if(field == "multiplier") {
            return luaL_error(state, "Field multiplier is read-only");
        }
        else if(field == "causer_player") {
            return luaL_error(state, "Field causer_player is read-only");
        }
        else if(field == "causer_object") {
            return luaL_error(state, "Field causer_object is read-only");
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
    }

    static int lua_rcon_message_event_args__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<RconMessageEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "message") {
            lua_pushstring(state, data->message.c_str());
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
        return 1;
    }

    static int lua_rcon_message_event_args__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<RconMessageEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "message") {
            return luaL_error(state, "Field message is read-only");
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
    }

    extern void lua_push_meta_engine_bitmap_data(lua_State *state, Engine::TagDefinitions::BitmapData &data) noexcept;

    static int lua_hud_element_bitmap_render_event_args__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<HUDElementBitmapRenderEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "vertices") {
            lua_push_meta_event_widget_render_vertices(state, *data->vertices);
        }
        else if(field == "bitmapData") {
            lua_push_meta_engine_bitmap_data(state, *data->bitmap_data);
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
        return 1;
    }

    static int lua_hud_element_bitmap_render_event_args__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<HUDElementBitmapRenderEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "vertices") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "bitmapData") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
    }

    static int lua_ui_widget_background_render_event_args__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<UIWidgetBackgroundRenderEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "vertices") {
            lua_push_meta_event_widget_render_vertices(state, *data->vertices);
        }
        else if(field == "widget") {
            lua_push_meta_engine_widget(state, *data->widget);
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
        return 1;
    }

    static int lua_ui_widget_background_render_event_args__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<UIWidgetBackgroundRenderEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "vertices") {
            return luaL_error(state, "Invalid operation");
        }
        else if(field == "widget") {
            return luaL_error(state, "Invalid operation");
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
    }

    static int lua_server_connect_event_args__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<ServerConnectEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "address") {
            lua_pushstring(state, ip_address_int_to_string(data->address).c_str());
        }
        else if(field == "port") {
            lua_pushinteger(state, data->port);
        }
        else if(field == "password") {
            lua_pushlightuserdata(state, const_cast<wchar_t *>(data->password.data()));
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
        return 1;
    }

    static int lua_server_connect_event_args__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<ServerConnectEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "address") {
            return luaL_error(state, "Field address is read-only");
        }
        else if(field == "port") {
            return luaL_error(state, "Field port is read-only");
        }
        else if(field == "password") {
            return luaL_error(state, "Field password is read-only");
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
    }

    extern void lua_push_meta_engine_sound(lua_State *state, Engine::TagDefinitions::Sound &data) noexcept;
    extern void lua_push_meta_engine_sound_permutation(lua_State *state, Engine::TagDefinitions::SoundPermutation &data) noexcept;

    static int lua_sound_playback_event_args__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<SoundPlaybackEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "sound") {
            lua_push_meta_engine_sound(state, *const_cast<Engine::TagDefinitions::Sound *>(data->sound));
        }
        else if(field == "permutation") {
            lua_push_meta_engine_sound_permutation(state, *const_cast<Engine::TagDefinitions::SoundPermutation *>(data->permutation));
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
        return 1;
    }

    static int lua_sound_playback_event_args__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<SoundPlaybackEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "sound") {
            return luaL_error(state, "Field sound is read-only");
        }
        else if(field == "permutation") {
            return luaL_error(state, "Field permutation is read-only");
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
    }

    static int lua_tick_event_args__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<TickEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "tick_count") {
            lua_pushinteger(state, data->tick_count);
        }
        else if(field == "delta_time_ms") {
            lua_pushinteger(state, data->delta_time_ms);
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
        return 1;
    }

    static int lua_tick_event_args__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<TickEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "tick_count") {
            return luaL_error(state, "Field tick_count is read-only");
        }
        else if(field == "delta_time_ms") {
            return luaL_error(state, "Field delta_time_ms is read-only");
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
    }

    static int lua_ui_widget_create_event_args__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<UIWidgetCreateEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "widget") {
            if(data->widget) {
                lua_push_meta_engine_widget(state, *const_cast<Engine::Widget *>(data->widget));
            }
            else {
                lua_pushnil(state);
            }
        }
        else if(field == "definition_tag_handle") {
            lua_push_engine_tag_handle(state, data->definition_tag_handle);
        }
        else if(field == "is_root_widget") {
            lua_pushboolean(state, data->is_root_widget);
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
        return 1;
    }

    static int lua_ui_widget_create_event_args__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<UIWidgetCreateEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "widget") {
            return luaL_error(state, "Field widget is read-only");
        }
        else if(field == "definition_tag_handle") {
            return luaL_error(state, "Field definition_tag_handle is read-only");
        }
        else if(field == "is_root_widget") {
            return luaL_error(state, "Field is_root_widget is read-only");
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
    }

    static int lua_ui_widget_back_event_args__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<UIWidgetEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "widget") {
            lua_push_meta_engine_widget(state, *const_cast<Engine::Widget *>(data->widget));
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
        return 1;
    }

    static int lua_ui_widget_back_event_args__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<UIWidgetEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "widget") {
            return luaL_error(state, "Field widget is read-only");
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
    }

    static int lua_ui_widget_focus_event_args__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<UIWidgetEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "widget") {
            lua_push_meta_engine_widget(state, *const_cast<Engine::Widget *>(data->widget));
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
        return 1;
    }

    static int lua_ui_widget_focus_event_args__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<UIWidgetEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "widget") {
            return luaL_error(state, "Field widget is read-only");
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
    }

    static int lua_ui_widget_accept_event_args__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<UIWidgetEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "widget") {
            lua_push_meta_engine_widget(state, *const_cast<Engine::Widget *>(data->widget));
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
        return 1;
    }

    static int lua_ui_widget_accept_event_args__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<UIWidgetEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "widget") {
            return luaL_error(state, "Field widget is read-only");
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
    }

    static int lua_ui_widget_sound_event_args__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<UIWidgetSoundEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "sound") {
            lua_pushstring(state, Engine::get_widget_navigation_sound_name(data->sound).c_str());
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
        return 1;
    }

    static int lua_ui_widget_sound_event_args__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<UIWidgetSoundEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "sound") {
            return luaL_error(state, "Field sound is read-only");
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
    }

    static std::string ui_widget_list_tab_type_to_string(const UIWidgetListTabType &type) {
        switch(type) {
            case UIWidgetListTabType::TAB_THRU_ITEM_LIST_ITEMS_NEXT_VERTICAL:
                return "tab_thru_item_list_items_next_vertical";
            case UIWidgetListTabType::TAB_THRU_ITEM_LIST_ITEMS_NEXT_HORIZONTAL:
                return "tab_thru_item_list_items_next_horizontal";
            case UIWidgetListTabType::TAB_THRU_ITEM_LIST_ITEMS_PREV_VERTICAL:
                return "tab_thru_item_list_items_prev_vertical";
            case UIWidgetListTabType::TAB_THRU_ITEM_LIST_ITEMS_PREV_HORIZONTAL:
                return "tab_thru_item_list_items_prev_horizontal";
            case UIWidgetListTabType::TAB_THRU_CHILDREN_NEXT_VERTICAL:
                return "tab_thru_children_next_vertical";
            case UIWidgetListTabType::TAB_THRU_CHILDREN_NEXT_HORIZONTAL:
                return "tab_thru_children_next_horizontal";
            case UIWidgetListTabType::TAB_THRU_CHILDREN_PREV:
                return "tab_thru_children_prev";
            default:
                return "unknown";
        }
    }

    static int lua_ui_widget_list_tab_event_args__index(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<UIWidgetListTabEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "widget_list") {
            lua_push_meta_engine_widget(state, *const_cast<Engine::Widget *>(data->widget_list));
        }
        else if(field == "tab_type") {
            lua_pushstring(state, ui_widget_list_tab_type_to_string(data->tab).c_str());
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
        return 1;
    }

    static int lua_ui_widget_list_tab_event_args__newindex(lua_State *state) noexcept {
        auto *data = lua_from_meta_object<UIWidgetListTabEventArgs>(state, 1); 
        auto *key = lua_tostring(state, 2);
        if(key == nullptr) {  
            return luaL_error(state, "Invalid key type"); 
        } 
        
        std::string field = key;
        if(field == "widget_list") {
            return luaL_error(state, "Field widget_list is read-only");
        }
        else if(field == "tab_type") {
            return luaL_error(state, "Field tab_type is read-only");
        }
        else {
            return luaL_error(state, "Invalid field %s", field.c_str());
        }
    }

    #define SET_BASIC_EVENT_FUNCTIONS(eventName, eventTable) \
        static int lua_event_##eventName##_remove_listener(lua_State *state) noexcept { \
            auto *plugin = get_lua_plugin(state); \
            if(plugin) { \
                int args = lua_gettop(state); \
                if(args == 1) { \
                    remove_event_listener(state, eventTable, 1); \
                    return 0; \
                } \
                else { \
                    return luaL_error(state, "Invalid number of arguments in function events." eventTable ".remove_listener."); \
                } \
            } \
            else { \
                logger.warning("Could not get plugin for lua state."); \
                return luaL_error(state, "Unknown plugin."); \
            } \
        } \
        \
        static int lua_event_##eventName##_subscribe(lua_State *state) noexcept { \
            auto *plugin = get_lua_plugin(state); \
            if(plugin) { \
                int args = lua_gettop(state); \
                if(args == 2) { \
                    auto priority_str = luaL_checkstring(state, 2); \
                    try { \
                        auto priority = event_priority_from_string(priority_str); \
                        return add_event_listener(state, eventTable, 1, priority, lua_event_##eventName##_remove_listener); \
                    } \
                    catch(const std::invalid_argument &e) { \
                        return luaL_error(state, "Invalid priority argument in function events." eventTable ".subscribe: %s.", priority_str); \
                    } \
                } \
                else { \
                    return luaL_error(state, "Invalid number of arguments in function events." eventTable ".subscribe."); \
                } \
            } \
            else { \
                logger.warning("Could not get plugin for lua state."); \
                return luaL_error(state, "Unknown plugin."); \
            } \
        } \
        \
        static int lua_event_##eventName##_remove_all_listeners(lua_State *state) noexcept { \
            auto *plugin = get_lua_plugin(state); \
            if(plugin) { \
                int args = lua_gettop(state); \
                if(args == 0) { \
                    remove_all_event_listeners(state, eventTable); \
                    return 0; \
                } \
                else { \
                    return luaL_error(state, "Invalid number of arguments in function events." eventTable ".remove_all_listeners."); \
                } \
            } \
            else { \
                logger.warning("Could not get plugin for lua state."); \
                return luaL_error(state, "Unknown plugin."); \
            } \
        } \

    #define SET_POPULATE_EVENT_FUNCTION(eventClass, eventName, eventTable) \
        static void populate_##eventName##_events(eventClass &context, EventPriority priority) noexcept { \
            auto plugins = get_lua_plugins(); \
            for(auto *&plugin : plugins) { \
                auto *state = plugin->state(); \
                create_event_data_table(state, context); \
                lua_push_meta_object(state, context.args, lua_##eventName##_event_args__index, lua_##eventName##_event_args__newindex); \
                lua_setfield(state, -2, "args"); \
                call_events_by_priority(state, eventTable, priority, -1); \
                lua_pop(state, 1); \
            } \
        }

    #define SET_POPULATE_EVENT_NO_ARGS_FUNCTION(eventClass, eventName, eventTable) \
        static void populate_##eventName##_events(eventClass &context, EventPriority priority) noexcept { \
            auto plugins = get_lua_plugins(); \
            for(auto *&plugin : plugins) { \
                auto *state = plugin->state(); \
                create_event_data_table(state, context); \
                call_events_by_priority(state, eventTable, priority, -1); \
                lua_pop(state, 1); \
            } \
        }

    #define SET_EVENT_FUNCTIONS(eventClass, eventName, eventTable) \
        SET_BASIC_EVENT_FUNCTIONS(eventName, eventTable) \
        SET_POPULATE_EVENT_FUNCTION(eventClass, eventName, eventTable)

    #define SET_EVENT_FUNCTIONS_NO_ARGS(eventClass, eventName, eventTable) \
        SET_BASIC_EVENT_FUNCTIONS(eventName, eventTable) \
        SET_POPULATE_EVENT_NO_ARGS_FUNCTION(eventClass, eventName, eventTable)

    SET_EVENT_FUNCTIONS(CameraEvent, camera, "camera");
    SET_EVENT_FUNCTIONS_NO_ARGS(FrameEvent, frame, "frame");
    SET_EVENT_FUNCTIONS(GameInputEvent, game_input, "gameInput");
    SET_EVENT_FUNCTIONS(HudHoldForActionMessageEvent, hud_hold_for_action_message, "hudHoldForActionMessage");
    SET_EVENT_FUNCTIONS(MapFileLoadEvent, map_file_load, "mapFileLoad");
    SET_EVENT_FUNCTIONS(MapLoadEvent, map_load, "mapLoad");
    SET_EVENT_FUNCTIONS(NetworkGameChatMessageEvent, network_game_chat_message, "networkGameChatMessage");
    SET_EVENT_FUNCTIONS(ObjectDamageEvent, object_damage, "objectDamage");
    SET_EVENT_FUNCTIONS(RconMessageEvent, rcon_message, "rconMessage");
    SET_EVENT_FUNCTIONS_NO_ARGS(UIRenderEvent, ui_render, "uiRender");
    SET_EVENT_FUNCTIONS_NO_ARGS(HUDRenderEvent, hud_render, "hudRender");
    SET_EVENT_FUNCTIONS_NO_ARGS(PostCarnageReportRenderEvent, post_carnage_report_render, "postCarnageReportRender");
    SET_EVENT_FUNCTIONS(HUDElementBitmapRenderEvent, hud_element_bitmap_render, "hudElementBitmapRender");
    SET_EVENT_FUNCTIONS(UIWidgetBackgroundRenderEvent, ui_widget_background_render, "uiWidgetBackgroundRender");
    SET_EVENT_FUNCTIONS_NO_ARGS(NavPointsRenderEvent, navpoints_render, "navpointsRender");
    SET_EVENT_FUNCTIONS(ServerConnectEvent, server_connect, "serverConnect");
    SET_EVENT_FUNCTIONS(SoundPlaybackEvent, sound_playback, "soundPlayback");
    SET_EVENT_FUNCTIONS(TickEvent, tick, "tick");
    SET_EVENT_FUNCTIONS(UIWidgetCreateEvent, ui_widget_create, "uiWidgetCreate");
    SET_EVENT_FUNCTIONS(UIWidgetBackEvent, ui_widget_back, "uiWidgetBack");
    SET_EVENT_FUNCTIONS(UIWidgetFocusEvent, ui_widget_focus, "uiWidgetFocus");
    SET_EVENT_FUNCTIONS(UIWidgetAcceptEvent, ui_widget_accept, "uiWidgetAccept");
    SET_EVENT_FUNCTIONS(UIWidgetSoundEvent, ui_widget_sound, "uiWidgetSound");
    SET_EVENT_FUNCTIONS(UIWidgetListTabEvent, ui_widget_list_tab, "uiWidgetListTab");

    #undef SET_EVENT_FUNCTIONS

    void lua_set_event_table(lua_State *state) noexcept {
        set_up_events_registry_table(state);

        // Events functions table
        lua_newtable(state);
        set_up_event_table(state, "camera", lua_event_camera_subscribe, lua_event_camera_remove_listener, lua_event_camera_remove_all_listeners);
        set_up_event_table(state, "frame", lua_event_frame_subscribe, lua_event_frame_remove_listener, lua_event_frame_remove_all_listeners);
        set_up_event_table(state, "gameInput", lua_event_game_input_subscribe, lua_event_game_input_remove_listener, lua_event_game_input_remove_all_listeners);
        set_up_event_table(state, "hudHoldForActionMessage", lua_event_hud_hold_for_action_message_subscribe, lua_event_hud_hold_for_action_message_remove_listener, lua_event_hud_hold_for_action_message_remove_all_listeners);
        set_up_event_table(state, "mapFileLoad", lua_event_map_file_load_subscribe, lua_event_map_file_load_remove_listener, lua_event_map_file_load_remove_all_listeners);
        set_up_event_table(state, "mapLoad", lua_event_map_load_subscribe, lua_event_map_load_remove_listener, lua_event_map_load_remove_all_listeners);
        set_up_event_table(state, "networkGameChatMessage", lua_event_network_game_chat_message_subscribe, lua_event_network_game_chat_message_remove_listener, lua_event_network_game_chat_message_remove_all_listeners);
        set_up_event_table(state, "objectDamage", lua_event_object_damage_subscribe, lua_event_object_damage_remove_listener, lua_event_object_damage_remove_all_listeners);
        set_up_event_table(state, "rconMessage", lua_event_rcon_message_subscribe, lua_event_rcon_message_remove_listener, lua_event_rcon_message_remove_all_listeners);
        set_up_event_table(state, "uiRender", lua_event_ui_render_subscribe, lua_event_ui_render_remove_listener, lua_event_ui_render_remove_all_listeners);
        set_up_event_table(state, "hudRender", lua_event_hud_render_subscribe, lua_event_hud_render_remove_listener, lua_event_hud_render_remove_all_listeners);
        set_up_event_table(state, "postCarnageReportRender", lua_event_post_carnage_report_render_subscribe, lua_event_post_carnage_report_render_remove_listener, lua_event_post_carnage_report_render_remove_all_listeners);
        set_up_event_table(state, "hudElementBitmapRender", lua_event_hud_element_bitmap_render_subscribe, lua_event_hud_element_bitmap_render_remove_listener, lua_event_hud_element_bitmap_render_remove_all_listeners);
        set_up_event_table(state, "uiWidgetBackgroundRender", lua_event_ui_widget_background_render_subscribe, lua_event_ui_widget_background_render_remove_listener, lua_event_ui_widget_background_render_remove_all_listeners);
        set_up_event_table(state, "navpointsRender", lua_event_navpoints_render_subscribe, lua_event_navpoints_render_remove_listener, lua_event_navpoints_render_remove_all_listeners);
        set_up_event_table(state, "serverConnect", lua_event_server_connect_subscribe, lua_event_server_connect_remove_listener, lua_event_server_connect_remove_all_listeners);
        set_up_event_table(state, "soundPlayback", lua_event_sound_playback_subscribe, lua_event_sound_playback_remove_listener, lua_event_sound_playback_remove_all_listeners);
        set_up_event_table(state, "tick", lua_event_tick_subscribe, lua_event_tick_remove_listener, lua_event_tick_remove_all_listeners);
        set_up_event_table(state, "uiWidgetCreate", lua_event_ui_widget_create_subscribe, lua_event_ui_widget_create_remove_listener, lua_event_ui_widget_create_remove_all_listeners);
        set_up_event_table(state, "uiWidgetBack", lua_event_ui_widget_back_subscribe, lua_event_ui_widget_back_remove_listener, lua_event_ui_widget_back_remove_all_listeners);
        set_up_event_table(state, "uiWidgetFocus", lua_event_ui_widget_focus_subscribe, lua_event_ui_widget_focus_remove_listener, lua_event_ui_widget_focus_remove_all_listeners);
        set_up_event_table(state, "uiWidgetAccept", lua_event_ui_widget_accept_subscribe, lua_event_ui_widget_accept_remove_listener, lua_event_ui_widget_accept_remove_all_listeners);
        set_up_event_table(state, "uiWidgetSound", lua_event_ui_widget_sound_subscribe, lua_event_ui_widget_sound_remove_listener, lua_event_ui_widget_sound_remove_all_listeners);
        set_up_event_table(state, "uiWidgetListTab", lua_event_ui_widget_list_tab_subscribe, lua_event_ui_widget_list_tab_remove_listener, lua_event_ui_widget_list_tab_remove_all_listeners);
        lua_setfield(state, -2, "event");

        #define SET_UP_EVENT_LISTENERS(event, populate) \
            static auto event##_event_highest = event::subscribe([](event &context) { \
                populate(context, EVENT_PRIORITY_HIGHEST); \
            }, EVENT_PRIORITY_HIGHEST); \
            \
            static auto event##_event_above_default = event::subscribe([](event &context) { \
                populate(context, EVENT_PRIORITY_ABOVE_DEFAULT); \
            }, EVENT_PRIORITY_ABOVE_DEFAULT); \
            \
            static auto event##_event_default = event::subscribe([](event &context) { \
                populate(context, EVENT_PRIORITY_DEFAULT); \
            }, EVENT_PRIORITY_DEFAULT); \
            \
            static auto event##_event_lowest = event::subscribe([](event &context) { \
                populate(context, EVENT_PRIORITY_LOWEST); \
            }, EVENT_PRIORITY_LOWEST);

        SET_UP_EVENT_LISTENERS(CameraEvent, populate_camera_events);
        SET_UP_EVENT_LISTENERS(FrameEvent, populate_frame_events);
        SET_UP_EVENT_LISTENERS(GameInputEvent, populate_game_input_events);
        SET_UP_EVENT_LISTENERS(HudHoldForActionMessageEvent, populate_hud_hold_for_action_message_events);
        SET_UP_EVENT_LISTENERS(MapFileLoadEvent, populate_map_file_load_events);
        SET_UP_EVENT_LISTENERS(MapLoadEvent, populate_map_load_events);
        SET_UP_EVENT_LISTENERS(NetworkGameChatMessageEvent, populate_network_game_chat_message_events);
        SET_UP_EVENT_LISTENERS(ObjectDamageEvent, populate_object_damage_events);
        SET_UP_EVENT_LISTENERS(RconMessageEvent, populate_rcon_message_events);
        SET_UP_EVENT_LISTENERS(UIRenderEvent, populate_ui_render_events);
        SET_UP_EVENT_LISTENERS(HUDRenderEvent, populate_hud_render_events);
        SET_UP_EVENT_LISTENERS(PostCarnageReportRenderEvent, populate_post_carnage_report_render_events);
        SET_UP_EVENT_LISTENERS(HUDElementBitmapRenderEvent, populate_hud_element_bitmap_render_events);
        SET_UP_EVENT_LISTENERS(UIWidgetBackgroundRenderEvent, populate_ui_widget_background_render_events);
        SET_UP_EVENT_LISTENERS(NavPointsRenderEvent, populate_navpoints_render_events);
        SET_UP_EVENT_LISTENERS(ServerConnectEvent, populate_server_connect_events);
        SET_UP_EVENT_LISTENERS(SoundPlaybackEvent, populate_sound_playback_events);
        SET_UP_EVENT_LISTENERS(TickEvent, populate_tick_events);
        SET_UP_EVENT_LISTENERS(UIWidgetCreateEvent, populate_ui_widget_create_events);
        SET_UP_EVENT_LISTENERS(UIWidgetBackEvent, populate_ui_widget_back_events);
        SET_UP_EVENT_LISTENERS(UIWidgetFocusEvent, populate_ui_widget_focus_events);
        SET_UP_EVENT_LISTENERS(UIWidgetAcceptEvent, populate_ui_widget_accept_events);
        SET_UP_EVENT_LISTENERS(UIWidgetSoundEvent, populate_ui_widget_sound_events);
        SET_UP_EVENT_LISTENERS(UIWidgetListTabEvent, populate_ui_widget_list_tab_events);

        #undef SET_UP_EVENT_LISTENERS
    }
}
