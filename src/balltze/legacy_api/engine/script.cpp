// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__ENGINE___SCRIPT_HPP
#define BALLTZE__ENGINE___SCRIPT_HPP

#include <cstdint>
#include <balltze/memory.hpp>
#include <balltze/legacy_api/engine/data_types.hpp>
#include <balltze/legacy_api/engine/game_state.hpp>
#include <balltze/legacy_api/engine/tag.hpp>
#include <balltze/legacy_api/engine/tag_definitions/vehicle.hpp>

namespace Balltze::LegacyApi::Engine {
    extern "C" {
        void unit_scripting_enter_vehicle_asm(ObjectHandle unit_handle, ObjectHandle vehicle_handle, const char *seat_name);
        void unit_scripting_exit_vehicle_asm(ObjectHandle unit_handle);
        std::uint16_t script_get_numeric_countdown_timer(std::uint16_t units);
        void execute_script_asm(const char *script) noexcept;
    }

    void execute_script(const char *script) noexcept {
        execute_script_asm(script);
    }

    void unit_scripting_enter_vehicle(ObjectHandle unit_handle, ObjectHandle vehicle_handle, std::string seat_label) {
        auto *vehicle_object = reinterpret_cast<LegacyApi::Engine::UnitObject *>(get_object_table().get_object(vehicle_handle));
        if(!vehicle_object) {
            throw std::runtime_error("vehicle object not found");
        }
        if(!vehicle_object->type == LegacyApi::Engine::OBJECT_TYPE_VEHICLE) {
            throw std::runtime_error("object is not a vehicle");
        }

        auto tag_handle = vehicle_object->tag_handle;
        auto tag = get_tag(tag_handle);
        if(!tag) {
            throw std::runtime_error("vehicle tag not found");
        }
        auto *tag_data = reinterpret_cast<TagDefinitions::Vehicle *>(tag->data);

        for(std::size_t seat_index = 0; seat_index < tag_data->seats.count; seat_index++) {
            auto &seat = tag_data->seats.elements[seat_index];
            if(seat.label.string == seat_label) {
                unit_scripting_enter_vehicle_asm(unit_handle, vehicle_handle, seat_label.c_str());
                return;
            }
        }

        throw std::runtime_error("seat not found");
    }

    void unit_scripting_enter_vehicle(ObjectHandle unit_handle, ObjectHandle vehicle_handle, std::size_t seat_index) {
        auto *vehicle_object = reinterpret_cast<LegacyApi::Engine::UnitObject *>(get_object_table().get_object(vehicle_handle));
        if(!vehicle_object) {
            throw std::runtime_error("vehicle object not found");
        }
        if(!vehicle_object->type == LegacyApi::Engine::OBJECT_TYPE_VEHICLE) {
            throw std::runtime_error("object is not a vehicle");
        }

        auto tag_handle = vehicle_object->tag_handle;
        auto tag = get_tag(tag_handle);
        if(!tag) {
            throw std::runtime_error("vehicle tag not found");
        }
        auto *tag_data = reinterpret_cast<TagDefinitions::Vehicle *>(tag->data);

        if(seat_index < 0 || seat_index >= tag_data->seats.count) {
            throw std::runtime_error("seat index out of bounds");
        }

        auto &seat_name = tag_data->seats.elements[seat_index].label.string;
        unit_scripting_enter_vehicle_asm(unit_handle, vehicle_handle, seat_name);
    }

    void unit_scripting_exit_vehicle(ObjectHandle unit_handle) {
        auto *unit = get_object_table().get_object(unit_handle);
        if(!unit) {
            throw std::runtime_error("unit object not found");
        }
        if(!unit->type == LegacyApi::Engine::OBJECT_TYPE_BIPED && !unit->type == LegacyApi::Engine::OBJECT_TYPE_VEHICLE) {
            throw std::runtime_error("object is not a biped/vehicle");
        }
        unit_scripting_exit_vehicle_asm(unit_handle);
    }

    std::uint16_t get_numeric_countdown_timer(std::uint16_t units) {
        return get_numeric_countdown_timer(units);
    }
}

#endif
