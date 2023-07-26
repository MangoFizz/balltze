// SPDX-License-Identifier: GPL-3.0-only

#include <optional>
#include <balltze/engine/multiplayer.hpp>
#include <balltze/engine/game_state.hpp>
#include <balltze/memory.hpp>

namespace Balltze::Engine {
    BaseDynamicObject *ObjectTable::get_dynamic_object(const ObjectHandle &object_handle) noexcept {
        auto *object = this->get_element(object_handle.index.index);
        if(object && object->id == object_handle.index.id) {
            return object->object;
        }
        else {
            return nullptr;
        }
    }

    BaseDynamicObject *ObjectTable::get_dynamic_object(std::uint32_t index) noexcept {
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
        std::uint32_t object_type = (get_server_type() == SERVER_LOCAL) ? 0 : 3;

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
        apply_damage_asm(&s, object.whole_id);
    }

    PlayerHandle Player::get_handle() const noexcept {
        PlayerHandle player_id;
        player_id.index.id = this->player_id;
        player_id.index.index = static_cast<std::uint16_t>(this - get_player_table().first_element);
        return player_id;
    }

    PlayerHandle get_client_player_id() noexcept {
        static PlayerHandle *player_id = reinterpret_cast<PlayerHandle *>(**reinterpret_cast<std::byte ***>(Memory::get_signature("player_handle_address")->data()) + 4);
        return *player_id;
    }

    Player *PlayerTable::get_player(PlayerHandle player_id) noexcept {
        if(player_id.is_null()) {
            return nullptr;
        }
        if(player_id.index.index >= this->current_size) {
            return nullptr;
        }
        auto &player = this->first_element[player_id.index.index];
        if(player.player_id != player_id.index.id) {
            return nullptr;
        }
        return &player;
    }

    Player *PlayerTable::get_player_by_rcon_handle(std::size_t rcon_id) noexcept {
        auto *server_info = ServerInfoPlayerList::get_server_info_player_list();
        if(!server_info) {
            return nullptr;
        }

        // Make sure we have enough thingies
        if(rcon_id < sizeof(server_info->players) / sizeof(server_info->players[rcon_id])) {
            auto *player = server_info->players[rcon_id].get_player_table_player();
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
        return this->get_player(get_client_player_id());
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
}
