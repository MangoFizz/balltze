-- SPDX-License-Identifier: GPL-3.0-only

local meters_path = "[shm]\\halo_4\\ui\\hud\\bitmaps\\combined\\meters"
local alphas_path = "[shm]\\halo_4\\ui\\hud\\bitmaps\\combined\\alphas"

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

local function on_hud_element_bitmap_render(event)
    if event.time == "before" then
        local tag_handle = event.args.bitmap_data.bitmap_tag_handle
        local tag_entry = engine.get_tag(tag_handle) 
        
        if(starts_with(tag_entry.path, alphas_path) or starts_with(tag_entry.path, meters_path)) then
            -- Get the angle to rotate the rectangle
            local screen_resolution = engine.get_resolution()
            local rotation_angle = 6
            local skew_angle = -6 
            local actual_width = (640 / (4 / 3)) * (screen_resolution.width / screen_resolution.height)
            if actual_width / 2 < event.args.vertices.top_left.x then
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

function plugin_init() 
    return true
end

function plugin_load() 
    balltze.event.hud_element_bitmap_render.subscribe(on_hud_element_bitmap_render, "highest")
end
