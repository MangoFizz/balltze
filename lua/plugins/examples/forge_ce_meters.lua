-- SPDX-License-Identifier: GPL-3.0-only

local paths = {
    meters = "[shm]\\halo_4\\ui\\hud\\bitmaps\\combined\\meters",
    meters_alphas = "[shm]\\halo_4\\ui\\hud\\bitmaps\\combined\\alphas",
    weapon_icons = "[shm]\\halo_4\\ui\\hud\\bitmaps\\combined\\hud_weapon_icons_final",
    numbers = "[shm]\\halo_4\\ui\\hud\\bitmaps\\combined\\hud_counter_numbers",
    percentage_sign = "[shm]\\halo_4\\ui\\hud\\bitmaps\\combined\\plasma percentage" 
}

local hud_element_render_event_listener = nil
local navpoints_render_event_listener = nil
local is_rendering_navpoints = false
local last_digit_vertices = nil
local digit_count = 0

local function save_digit_vertices(vertices) 
    last_digit_vertices = {
        top_left = {x = vertices.top_left.x, y = vertices.top_left.y},
        top_right = {x = vertices.top_right.x, y = vertices.top_right.y},
        bottom_left = {x = vertices.bottom_left.x, y = vertices.bottom_left.y},
        bottom_right = {x = vertices.bottom_right.x, y = vertices.bottom_right.y}
    }
end

local function starts_with(str, prefix)
    return string.sub(str, 1, string.len(prefix)) == prefix
end

local function rotate_vertex(vertex, cx, cy, angle)
    local cos_a = math.cos(angle)
    local sin_a = math.sin(angle)
    local new_x = cos_a * (vertex.x - cx) - sin_a * (vertex.y - cy) + cx
    local new_y = sin_a * (vertex.x - cx) + cos_a * (vertex.y - cy) + cy
    return {x = new_x, y = new_y}
end

local function rotate_rectangle(vertices, angle)
    local center_x = vertices.top_left.x + (vertices.top_right.x - vertices.top_left.x) / 2
    local center_y = vertices.top_left.y + (vertices.bottom_left.y - vertices.top_left.y) / 2

    local rotate = function(vertex) 
        local new_vertex = rotate_vertex(vertex, center_x, center_y, angle)
        vertex.x = new_vertex.x
        vertex.y = new_vertex.y
    end

    rotate(vertices.top_left)
    rotate(vertices.top_right)
    rotate(vertices.bottom_left)
    rotate(vertices.bottom_right)
end

local function skew_rectangle_horizontally(vertices, angle)
    local skew = function(top_vertex, bottom_vertex) 
        local center_x = top_vertex.x
        local center_y = top_vertex.y + (bottom_vertex.y - top_vertex.y) / 2
        local top = rotate_vertex(top_vertex, center_x, center_y, angle)
        local bottom = rotate_vertex(bottom_vertex, center_x, center_y, angle)
        top_vertex.x = top.x
        top_vertex.y = top.y
        bottom_vertex.x = bottom.x
        bottom_vertex.y = bottom.y
    end

    skew(vertices.top_left, vertices.bottom_left)
    skew(vertices.top_right, vertices.bottom_right)
end

local function displace_rectangle(vertices, x, y)
    local displace = function(vertex) 
        vertex.x = vertex.x + x
        vertex.y = vertex.y + y
    end

    displace(vertices.top_left)
    displace(vertices.top_right)
    displace(vertices.bottom_left)
    displace(vertices.bottom_right)
end

local function on_hud_element_bitmap_render(event)
    if event.time == "before" then
        local tag_handle = event.args.bitmap_data.bitmap_tag_handle
        local tag_entry = engine.get_tag(tag_handle) 

        local found = false
        for _, path in pairs(paths) do
            if(starts_with(tag_entry.path, path)) then
                found = true
                break
            end
        end
        
        if found and not is_rendering_navpoints then
            -- Get the angle to rotate the rectangle
            local screen_resolution = engine.get_resolution()
            local actual_width = (640 / (4 / 3)) * (screen_resolution.width / screen_resolution.height)
            local rightSide = actual_width / 2 < event.args.vertices.top_left.x

            if tag_entry.path == paths.numbers then
                if not last_digit_vertices then
                    save_digit_vertices(event.args.vertices)
                    digit_count = 1
                else
                    rightSide = actual_width / 2 < last_digit_vertices.top_left.x
                    local rectangle_height = last_digit_vertices.bottom_left.y - last_digit_vertices.top_left.y
                    local offset = (rectangle_height * 0.05) * digit_count * -1
                    if rightSide then
                        offset = -offset
                    end
                    displace_rectangle(event.args.vertices, 0, offset)
                    digit_count = digit_count + 1
                end
            else
                last_digit_vertices = nil
            end

            local rotation_angle = 6
            local skew_angle = -6 
            if rightSide then
                rotation_angle = -6
                skew_angle = 6
            end
            
            -- Rotate the rectangle
            rotate_rectangle(event.args.vertices, math.rad(rotation_angle))

            -- Skew the rectangle horizontally
            skew_rectangle_horizontally(event.args.vertices, math.rad(skew_angle))
        end
    end
end

local function on_navpoints_render(event) 
    if event.time == "before" then
        is_rendering_navpoints = true
    else 
        is_rendering_navpoints = false
    end
end

local function on_map_load(event)
    if event.time == "before" then
        if(event.args.map_name == "forge_island_dev") then
            hud_element_render_event_listener = balltze.event.hud_element_bitmap_render.subscribe(on_hud_element_bitmap_render, "highest")
            navpoints_render_event_listener = balltze.event.navpoints_render.subscribe(on_navpoints_render, "highest")
        else 
            if(hud_element_render_event_listener) then
                hud_element_render_event_listener:remove()
            end
            if(navpoints_render_event_listener) then
                navpoints_render_event_listener:remove()
            end
        end
    end
end

function plugin_init() 
    return true
end

function plugin_load() 
    balltze.event.map_load.subscribe(on_map_load, "highest")
end
