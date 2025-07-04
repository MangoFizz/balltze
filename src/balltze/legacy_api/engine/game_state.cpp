// SPDX-License-Identifier: GPL-3.0-only

#include <optional>
#include <balltze/legacy_api/engine/netgame.hpp>
#include <balltze/legacy_api/engine/tag.hpp>
#include <balltze/legacy_api/engine/tag_definitions/vehicle.hpp>
#include <balltze/legacy_api/engine/game_state.hpp>
#include <balltze/memory.hpp>
#include <impl/unit/unit.h>

namespace Balltze::LegacyApi::Engine {
    BaseObject *ObjectTable::get_object(const ObjectHandle &object_handle) noexcept {
        auto *object = this->get_element(object_handle.index);
        if(object && object->id == object_handle.id) {
            return object->object;
        }
        else {
            return nullptr;
        }
    }

    BaseObject *ObjectTable::get_object(std::uint32_t index) noexcept {
        auto *object = this->get_element(index);
        if(object) {
            return object->object;
        }
        else {
            return nullptr;
        }
    }

    struct s_object_creation_disposition {
        uint32_t tag_handle;
        PADDING(0x4);
        uint32_t player_handle;
        uint32_t parent;
        PADDING(0x4);
        PADDING(0x4);
        float pos[3];
    };

    extern "C" {
        void create_object_query_asm(TagHandle tag_handle, ObjectHandle parent, s_object_creation_disposition *query);
        ObjectHandle create_object_asm(s_object_creation_disposition *query, std::uint32_t object_type);
        void delete_object_asm(TagHandle tag_handle);
    }

    ObjectHandle ObjectTable::create_object(const TagHandle &tag_handle, Point3D offset, const ObjectHandle &parent) noexcept {
        char buffer[1024] = {};
        auto *object_create_query = reinterpret_cast<s_object_creation_disposition *>(buffer);

        // Create query
        create_object_query_asm(tag_handle, parent, object_create_query);

        // Set object parameters
        object_create_query->player_handle = 0xFFFFFFFF;
        object_create_query->pos[0] = offset.x;
        object_create_query->pos[1] = offset.y;
        object_create_query->pos[2] = offset.z;

        /**
            0 = created by local machine
            1 = host
            2 = ???
            3 = client sided object (from giraffe)
        */
        std::uint32_t object_type = (network_game_get_server_type() == NETWORK_GAME_SERVER_LOCAL) ? 0 : 3;

        auto object_handle = create_object_asm(object_create_query, object_type);
        return object_handle;
    }

    void ObjectTable::delete_object(const ObjectHandle &object_handle) noexcept {
        delete_object_asm(object_handle);
    }

    void ObjectTable::delete_object(std::uint32_t index) noexcept {
        delete_object_asm(index);
    }

    ObjectTable &get_object_table() noexcept {
        static auto &object_table = ***reinterpret_cast<ObjectTable ***>(Memory::get_signature("object_table_address")->data());
        return object_table;
    }

    extern "C" void apply_damage_asm(DamageObjectStructThing *damage, std::uint32_t object);

    void apply_damage(ObjectHandle object, TagHandle damage_effect, float multiplier, PlayerHandle causer_player, ObjectHandle causer_object) noexcept {
        DamageObjectStructThing s;
        s.damage_tag_handle = damage_effect;
        s.causer_player = causer_player;
        s.causer_object = causer_object;
        s.multiplier = multiplier;
        apply_damage_asm(&s, object.value);
    }

    PlayerHandle Player::get_handle() const noexcept {
        PlayerHandle player_handle;
        player_handle.id = this->player_id;
        player_handle.index = static_cast<std::uint16_t>(this - get_player_table().first_element);
        return player_handle;
    }

    PlayerHandle get_client_player_handle() noexcept {
        static PlayerHandle *player_handle = reinterpret_cast<PlayerHandle *>(**reinterpret_cast<std::byte ***>(Memory::get_signature("player_handle_address")->data()) + 4);
        return *player_handle;
    }

    Player *PlayerTable::get_player(PlayerHandle player_handle) noexcept {
        if(player_handle.is_null()) {
            return nullptr;
        }
        if(player_handle.index >= this->current_size) {
            return nullptr;
        }
        auto &player = this->first_element[player_handle.index];
        if(player.player_id != player_handle.id) {
            return nullptr;
        }
        return &player;
    }

    Player *PlayerTable::get_player_by_rcon_handle(std::size_t rcon_id) noexcept {
        auto *server_info = network_game_get_server();
        if(!server_info) {
            return nullptr;
        }

        // Make sure we have enough thingies
        if(rcon_id < sizeof(server_info->players) / sizeof(server_info->players[rcon_id])) {
            auto *player = server_info->players[rcon_id].get_player();
            if(player && player->player_id == 0xFFFF) {
                return nullptr;
            }
            return player;
        }
        else {
            return nullptr;
        }
    }

    Player *PlayerTable::get_client_player() noexcept {
        return this->get_player(get_client_player_handle());
    }

    PlayerTable &get_player_table() noexcept {
        static PlayerTable *table = nullptr;
        if(!table) {
            table = *reinterpret_cast<PlayerTable **>(*reinterpret_cast<std::byte **>(Memory::get_signature("player_table_address")->data()));
        }
        return *table;
    }

    AntennaTable &get_antenna_table() noexcept {
        static auto *antenna_table = **reinterpret_cast<AntennaTable ***>(Memory::get_signature("antenna_table_address")->data());
        return *antenna_table;
    }

    DecalTable &get_decal_table() noexcept {
        static auto *decal_table = **reinterpret_cast<DecalTable ***>(Memory::get_signature("decal_table_address")->data());
        return *decal_table;
    }

    EffectTable &get_effect_table() noexcept {
        static auto *effect_table = **reinterpret_cast<EffectTable ***>(Memory::get_signature("effect_table_address")->data());
        return *effect_table;
    }

    FlagTable &get_flag_table() noexcept {
        static auto *flag_table = **reinterpret_cast<FlagTable ***>(Memory::get_signature("flag_table_address")->data());
        return *flag_table;
    }

    LightTable &get_light_table() noexcept {
        static auto *light_table = **reinterpret_cast<LightTable ***>(Memory::get_signature("light_table_address")->data());
        return *light_table;
    }

    ParticleTable &get_particle_table() noexcept {
        static auto *particle_table = **reinterpret_cast<ParticleTable ***>(Memory::get_signature("particle_table_address")->data());
        return *particle_table;
    }

    bool game_paused() noexcept {
        static std::optional<std::byte **> paused_addr;
        if(!paused_addr.has_value()) {
            paused_addr = *reinterpret_cast<std::byte ***>(Memory::get_signature("game_paused_flag_address")->data());
        }
        return *reinterpret_cast<bool *>(*paused_addr.value() + 2);
    }

    CameraType get_camera_type() noexcept {
        static auto *cta = reinterpret_cast<CameraType *>(*reinterpret_cast<std::byte **>(Memory::get_signature("camera_type")->data()) + 0x56);
        return *cta;
    }

    CameraData &get_camera_data() noexcept {
        static std::optional<CameraData *> camera_coord_addr;
        if(!camera_coord_addr.has_value()) {
            camera_coord_addr = reinterpret_cast<CameraData *>(*reinterpret_cast<std::byte **>(Memory::get_signature("camera_coord")->data()) - 0x8);
        }
        return **camera_coord_addr;
    }

    void unit_delete_all_weapons(ObjectHandle unit_handle) {
        if(unit_handle.is_null()) {
            throw std::runtime_error("invalid unit object handle");
        }
        auto &object_table = LegacyApi::Engine::get_object_table();
        auto *unit = reinterpret_cast<LegacyApi::Engine::UnitObject *>(object_table.get_object(unit_handle));
        if(!unit) {
            throw std::runtime_error("unit object not found");
        }
        if(unit->type != LegacyApi::Engine::OBJECT_TYPE_BIPED) {
            throw std::runtime_error("invalid object type, expected biped");
        }
        ::unit_delete_all_weapons((::ObjectHandle)unit_handle.value);
    }

    void unit_add_weapon_to_inventory(ObjectHandle unit_handle, ObjectHandle weapon_handle, uint16_t param_3) {
        if(unit_handle.is_null()) {
            throw std::runtime_error("invalid unit object handle");
        }
        if(weapon_handle.is_null()) {
            throw std::runtime_error("invalid weapon object handle");
        }
        auto &object_table = LegacyApi::Engine::get_object_table();
        auto *unit = reinterpret_cast<LegacyApi::Engine::UnitObject *>(object_table.get_object(unit_handle));
        if(!unit) {
            throw std::runtime_error("unit object not found");
        }
        if(unit->type != LegacyApi::Engine::OBJECT_TYPE_BIPED) {
            throw std::runtime_error("invalid object type, expected biped");
        }
        ::unit_add_weapon_to_inventory((::ObjectHandle)unit_handle.value, (::ObjectHandle)weapon_handle.value, param_3);
    }

    void object_attach_to_marker(ObjectHandle object, std::optional<std::string> object_marker, ObjectHandle attachment, std::optional<std::string> attachment_marker) {
        if(object.is_null()) {
            throw std::runtime_error("invalid object handle");
        }
        if(attachment.is_null()) {
            throw std::runtime_error("invalid attachment handle");
        }
        ::object_attach_to_marker((::ObjectHandle)object.value, object_marker.value_or("").c_str(), (::ObjectHandle)attachment.value, attachment_marker.value_or("").c_str());
    }
}
