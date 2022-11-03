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
local dataTypes = {
    -- Engine types
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
    scenario_script_node_value = {width = 0x4},

    -- Enums
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
    vertex_type = {width = 0x2},

    -- Bitfields
    is_unused_flag = {width = 0x4},
    is_unfiltered_flag = {width = 0x4},
    color_interpolation_flags = {width = 0x4},

    -- Atomic types
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

-- Dirty workaround for resolve dependencies between tags definitions
local dependencies = {
    antenna = {
        {
            name = "unit",
            types = {
                unit = {width = 0x2F0},
            }
        }
    },
    actor_variant = {
        {
            name = "unit",
            types = {
                metagame_type = {width = 0x2},
                metagame_class = {width = 0x2}
            }
        }
    },
    biped = {
        {
            name = "unit",
            types = {
                unit = {width = 0x2F0},
            }
        }
    },
    continuous_damage_effect = {
        {
            name = "damage_effect",
            types = {
                damage_effect_side_effect = {width = 0x2},
                damage_effect_category = {width = 0x2},
                damage_effect_damage_flags = {width = 0x4}
            }
        }
    },
    contrail = {
        {
            name = "particle",
            types = {
                particle_shader_flags = {width = 0x2},
                particle_anchor = {width = 0x2}
            }
        }
    },
    device = {
        {
            name = "object",
            types = {
                object = {width = 0x17C},
            }
        }
    },
    device_control = {
        {
            name = "device",
            types = {
                device = {width = 0x290},
            }
        }
    },
    device_light_fixture = {
        {
            name = "device",
            types = {
                device = {width = 0x290},
            }
        }
    },
    device_machine = {
        {
            name = "device",
            types = {
                device = {width = 0x290},
            }
        }
    },
    equipment = {
        {
            name = "item",
            types = {
                item = {width = 0x308},
            }
        }
    },
    garbage = {
        {
            name = "item",
            types = {
                item = {width = 0x308},
            }
        }
    },
    gbxmodel = {
        {
            name = "model",
            types = {
                model_flags = {width = 0x4},
                model_marker = {width = 0x40},
                model_node = {width = 0x9C},
                model_region = {width = 0x4C},
                model_shader_reference = {width = 0x4},
                model_geometry_part = {width = 0x68}
            }
        }
    },
    grenade_hud_interface = {
        {
            name = "hud_interface_types",
            types = {
                h_u_d_interface_scaling_flags = {width = 0x2},
                h_u_d_interface_flash_flags = {width = 0x2},
                h_u_d_interface_overlay_flash_flags = {width = 0x2},
                h_u_d_interface_anchor = {width = 0x2},
                h_u_d_interface_multitexture_overlay = {width = 0x1E0},
                h_u_d_interface_number_flags = {width = 0x2},
                h_u_d_interface_messaging_flags = {width = 0x2}
            }
        }
    },
    hud_globals = {
        {
            name = "hud_interface_types",
            types = {
                h_u_d_interface_messaging_flags = {width = 0x2},
                h_u_d_interface_anchor = {width = 0x2},
                h_u_d_interface_scaling_flags = {width = 0x2},
                h_u_d_interface_flash_flags = {width = 0x2}
            }
        }
    },
    item = {
        {
            name = "object",
            types = {
                object = {width = 0x17C},
                object_function_in = {width = 0x2}
            }
        }
    },
    lightning = {
        {
            name = "particle",
            types = {
                particle_shader_flags = {width = 0x2}
            }
        }
    },
    object = {
        {
            name = "object",
            types = {
                object = {width = 0x17C}
            }
        }
    },
    particle_system = {
        {
            name = "particle",
            types = {
                particle_shader_flags = {width = 0x2},
                particle_anchor = {width = 0x2}
            }
        }
    },
    placeholder = {
        {
            name = "object",
            types = {
                basic_object = {width = 0x17C}
            }
        }
    },
    projectile = {
        {
            name = "object",
            types = {
                object = {width = 0x2},
                object_noise = {width = 0x2}
            }
        }
    },
    scenario = {
        {
            name = "object",
            types = {
                object_type = {width = 0x2},
                predicted_resource = {width = 0x2}
            }
        },
        {
            name = "actor",
            types = {
                actor_type = {width = 0x2},
            }
        }
    },
    scenario_structure_bsp = {
        {
            name = "object",
            types = {
                predicted_resource = {width = 0x2}
            }
        },
        {
            name = "model_collision_geometry",
            types = {
                model_collision_geometry_b_s_p = {width = 0x60}
            }
        }
    },
    scenery = {
        {
            name = "object",
            types = {
                basic_object = {width = 0x17C}
            }
        }
    },
    shader_environment = {
        {
            name = "shader",
            types = {
                shader = {width = 0x28},
                shader_detail_function = {width = 0x2}
            }
        }
    },
    shader_model = {
        {
            name = "shader",
            types = {
                shader = {width = 0x28},
                shader_detail_function = {width = 0x2}
            }
        }
    },
    shader_transparent_chicago = {
        {
            name = "shader",
            types = {
                shader = {width = 0x28},
                shader_color_function_type = {width = 0x2},
                shader_first_map_type = {width = 0x2},
                shader_transparent_extra_layer = {width = 0x10},
            }
        },
        {
            name = "shader_transparent_generic",
            types = {
                shader_transparent_generic_flags = {width = 0x8}
            }
        }
    },
    shader_transparent_chicago_extended = {
        {
            name = "shader",
            types = {
                shader = {width = 0x28},
                shader_color_function_type = {width = 0x2},
                shader_first_map_type = {width = 0x2},
                shader_transparent_extra_layer = {width = 0x10},
            }
        },
        {
            name = "shader_transparent_generic",
            types = {
                shader_transparent_generic_flags = {width = 0x8}
            }
        },
        {
            name = "shader_transparent_chicago",
            types = {
                shader_transparent_chicago_map = {width = 0xDC},
                shader_transparent_chicago_extra_flags = {width = 0x2}
            }
        }
    },
    shader_transparent_generic = {
        {
            name = "shader",
            types = {
                shader = {width = 0x28},
                shader_first_map_type = {width = 0x2},
                shader_transparent_extra_layer = {width = 0x10}
            }
        }
    },
    shader_transparent_glass = {
        {
            name = "shader",
            types = {
                shader = {width = 0x28}
            }
        }
    },
    shader_transparent_meter = {
        {
            name = "shader",
            types = {
                shader = {width = 0x28}
            }
        }
    },
    shader_transparent_plasma = {
        {
            name = "shader",
            types = {
                shader = {width = 0x28}
            }
        }
    },
    shader_transparent_water = {
        {
            name = "shader",
            types = {
                shader = {width = 0x28}
            }
        }
    },
    sound_scenery = {
        {
            name = "object",
            types = {
                basic_object = {width = 0x17C}
            }
        }
    },
    unit = {
        {
            name = "object",
            types = {
                object = {width = 0x2},
                object_noise = {width = 0x2},
            }
        }
    },
    unit_hud_interface = {
        {
            name = "hud_interface_types",
            types = {
                h_u_d_interface_scaling_flags = {width = 0x2},
                h_u_d_interface_flash_flags = {width = 0x2},
                h_u_d_interface_multitexture_overlay = {width = 0x1E0},
                h_u_d_interface_meter_flags = {width = 0x8},
                h_u_d_interface_anchor = {width = 0x2}
            }
        }
    },
    weapon = {
        {
            name = "object",
            types = {
                object_noise = {width = 0x2},
                predicted_resource = {width = 0x2}
            }
        },
        {
            name = "item",
            types = {
                item = {width = 0x308},
            }
        }
    },
    weapon_hud_interface = {
        {
            name = "hud_interface_types",
            types = {
                h_u_d_interface_child_anchor = {width = 0x2},
                h_u_d_interface_scaling_flags = {width = 0x2},
                h_u_d_interface_flash_flags = {width = 0x2},
                h_u_d_interface_multitexture_overlay = {width = 0x1E0},
                h_u_d_interface_meter_flags = {width = 0x8},
                h_u_d_interface_number_flags = {width = 0x2},
                h_u_d_interface_overlay_flash_flags = {width = 0x2},
                h_u_d_interface_anchor = {width = 0x2},
                h_u_d_interface_messaging_flags = {width = 0x2}
            }
        }
    },
    weather_particle_system = {
        {
            name = "particle",
            types = {
                particle_orientation = {width = 0x2},
                particle_shader_flags = {width = 0x2},
                particle_anchor = {width = 0x2}
            }
        }
    },
    vehicle = {
        {
            name = "unit",
            types = {
                unit = {width = 0x2F0},
            }
        }
    }
}

local function camelCaseToSnakeCase(str)
    if str then
        -- convert camelCase to snake_case but keep numbers
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
end

---Parse a struct definition
---@param structDefinition DefinitionElement
---@return Struct
local function parseStruct(structDefinition)
    local struct = {
        name = structDefinition.class or structDefinition.name,
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
            structField = {name = fieldName, type = "tag_handle"}
        elseif field.type == "Pointer" then
            structField = {name = fieldName, type = "byte", pointer = true}
        elseif field.type == "TagReflexive" then
            structField = {name = fieldName, type = "tag_reflexive", struct = field.struct}    
        else
            structField = {name = fieldName, type = fieldType}
        end

        structField.size = field.size or field.count

        if field.bounds then
            structField.size = 2
        end

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
            -- prevent name conflicts with C memes
            if value == "null" or value == "passthrough" or value == "void" then
                value = value .. " value"
            elseif value == "double click" then
                value = "double click event"
            elseif value:find("%d") == 1 then
                value = "_" .. value
            end
            enum.values[index] = dashAndSentenceToSnakeCase(value)
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

    
    local typeExists = function(typeName)
        if typeName == "pad" then
            return true
        elseif dataTypes["" .. camelCaseToSnakeCase(typeName)] then
            return true
        elseif(dependencies[definitionName]) then
            for _, dependency in ipairs(dependencies[definitionName]) do
                if dependency.types[camelCaseToSnakeCase(typeName)] then
                    return true
                end
            end
        end

        for _, enum in ipairs(enums) do
            if enum.name == typeName then
                return true
            end
        end

        for _, bitfield in ipairs(bitfields) do
            if bitfield.name == typeName then
                return true
            end
        end

        for _, struct in ipairs(structs) do
            if struct.name == typeName then
                return true
            end
        end

        return false
    end

    -- Check if everything is ok
    for _, struct in ipairs(structs) do
        if struct.inherits then
            if not typeExists(struct.inherits) then
                error("Struct " .. struct.name .. " inherits from " .. struct.inherits .. " which does not exist")
            end
        end
        for _, field in pairs(struct.fields) do
            if field.type == "tag_reflexive" then
                if not typeExists(field.struct) then
                    error("Struct " .. struct.name .. " has a reflexive field with an unknown struct type: " .. field.struct)
                end
            else
                if not typeExists(field.type) then
                    error("Struct " .. struct.name .. " has a field with an unknown type: " .. field.type)
                end
            end
        end
    end

    return {enums = enums, bitfields = bitfields, structs = structs}
end

return {
    parseDefinition = parseDefinition,
    snakeCaseToCamelCase = snakeCaseToCamelCase,
    camelCaseToSnakeCase = camelCaseToSnakeCase,
    dashAndSentenceToSnakeCase = dashAndSentenceToSnakeCase,
    normalToSnakeCase = normalToSnakeCase,
    dependencies = dependencies
}


