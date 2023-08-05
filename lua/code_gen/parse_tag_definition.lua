-- SPDX-License-Identifier: GPL-3.0-only

---@class DefinitionElement
---@field name string
---@field class string
---@field options string[]
---@field type string | '"struct"' | '"enum"' | '"pad"' | '"bitfield"' | '"Angle"' | '"Fraction"' | '"TagEnum"' | '"TagString"' | '"TagIndex"' | '"unint8"' | '"unint16"' | '"unint32"' | '"int8"' | '"int16"' | '"int32"' | '"float"'
---@field size number
---@field width number
---@field count number
---@field bounds number
---@field fields DefinitionElement[]
---@field struct string
---@field description string
---@field inherits string
---@field cache_only boolean

---@class StructField
---@field name string
---@field type string
---@field pointer boolean
---@field size number
---@field description string
---@field struct string

---@class Struct
---@field name string
---@field width number
---@field inherits string
---@field fields StructField[]

---@class BitfieldStruct
---@field name string
---@field type string
---@field fields string[]

---@class Enum
---@field name string
---@field values string[]

---@class TagDefinition
---@field structs Struct[]
---@field enums Enum[]
---@field bitfields BitfieldStruct[]

---@class DataType
---@field width number
---@field template boolean

---@type table<string, DataType>
local commonStructs = {
    tag_handle = {width = 0x4},
    tag_reflexive = {width = 0xC, template = true},
    point2_d = {width = 0x8},
    point2_d_int = {width = 0x4},
    point3_d = {width = 0xC},
    rectangle2_d = {width = 0x8},
    color_a_r_g_b = {width = 0x10},
    color_a_r_g_b_int = {width = 0x4},
    color_r_g_b = {width = 0xC},
    tag_data_offset = {width = 0x14},
    tag_dependency = {width = 0x10},
    vector2_d = {width = 0x8},
    vector3_d = {width = 0xC},
    euler2_d = {width = 0x8},
    euler3_d = {width = 0xC},
    angle = {width = 0x4},
    fraction = {width = 0x4},
    index = {width = 0x2},
    tag_enum = {width = 0x2},
    tag_string = {width = 0x20},
    tag_four_c_c = {width = 0x4},
    quaternion = {width = 0x10},
    matrix = {width = 0x24},
    plane3_d = {width = 0x10},
    plane2_d = {width = 0xC},
    scenario_script_node_value = {width = 0x4}
}

local commonEnums = {
    framebuffer_blend_function = {width = 0x2},
    framebuffer_fade_mode = {width = 0x2},
    function_out = {width = 0x2},
    wave_function = {width = 0x2},
    material_type = {width = 0x2},
    function_type = {width = 0x2},
    function_bounds_mode = {width = 0x2},
    function_scale_by = {width = 0x2},
    function_name_nullable = {width = 0x2},
    grenade_type = {width = 0x2},
    vertex_type = {width = 0x2}
}

local commonBitfields = {
    is_unused_flag = {width = 0x4},
    is_unfiltered_flag = {width = 0x4},
    color_interpolation_flags = {width = 0x4}
}

local primitiveTypes = {
    byte = {width = 0x1},
    float = {width = 0x4},
    char = {width = 0x1},
    uint32 = {width = 0x4},
    uint16 = {width = 0x2},
    uint8 = {width = 0x1},
    int32 = {width = 0x4},
    int16 = {width = 0x2},
    int8 = {width = 0x1}
}

local tagClasses = {
    "actor",
    "actor_variant",
    "antenna",
    "model_animations",
    "biped",
    "bitmap",
    "continuous_damage_effect",
    "model_collision_geometry",
    "color_table",
    "contrail",
    "device_control",
    "decal",
    "ui_widget_definition",
    "input_device_defaults",
    "device",
    "detail_object_collection",
    "effect",
    "equipment",
    "flag",
    "fog",
    "font",
    "material_effects",
    "garbage",
    "glow",
    "grenade_hud_interface",
    "hud_message_text",
    "hud_number",
    "hud_globals",
    "item",
    "item_collection",
    "damage_effect",
    "lens_flare",
    "lightning",
    "device_light_fixture",
    "light",
    "sound_looping",
    "device_machine",
    "globals",
    "meter",
    "light_volume",
    "gbxmodel",
    "model",
    "multiplayer_scenario_description",
    "preferences_network_game",
    "object",
    "particle",
    "particle_system",
    "physics",
    "placeholder",
    "point_physics",
    "projectile",
    "weather_particle_system",
    "scenario_structure_bsp",
    "scenery",
    "shader_transparent_chicago_extended",
    "shader_transparent_chicago",
    "scenario",
    "shader_environment",
    "shader_transparent_glass",
    "shader",
    "sky",
    "shader_transparent_meter",
    "sound",
    "sound_environment",
    "shader_model",
    "shader_transparent_generic",
    "ui_widget_collection",
    "shader_transparent_plasma",
    "sound_scenery",
    "string_list",
    "shader_transparent_water",
    "tag_collection",
    "camera_track",
    "dialogue",
    "unit_hud_interface",
    "unit",
    "unicode_string_list",
    "virtual_keyboard",
    "vehicle",
    "weapon",
    "wind",
    "weapon_hud_interface"
}

local function camelCaseToSnakeCase(str)
    if str then
        -- convert camelCase to snake_case but keep numbers
        str = str:sub(1, 1):lower() .. str:sub(2)
        local new = str:gsub("([A-Z])", function(c)
            return "_" .. c:lower()
        end)
        if new:sub(1, 1) == "_" then
            new = new:sub(2)
        end
        return new
    end
end

local function dashAndSentenceToSnakeCase(str)
    -- convert dash-and-sentence to snake_case
    return str:gsub("([A-Z])", function(c)
        return "_" .. c:lower()
    end):gsub("-", "_"):gsub(" ", "_"):gsub("'", "")
end

local function normalToSnakeCase(str)
    if str then
        -- convert normal sentence to snake_case
        return str:gsub(" ", "_"):gsub("[(|)|']", "")
    end
end

local function snakeCaseToCamelCase(str)
    if str then
        -- convert snake_case to camelCase
        local new = str:gsub("_([a-z])", function(c)
            return c:upper()
        end)
        -- capitalize first letter
        new = new:sub(1, 1):upper() .. new:sub(2)
        return new
    end
    return nil
end

---Parse a struct definition
---@param structDefinition DefinitionElement
---@return Struct
local function parseStruct(structDefinition)
    local struct = {
        name = snakeCaseToCamelCase(structDefinition.class) or structDefinition.name,
        width = structDefinition.size,
        fields = {}
    }

    if(structDefinition.inherits) then
        struct.inherits = structDefinition.inherits
    end

    for fieldIndex, field in pairs(structDefinition.fields) do
        local fieldName = normalToSnakeCase(camelCaseToSnakeCase(field.name))
        local fieldType = field.type

        if fieldName and fieldName:find("%d") == 1 then
            fieldName = "_" .. fieldName
        end

        local structField = {}

        if field.type == "pad" then
            structField = {type = "pad"}
        elseif field.type == "TagID" then
            -- Replace "tag_id" with "tag_handle" in field name
            fieldName = fieldName:gsub("tag_id", "tag_handle")
            structField = {name = fieldName, type = "TagHandle"}
        elseif field.type == "Pointer" then
            structField = {name = fieldName, type = "byte", pointer = true}
        elseif field.type == "TagReflexive" then
            structField = {name = fieldName, type = "TagReflexive", struct = field.struct}    
        else
            structField = {name = fieldName, type = fieldType}
        end

        structField.size = field.size or field.count

        if field.bounds then
            structField.size = 2
        end

        structField.cacheOnly = field.cache_only or false

        struct.fields[fieldIndex] = structField
    end

    return struct
end

---Parse a enum definition
---@param enumDefinition DefinitionElement
---@return Enum
local function parseEnum(enumDefinition) 
    local enum = {name = enumDefinition.name, values = {}}
    if enumDefinition.options then
        for index, value in pairs(enumDefinition.options) do
            enum.values[index] = camelCaseToSnakeCase(enumDefinition.name) .. "_" .. dashAndSentenceToSnakeCase(value)
        end
    end
    return enum
end

---Parse a bitfield definition
---@param bitfieldDefinition DefinitionElement
---@return BitfieldStruct
local function parseBitfield(bitfieldDefinition)
    local bitfieldType
    if bitfieldDefinition.width == 32 then
        bitfieldType = "uint32"
    elseif bitfieldDefinition.width == 16 then
        bitfieldType = "uint16"
    elseif bitfieldDefinition.width == 8 then
        bitfieldType = "uint8"
    end

    local bitfield = {name = bitfieldDefinition.name, type = bitfieldType, fields = {}}
    if bitfieldDefinition.fields then
        for index, field in pairs(bitfieldDefinition.fields) do
            local fieldName = dashAndSentenceToSnakeCase(field)
            if fieldName and fieldName:find("%d") == 1 then
                fieldName = "_" .. fieldName
            end
            bitfield.fields[index] = fieldName
        end
    end
    return bitfield
end

---Convert an Invader element to type definition
---@param definition DefinitionElement
---@return TagDefinition
local function parseDefinition(definitionName, definition)
    local enums = {}
    local bitfields = {}
    local structs = {}
    
    for _, element in pairs(definition) do
        if element.type == "enum" then
            enums[#enums + 1] = parseEnum(element)
        elseif element.type == "bitfield" then
            bitfields[#bitfields + 1] = parseBitfield(element)
        elseif element.type == "struct" then
            structs[#structs + 1] = parseStruct(element)
        end
    end

    return {enums = enums, bitfields = bitfields, structs = structs}
end

local function getDependencies(definitions)
    local dependencies = {}

    local findType = function(typeName)
        if typeName == "pad" then
            return true, nil, "pad"
        elseif commonStructs["" .. camelCaseToSnakeCase(typeName)] then
            return true, "common", "struct"
        elseif commonEnums["" .. camelCaseToSnakeCase(typeName)] then
            return true, "common", "enum"
        elseif commonBitfields["" .. camelCaseToSnakeCase(typeName)] then
            return true, "common", "bitfield"
        elseif primitiveTypes["" .. camelCaseToSnakeCase(typeName)] then
            return true, nil, "primitive"
        end
        
        for definitionName, definition in pairs(definitions) do
            for _, enum in ipairs(definition.enums) do
                if enum.name == typeName then
                    return true, definitionName, "enum"
                end
            end
    
            for _, bitfield in ipairs(definition.bitfields) do
                if bitfield.name == typeName then
                    return true, definitionName, "bitfield"
                end
            end
    
            for _, struct in ipairs(definition.structs) do
                if struct.name == typeName then
                    return true, definitionName, "struct"
                end
            end
        end
        return false, nil, nil
    end

    -- Check if everything is ok
    for definitionName, definition in pairs(definitions) do
        for _, struct in ipairs(definition.structs) do
            if struct.inherits then
                local exists, dependency, dependencyType = findType(struct.inherits)
                if not exists or dependencyType ~= "struct" then
                    error("Struct " .. struct.name .. " inherits from " .. struct.inherits .. " which does not exist or is not a struct")
                end
                if dependency then
                    if not dependencies[definitionName] then
                        dependencies[definitionName] = {}
                    end
                    if not dependencies[definitionName][dependency] then
                        dependencies[definitionName][dependency] = {}
                    end
                    dependencies[definitionName][dependency][struct.inherits] = dependencyType
                end
            end
            for _, field in pairs(struct.fields) do
                if field.type == "tag_reflexive" then
                    local exists, dependency, dependencyType = findType(field.struct)
                    if not exists or dependencyType ~= "struct" then
                        error("Struct " .. struct.name .. " has a reflexive field with an unknown struct type: " .. field.struct)
                    end
                    if dependency then
                        if not dependencies[definitionName] then
                            dependencies[definitionName] = {}
                        end
                        if not dependencies[definitionName][dependency] then
                            dependencies[definitionName][dependency] = {}
                        end
                        dependencies[definitionName][dependency][field.struct] = dependencyType
                    end
                else
                    local exists, dependency, dependencyType = findType(field.type)
                    if not exists then
                        error("Struct " .. struct.name .. " has a field with an unknown type: " .. field.type)
                    end
                    if dependency then
                        if not dependencies[definitionName] then
                            dependencies[definitionName] = {}
                        end
                        if not dependencies[definitionName][dependency] then
                            dependencies[definitionName][dependency] = {}
                        end
                        dependencies[definitionName][dependency][field.type] = dependencyType
                    end
                end
            end
        end
    end

    return dependencies
end

return {
    camelCaseToSnakeCase = camelCaseToSnakeCase,
    normalToSnakeCase = normalToSnakeCase,
    snakeCaseToCamelCase = snakeCaseToCamelCase,
    dashAndSentenceToSnakeCase = dashAndSentenceToSnakeCase,
    parseDefinition = parseDefinition,
    getDependencies = getDependencies,
    commonStructs = commonStructs,
    commonEnums = commonEnums,
    commonBitfields = commonBitfields,
    primitiveTypes = primitiveTypes,
    tagClasses = tagClasses,
}
