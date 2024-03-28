// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__HELPERS_HPP
#define BALLTZE__PLUGINS__LUA__HELPERS_HPP

#include <functional>
#include <typeinfo>
#include <lua.hpp>
#include <balltze/plugin.hpp>
#include <balltze/event.hpp>
#include <balltze/engine.hpp>

namespace Balltze::Plugins {
    void lua_create_functions_table(lua_State *state, const char *name, const luaL_Reg *functions) noexcept;
    
    Engine::ColorARGB lua_to_color_argb(lua_State *state, int index);
    
    std::string engine_edition_to_string(Engine::EngineEdition edition);
    Engine::EngineEdition engine_edition_from_string(const std::string &edition);
    
    std::string cache_file_engine_to_string(Engine::CacheFileEngine engine);
    Engine::CacheFileEngine cache_file_engine_from_string(const std::string &engine);
    
    std::string map_game_type_to_string(Engine::MapGameType type);
    Engine::MapGameType map_game_type_from_string(const std::string &type);
    
    std::string network_game_server_type_to_string(Engine::NetworkGameServerType type);
    Engine::NetworkGameServerType network_game_server_type_from_string(const std::string &type);

    std::string network_game_server_game_type_to_string(Engine::NetworkGameType gametype);
    Engine::NetworkGameType network_game_server_game_type_from_string(const std::string &gametype);

    void lua_push_engine_matrix(lua_State *state, Engine::Matrix &matrix) noexcept;
    std::shared_ptr<Engine::Matrix> lua_to_engine_matrix(lua_State *state, int index) noexcept;

    void lua_push_engine_resolution(lua_State *state, Engine::Resolution &resolution) noexcept;

    std::string object_type_to_string(Engine::ObjectType type);
    Engine::ObjectType object_type_from_string(const std::string &type);

    std::string object_network_role_to_string(Engine::ObjectNetworkRole role);
    Engine::ObjectNetworkRole object_network_role_from_string(const std::string &role);

    std::string object_attachment_type_to_string(Engine::BaseObjectAttachmentType type);
    Engine::BaseObjectAttachmentType object_attachment_type_from_string(const std::string &type);

    std::string network_game_multiplayer_team_to_string(Engine::NetworkGameMultiplayerTeam team);
    Engine::NetworkGameMultiplayerTeam network_game_multiplayer_team_from_string(const std::string &team);

    std::string camera_type_to_string(Engine::CameraType camera_type);
    Engine::CameraType camera_type_from_string(const std::string &camera_type);

    Engine::Point2DInt lua_to_point2_d_int(lua_State *state, int index);
    Engine::Point3D lua_to_point3_d(lua_State *state, int index);
    Engine::ColorARGBInt lua_to_color_a_r_g_b_int(lua_State *state, int index);

    std::string input_device_to_string(Engine::InputDevice device);
    Engine::InputDevice input_device_from_string(const std::string &device);

    void lua_push_engine_tag(lua_State *state, Engine::Tag *tag) noexcept;
    void lua_push_meta_engine_tag(lua_State *state, Engine::Tag &tag, bool read_only = false) noexcept;

    void lua_push_engine_resource_handle(lua_State *state, Engine::ResourceHandle *handle) noexcept;
    void lua_push_engine_resource_handle(lua_State *state, const Engine::ResourceHandle &handle) noexcept;
    void lua_push_engine_object_handle(lua_State *state, const Engine::ObjectHandle &handle) noexcept;
    void lua_push_engine_tag_handle(lua_State *state, const Engine::TagHandle &handle) noexcept;
    void lua_push_engine_player_handle(lua_State *state, const Engine::PlayerHandle &handle) noexcept;

    std::string unit_throwing_grenade_state_to_string(Engine::UnitThrowingGrenadeState state);
    std::string unit_animation_state_to_string(Engine::UnitAnimationState state);
    std::string unit_replacement_animation_state_to_string(Engine::UnitReplacementAnimationState state);
    std::string unit_overlay_animation_state_to_string(Engine::UnitOverlayAnimationState state);
    std::string unit_base_seat_to_string(Engine::UnitBaseSeat seat);
    std::string unit_speech_priority_to_string(Engine::UnitSpeechPriority priority);
    std::string unit_scream_type_to_string(Engine::UnitScreamType type);
    std::string ai_communication_type_to_string(Engine::AiCommunicationType type);
    std::string biped_movement_state_to_string(Engine::BipedMovementState state);
    std::string weapon_state_to_string(Engine::WeaponState state);
    std::string weapon_magazine_state_to_string(Engine::WeaponMagazineState state);
    std::string network_color_to_string(Engine::NetworkPlayerColor color);
    std::string player_objective_mode_to_string(Engine::PlayerObjectiveMode mode);

    Engine::UnitThrowingGrenadeState unit_throwing_grenade_state_from_string(const std::string &state);
    Engine::UnitAnimationState unit_animation_state_from_string(const std::string &state);
    Engine::UnitReplacementAnimationState unit_replacement_animation_state_from_string(const std::string &state);
    Engine::UnitOverlayAnimationState unit_overlay_animation_state_from_string(const std::string &state);
    Engine::UnitBaseSeat unit_base_seat_from_string(const std::string &seat);
    Engine::UnitSpeechPriority unit_speech_priority_from_string(const std::string &priority);
    Engine::UnitScreamType unit_scream_type_from_string(const std::string &type);
    Engine::AiCommunicationType ai_communication_type_from_string(const std::string &type);
    Engine::BipedMovementState biped_movement_state_from_string(const std::string &state);
    Engine::WeaponState weapon_state_from_string(const std::string &state);
    Engine::WeaponMagazineState weapon_magazine_state_from_string(const std::string &state);
    Engine::NetworkPlayerColor network_color_from_string(const std::string &color);
    Engine::PlayerObjectiveMode player_objective_mode_from_string(const std::string &mode);

    void lua_push_meta_engine_color_a_r_g_b_int(lua_State *state, Engine::ColorARGBInt &color, bool read_only = false) noexcept;
    void lua_push_meta_engine_tag_dependency(lua_State *state, Engine::TagDependency &dependency, bool read_only = false) noexcept;
    void lua_push_meta_engine_point2_d(lua_State *state, Engine::Point2D &point, bool read_only = false) noexcept;
    void lua_push_meta_engine_point3_d(lua_State *state, Engine::Point3D &point, bool read_only = false) noexcept;
    void lua_push_meta_engine_tag_data_offset(lua_State *state, Engine::TagDataOffset &offset, bool read_only = false) noexcept;
    void lua_push_meta_engine_color_a_r_g_b(lua_State *state, Engine::ColorARGB &color, bool read_only = false) noexcept;
    void lua_push_meta_engine_rectangle2_d(lua_State *state, Engine::Rectangle2D &rectangle, bool read_only = false) noexcept;
    void lua_push_meta_engine_rectangle2_d_f(lua_State *state, Engine::Rectangle2DF &rectangle, bool read_only = false) noexcept;
    void lua_push_meta_engine_point2_d_int(lua_State *state, Engine::Point2DInt &point, bool read_only = false) noexcept;
    void lua_push_meta_engine_euler2_d(lua_State *state, Engine::Euler2D &euler, bool read_only = false) noexcept;
    void lua_push_meta_engine_euler3_d(lua_State *state, Engine::Euler3D &euler, bool read_only = false) noexcept;
    void lua_push_meta_engine_euler3_d_p_y_r(lua_State *state, Engine::Euler3D &euler, bool read_only = false) noexcept;
    void lua_push_meta_engine_vector2_d(lua_State *state, Engine::Vector2D &vector, bool read_only = false) noexcept;
    void lua_push_meta_engine_vector3_d(lua_State *state, Engine::Vector3D &vector, bool read_only = false) noexcept;
    void lua_push_meta_engine_color_r_g_b(lua_State *state, Engine::ColorRGB &color, bool read_only = false) noexcept;
    void lua_push_meta_engine_quaternion(lua_State *state, Engine::Quaternion &quaternion, bool read_only = false) noexcept;
    void lua_push_meta_engine_plane3_d(lua_State *state, Engine::Plane3D &plane, bool read_only = false) noexcept;
    void lua_push_meta_engine_plane2_d(lua_State *state, Engine::Plane2D &plane, bool read_only = false) noexcept;
    void lua_push_meta_engine_widget(lua_State *state, Engine::Widget &widget, bool read_only = false) noexcept;
    void lua_push_meta_engine_camera_data(lua_State *state, Engine::CameraData &data, bool read_only = false) noexcept;
    void lua_push_meta_engine_rotation_matrix(lua_State *state, Engine::RotationMatrix &matrix, bool read_only = false) noexcept;
    void lua_push_meta_engine_model_node(lua_State *state, Engine::ModelNode &node, bool read_only = false) noexcept;
    
    void lua_push_meta_engine_object_flags(lua_State *state, Engine::BaseObjectFlags &flags, bool read_only = false) noexcept;
    void lua_push_meta_engine_object_network(lua_State *state, Engine::BaseObjectNetwork &network, bool read_only = false) noexcept;
    void lua_push_meta_engine_scenario_location(lua_State *state, Engine::ScenarioLocation &location, bool read_only = false) noexcept;
    void lua_push_meta_engine_object_vitals_flags(lua_State *state, Engine::BaseObjectVitalsFlags &flags, bool read_only = false) noexcept;
    void lua_push_meta_engine_object_vitals(lua_State *state, Engine::BaseObjectVitals &flags, bool read_only = false) noexcept;
    void lua_push_meta_engine_object_attachments_data(lua_State *state, Engine::BaseObjectAttachmentsData &data, bool read_only = false) noexcept;
    void lua_push_meta_engine_object_region_destroyeds(lua_State *state, Engine::BaseObjectRegionDestroyeds &destroyeds, bool read_only = false) noexcept;
    void lua_push_meta_engine_object_block_reference(lua_State *state, Engine::BaseObjectBlockReference &reference, bool read_only = false) noexcept;
    void lua_push_meta_engine_object(lua_State *state, Engine::BaseObject &object, bool read_only = false) noexcept;

    void lua_push_meta_engine_unit_recent_damager(lua_State *state, Engine::UnitRecentDamager &damager, bool read_only = false) noexcept;
    void lua_push_meta_engine_unit_flags(lua_State *state, Engine::UnitFlags &flags, bool read_only = false) noexcept;
    void lua_push_meta_engine_unit_control_flags(lua_State *state, Engine::UnitControlFlags &flags, bool read_only = false) noexcept;
    void lua_push_meta_engine_unit_animation_state_data(lua_State *state, Engine::UnitAnimationStateData &data, bool read_only = false) noexcept;
    void lua_push_meta_engine_unit_animation_flags(lua_State *state, Engine::UnitAnimationFlags &data, bool read_only = false) noexcept;
    void lua_push_meta_engine_unit_animation_data(lua_State *state, Engine::UnitAnimationData &data, bool read_only = false) noexcept;
    void lua_push_meta_engine_ai_communication_packet(lua_State *state, Engine::AiCommunicationPacket &packet, bool read_only = false) noexcept;
    void lua_push_meta_engine_unit_speech(lua_State *state, Engine::UnitSpeech &speech, bool read_only = false) noexcept;
    void lua_push_meta_engine_unit_speech_data(lua_State *state, Engine::UnitSpeech &speech, bool read_only = false) noexcept;
    void lua_push_meta_engine_unit_control_data(lua_State *state, Engine::UnitControlData &data, bool read_only = false) noexcept;
    void lua_push_meta_engine_unit_object(lua_State *state, Engine::UnitObject &object, bool read_only = false) noexcept;
    void lua_push_meta_engine_biped_flags(lua_State *state, Engine::BipedFlags &flags, bool read_only = false) noexcept;
    void lua_push_meta_engine_biped_network_delta(lua_State *state, Engine::BipedNetworkDelta &delta, bool read_only = false) noexcept;
    void lua_push_meta_engine_biped_network(lua_State *state, Engine::BipedNetwork &network, bool read_only = false) noexcept;
    void lua_push_meta_engine_biped_object(lua_State *state, Engine::BipedObject &object, bool read_only = false) noexcept;
    void lua_push_meta_engine_vehicle_flags(lua_State *state, Engine::VehicleFlags &flags, bool read_only = false) noexcept;
    void lua_push_meta_engine_vehicle_network_data(lua_State *state, Engine::VehicleNetworkData &data, bool read_only = false) noexcept;
    void lua_push_meta_engine_vehicle_network(lua_State *state, Engine::VehicleNetwork &network, bool read_only = false) noexcept;
    void lua_push_meta_engine_vehicle_object(lua_State *state, Engine::VehicleObject &object, bool read_only = false) noexcept;

    void lua_push_meta_engine_item_object(lua_State *state, Engine::ItemObject &object, bool read_only = false) noexcept;
    void lua_push_meta_engine_garbage_object(lua_State *state, Engine::GarbageObject &object, bool read_only = false) noexcept;
    void lua_push_meta_engine_weapon_trigger(lua_State *state, Engine::WeaponTrigger &trigger, bool read_only = false) noexcept;
    void lua_push_meta_engine_weapon_magazine(lua_State *state, Engine::WeaponMagazine &magazine, bool read_only = false) noexcept;
    void lua_push_meta_engine_weapon_reload_start_data(lua_State *state, Engine::WeaponReloadStartData &data, bool read_only = false) noexcept;
    void lua_push_meta_engine_weapon_network_data(lua_State *state, Engine::WeaponNetworkData &data, bool read_only = false) noexcept;
    void lua_push_meta_engine_weapon_network(lua_State *state, Engine::WeaponNetwork &network, bool read_only = false) noexcept;
    void lua_push_meta_engine_weapon_object(lua_State *state, Engine::WeaponObject &object, bool read_only = false) noexcept;
    void lua_push_meta_engine_equipment_network_data(lua_State *state, Engine::EquipmentNetworkData &data, bool read_only = false) noexcept;
    void lua_push_meta_engine_equipment_network(lua_State *state, Engine::EquipmentNetwork &network, bool read_only = false) noexcept;
    void lua_push_meta_engine_equipment_object(lua_State *state, Engine::EquipmentObject &object, bool read_only = false) noexcept;
    void lua_push_meta_engine_projectile_object_flags(lua_State *state, Engine::ProjectileObjectFlags &flags, bool read_only = false) noexcept;
    void lua_push_meta_engine_projectile_network_data(lua_State *state, Engine::ProjectileNetworkData &data, bool read_only = false) noexcept;
    void lua_push_meta_engine_projectile_network(lua_State *state, Engine::ProjectileNetwork &network, bool read_only = false) noexcept;
    void lua_push_meta_engine_projectile_object(lua_State *state, Engine::ProjectileObject &object, bool read_only = false) noexcept;

    void lua_push_meta_engine_device_object_state(lua_State *state, Engine::DeviceObjectState &object_state, bool read_only = false) noexcept;
    void lua_push_meta_engine_device_object(lua_State *state, Engine::DeviceObject &object, bool read_only = false) noexcept;
    void lua_push_meta_engine_device_machine_object_flags(lua_State *state, Engine::DeviceMachineObjectFlags &flags, bool read_only = false) noexcept;
    void lua_push_meta_engine_device_machine_object(lua_State *state, Engine::DeviceMachineObject &object, bool read_only = false) noexcept;
    void lua_push_meta_engine_device_control_object_flags(lua_State *state, Engine::DeviceControlObjectFlags &flags, bool read_only = false) noexcept;
    void lua_push_meta_engine_device_control_object(lua_State *state, Engine::DeviceControlObject &object, bool read_only = false) noexcept;
    void lua_push_meta_engine_device_light_fixture_object(lua_State *state, Engine::DeviceLightFixtureObject &object, bool read_only = false) noexcept;

    void lua_push_meta_engine_player_multiplayer_statistics(lua_State *state, Engine::PlayerMultiplayerStatistics &statistics, bool read_only = false) noexcept;
    void lua_push_meta_engine_player(lua_State *state, Engine::Player &player, bool read_only = false) noexcept;

    void lua_push_meta_event_widget_render_vertex(lua_State *state, Event::UIWidgetRenderVertices::Vertex &vertex, bool read_only = false) noexcept;
    void lua_push_meta_event_widget_render_vertices(lua_State *state, Event::UIWidgetRenderVertices &vertices, bool read_only = false) noexcept;

    int lua_meta_object_read_only__newindex(lua_State *state) noexcept;
    int lua_meta_object__eq(lua_State *state) noexcept;

    template<typename T>
    void lua_push_meta_object(lua_State *state, T &elem, lua_CFunction index, lua_CFunction newindex, bool read_only = false) noexcept {
        // Just to be sure
        if(&elem == nullptr) {
            lua_pushnil(state);
            return;
        }

        // Create table 
        lua_newtable(state);
        lua_pushlightuserdata(state, reinterpret_cast<void *>(const_cast<T *>(&elem)));
        lua_setfield(state, -2, "_data");
        lua_pushinteger(state, typeid(T).hash_code());
        lua_setfield(state, -2, "_type");

        // Create metatable 
        lua_newtable(state);
        lua_pushcfunction(state, index);
        lua_setfield(state, -2, "__index");
        if(read_only) {
            lua_pushcfunction(state, lua_meta_object_read_only__newindex);
        } else {
            lua_pushcfunction(state, newindex);
        }
        lua_setfield(state, -2, "__newindex");
        lua_pushcfunction(state, lua_meta_object__eq);
        lua_setfield(state, -2, "__eq");

        // Set metatable for the table
        lua_setmetatable(state, -2);
    }

    template<typename T>
    T *lua_from_meta_object(lua_State *state, int index) noexcept {
        if(!lua_istable(state, index)) {
            return nullptr;
        }
        T *elem = nullptr;
        lua_getfield(state, index, "_type");
        if(lua_isinteger(state, -1) && lua_tointeger(state, -1) == typeid(T).hash_code()) {
            lua_getfield(state, index, "_data");
            if(lua_islightuserdata(state, -1)) {
                elem = reinterpret_cast<T *>(lua_touserdata(state, -1));
            }
        }
        lua_pop(state, 2);
        return elem;
    }

    template<typename T>
    int lua_engine_tag_reflexive__index(lua_State *state, std::function<void (lua_State *, T &)> push_function) noexcept {
        lua_getfield(state, 1, "_data"); 
        auto *elem = static_cast<Engine::TagReflexive<T> *>(lua_touserdata(state, -1));
        lua_pop(state, 1); 
        auto key = luaL_checkinteger(state, 2);
        if (key < 0 || key >= elem->count) {
            return luaL_error(state, "Index out of bounds");
        }
        push_function(state, elem->offset[key]);
    }
}   

#endif
