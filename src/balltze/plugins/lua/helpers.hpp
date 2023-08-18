// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__PLUGINS__LUA__HELPERS_HPP
#define BALLTZE__PLUGINS__LUA__HELPERS_HPP

#include <functional>
#include <typeinfo>
#include <lua.hpp>
#include <balltze/plugin.hpp>
#include <balltze/engine.hpp>

namespace Balltze::Plugins {
    void lua_create_functions_table(lua_State *state, const char *name, const luaL_Reg *functions) noexcept;
    VersionNumber lua_check_version_number(lua_State* state, int index);
    
    Engine::ColorARGB lua_to_color_argb(lua_State *state, int index);
    
    std::string engine_edition_to_string(Engine::EngineEdition edition);
    Engine::EngineEdition engine_edition_from_string(const std::string &edition);
    
    std::string cache_file_engine_to_string(Engine::CacheFileEngine engine);
    Engine::CacheFileEngine cache_file_engine_from_string(const std::string &engine);
    
    std::string map_game_type_to_string(Engine::MapGameType type);
    Engine::MapGameType map_game_type_from_string(const std::string &type);
    
    std::string server_type_to_string(Engine::ServerType type);
    Engine::ServerType server_type_from_string(const std::string &type);

    std::string server_game_type_to_string(Engine::Gametype gametype);
    Engine::Gametype server_game_type_from_string(const std::string &gametype);

    void lua_push_engine_matrix(lua_State *state, Engine::Matrix &matrix) noexcept;
    std::shared_ptr<Engine::Matrix> lua_to_engine_matrix(lua_State *state, int index) noexcept;

    void lua_push_engine_resolution(lua_State *state, Engine::Resolution &resolution) noexcept;

    std::string object_type_to_string(Engine::ObjectType type);
    Engine::ObjectType object_type_from_string(const std::string &type);

    std::string object_network_role_to_string(Engine::ObjectNetworkRole role);
    Engine::ObjectNetworkRole object_network_role_from_string(const std::string &role);

    std::string object_attachment_type_to_string(Engine::BaseObjectAttachmentType type);
    Engine::BaseObjectAttachmentType object_attachment_type_from_string(const std::string &type);

    std::string multiplayer_team_to_string(Engine::MultiplayerTeam team);
    Engine::MultiplayerTeam multiplayer_team_from_string(const std::string &team);

    std::string camera_type_to_string(Engine::CameraType camera_type);
    Engine::CameraType camera_type_from_string(const std::string &camera_type);

    Engine::Point2DInt lua_to_point2_d_int(lua_State *state, int index);
    Engine::Point3D lua_to_point3_d(lua_State *state, int index);
    Engine::ColorARGBInt lua_to_color_a_r_g_b_int(lua_State *state, int index);

    void lua_push_engine_tag(lua_State *state, Engine::Tag *tag) noexcept;

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
    std::string network_color_to_string(Engine::NetworkColor color);
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
    Engine::NetworkColor network_color_from_string(const std::string &color);
    Engine::PlayerObjectiveMode player_objective_mode_from_string(const std::string &mode);

    void lua_push_meta_engine_color_a_r_g_b_int(lua_State *state, Engine::ColorARGBInt &color) noexcept;
    void lua_push_meta_engine_tag_dependency(lua_State *state, Engine::TagDependency &dependency) noexcept;
    void lua_push_meta_engine_point2_d(lua_State *state, Engine::Point2D &point) noexcept;
    void lua_push_meta_engine_point3_d(lua_State *state, Engine::Point3D &point) noexcept;
    void lua_push_meta_engine_tag_data_offset(lua_State *state, Engine::TagDataOffset &offset) noexcept;
    void lua_push_meta_engine_color_a_r_g_b(lua_State *state, Engine::ColorARGB &color) noexcept;
    void lua_push_meta_engine_rectangle2_d(lua_State *state, Engine::Rectangle2D &rectangle) noexcept;
    void lua_push_meta_engine_rectangle2_d_f(lua_State *state, Engine::Rectangle2DF &rectangle) noexcept;
    void lua_push_meta_engine_point2_d_int(lua_State *state, Engine::Point2DInt &point) noexcept;
    void lua_push_meta_engine_euler2_d(lua_State *state, Engine::Euler2D &euler) noexcept;
    void lua_push_meta_engine_euler3_d(lua_State *state, Engine::Euler3D &euler) noexcept;
    void lua_push_meta_engine_euler3_d_p_y_r(lua_State *state, Engine::Euler3D &euler) noexcept;
    void lua_push_meta_engine_vector2_d(lua_State *state, Engine::Vector2D &vector) noexcept;
    void lua_push_meta_engine_vector3_d(lua_State *state, Engine::Vector3D &vector) noexcept;
    void lua_push_meta_engine_color_r_g_b(lua_State *state, Engine::ColorRGB &color) noexcept;
    void lua_push_meta_engine_quaternion(lua_State *state, Engine::Quaternion &quaternion) noexcept;
    void lua_push_meta_engine_plane3_d(lua_State *state, Engine::Plane3D &plane) noexcept;
    void lua_push_meta_engine_plane2_d(lua_State *state, Engine::Plane2D &plane) noexcept;
    void lua_push_meta_engine_widget(lua_State *state, Engine::Widget &widget) noexcept;
    void lua_push_meta_engine_camera_data(lua_State *state, Engine::CameraData &data) noexcept;
    void lua_push_meta_engine_rotation_matrix(lua_State *state, Engine::RotationMatrix &matrix) noexcept;
    void lua_push_meta_engine_model_node(lua_State *state, Engine::ModelNode &node) noexcept;
    
    void lua_push_meta_engine_object_flags(lua_State *state, Engine::BaseObjectFlags &flags) noexcept;
    void lua_push_meta_engine_object_network(lua_State *state, Engine::BaseObjectNetwork &network) noexcept;
    void lua_push_meta_engine_scenario_location(lua_State *state, Engine::ScenarioLocation &location) noexcept;
    void lua_push_meta_engine_object_vitals_flags(lua_State *state, Engine::BaseObjectVitalsFlags &flags) noexcept;
    void lua_push_meta_engine_object_vitals(lua_State *state, Engine::BaseObjectVitals &flags) noexcept;
    void lua_push_meta_engine_object_attachments_data(lua_State *state, Engine::BaseObjectAttachmentsData &data) noexcept;
    void lua_push_meta_engine_object_region_destroyeds(lua_State *state, Engine::BaseObjectRegionDestroyeds &destroyeds) noexcept;
    void lua_push_meta_engine_object_block_reference(lua_State *state, Engine::BaseObjectBlockReference &reference) noexcept;
    void lua_push_meta_engine_object(lua_State *state, Engine::BaseObject &object) noexcept;

    void lua_push_meta_engine_unit_recent_damager(lua_State *state, Engine::UnitRecentDamager &damager) noexcept;
    void lua_push_meta_engine_unit_flags(lua_State *state, Engine::UnitFlags &flags) noexcept;
    void lua_push_meta_engine_unit_control_flags(lua_State *state, Engine::UnitControlFlags &flags) noexcept;
    void lua_push_meta_engine_unit_animation_state_data(lua_State *state, Engine::UnitAnimationStateData &data) noexcept;
    void lua_push_meta_engine_unit_animation_flags(lua_State *state, Engine::UnitAnimationFlags &data) noexcept;
    void lua_push_meta_engine_unit_animation_data(lua_State *state, Engine::UnitAnimationData &data) noexcept;
    void lua_push_meta_engine_ai_communication_packet(lua_State *state, Engine::AiCommunicationPacket &packet) noexcept;
    void lua_push_meta_engine_unit_speech(lua_State *state, Engine::UnitSpeech &speech) noexcept;
    void lua_push_meta_engine_unit_speech_data(lua_State *state, Engine::UnitSpeech &speech) noexcept;
    void lua_push_meta_engine_unit_control_data(lua_State *state, Engine::UnitControlData &data) noexcept;
    void lua_push_meta_engine_unit_object(lua_State *state, Engine::UnitObject &object) noexcept;
    void lua_push_meta_engine_biped_flags(lua_State *state, Engine::BipedFlags &flags) noexcept;
    void lua_push_meta_engine_biped_network_delta(lua_State *state, Engine::BipedNetworkDelta &delta) noexcept;
    void lua_push_meta_engine_biped_network(lua_State *state, Engine::BipedNetwork &network) noexcept;
    void lua_push_meta_engine_biped_object(lua_State *state, Engine::BipedObject &object) noexcept;
    void lua_push_meta_engine_vehicle_flags(lua_State *state, Engine::VehicleFlags &flags) noexcept;
    void lua_push_meta_engine_vehicle_network_data(lua_State *state, Engine::VehicleNetworkData &data) noexcept;
    void lua_push_meta_engine_vehicle_network(lua_State *state, Engine::VehicleNetwork &network) noexcept;
    void lua_push_meta_engine_vehicle_object(lua_State *state, Engine::VehicleObject &object) noexcept;

    void lua_push_meta_engine_item_object(lua_State *state, Engine::ItemObject &object) noexcept;
    void lua_push_meta_engine_garbage_object(lua_State *state, Engine::GarbageObject &object) noexcept;
    void lua_push_meta_engine_weapon_trigger(lua_State *state, Engine::WeaponTrigger &trigger) noexcept;
    void lua_push_meta_engine_weapon_magazine(lua_State *state, Engine::WeaponMagazine &magazine) noexcept;
    void lua_push_meta_engine_weapon_reload_start_data(lua_State *state, Engine::WeaponReloadStartData &data) noexcept;
    void lua_push_meta_engine_weapon_network_data(lua_State *state, Engine::WeaponNetworkData &data) noexcept;
    void lua_push_meta_engine_weapon_network(lua_State *state, Engine::WeaponNetwork &network) noexcept;
    void lua_push_meta_engine_weapon_object(lua_State *state, Engine::WeaponObject &object) noexcept;
    void lua_push_meta_engine_equipment_network_data(lua_State *state, Engine::EquipmentNetworkData &data) noexcept;
    void lua_push_meta_engine_equipment_network(lua_State *state, Engine::EquipmentNetwork &network) noexcept;
    void lua_push_meta_engine_equipment_object(lua_State *state, Engine::EquipmentObject &object) noexcept;
    void lua_push_meta_engine_projectile_object_flags(lua_State *state, Engine::ProjectileObjectFlags &flags) noexcept;
    void lua_push_meta_engine_projectile_network_data(lua_State *state, Engine::ProjectileNetworkData &data) noexcept;
    void lua_push_meta_engine_projectile_network(lua_State *state, Engine::ProjectileNetwork &network) noexcept;
    void lua_push_meta_engine_projectile_object(lua_State *state, Engine::ProjectileObject &object) noexcept;

    void lua_push_meta_engine_device_object_state(lua_State *state, Engine::DeviceObjectState &object_state) noexcept;
    void lua_push_meta_engine_device_object(lua_State *state, Engine::DeviceObject &object) noexcept;
    void lua_push_meta_engine_device_machine_object_flags(lua_State *state, Engine::DeviceMachineObjectFlags &flags) noexcept;
    void lua_push_meta_engine_device_machine_object(lua_State *state, Engine::DeviceMachineObject &object) noexcept;
    void lua_push_meta_engine_device_control_object_flags(lua_State *state, Engine::DeviceControlObjectFlags &flags) noexcept;
    void lua_push_meta_engine_device_control_object(lua_State *state, Engine::DeviceControlObject &object) noexcept;
    void lua_push_meta_engine_device_light_fixture_object(lua_State *state, Engine::DeviceLightFixtureObject &object) noexcept;

    void lua_push_meta_engine_player_multiplayer_statistics(lua_State *state, Engine::PlayerMultiplayerStatistics &statistics) noexcept;
    void lua_push_meta_engine_player(lua_State *state, Engine::Player &player) noexcept;

    template<typename T>
    void lua_push_meta_object(lua_State *state, T &elem, lua_CFunction index, lua_CFunction newindex) noexcept {
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
        lua_pushcfunction(state, newindex);
        lua_setfield(state, -2, "__newindex");

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
