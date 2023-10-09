-- SPDX-License-Identifier: GPL-3.0-only

local paths = {
    meters = "[shm]\\halo_4\\ui\\hud\\bitmaps\\combined\\meters",
    metersAlphas = "[shm]\\halo_4\\ui\\hud\\bitmaps\\combined\\alphas",
    weaponIcons = "[shm]\\halo_4\\ui\\hud\\bitmaps\\combined\\hud_weapon_icons_final",
    numbers = "[shm]\\halo_4\\ui\\hud\\bitmaps\\combined\\hud_counter_numbers",
    percentageSign = "[shm]\\halo_4\\ui\\hud\\bitmaps\\combined\\plasma percentage" 
}

local hudElementRenderEventListener = nil
local navpointsRenderEventListener = nil
local isRenderingNavpoints = false
local lastDigitVertices = nil
local digitCount = 0

local function saveDigitVertices(vertices) 
    lastDigitVertices = {
        topLeft = {x = vertices.topLeft.x, y = vertices.topLeft.y},
        topRight = {x = vertices.topRight.x, y = vertices.topRight.y},
        bottomLeft = {x = vertices.bottomLeft.x, y = vertices.bottomLeft.y},
        bottomRight = {x = vertices.bottomRight.x, y = vertices.bottomRight.y}
    }
end

local function startsWith(str, prefix)
    return string.sub(str, 1, string.len(prefix)) == prefix
end

local function rotateVertex(vertex, cx, cy, angle)
    local cosA = math.cos(angle)
    local sinA = math.sin(angle)
    local newX = cosA * (vertex.x - cx) - sinA * (vertex.y - cy) + cx
    local newY = sinA * (vertex.x - cx) + cosA * (vertex.y - cy) + cy
    return {x = newX, y = newY}
end

local function rotateRectangle(vertices, angle)
    local centerX = vertices.topLeft.x + (vertices.topRight.x - vertices.topLeft.x) / 2
    local centerY = vertices.topLeft.y + (vertices.bottomLeft.y - vertices.topLeft.y) / 2

    local rotate = function(vertex) 
        local newVertex = rotateVertex(vertex, centerX, centerY, angle)
        vertex.x = newVertex.x
        vertex.y = newVertex.y
    end

    rotate(vertices.topLeft)
    rotate(vertices.topRight)
    rotate(vertices.bottomLeft)
    rotate(vertices.bottomRight)
end

local function skewRectangleHorizontally(vertices, angle)
    local skew = function(topVertex, bottomVertex) 
        local centerX = topVertex.x
        local centerX = topVertex.y + (bottomVertex.y - topVertex.y) / 2
        local top = rotateVertex(topVertex, centerX, centerX, angle)
        local bottom = rotateVertex(bottomVertex, centerX, centerX, angle)
        topVertex.x = top.x
        topVertex.y = top.y
        bottomVertex.x = bottom.x
        bottomVertex.y = bottom.y
    end

    skew(vertices.topLeft, vertices.bottomLeft)
    skew(vertices.topRight, vertices.bottomRight)
end

local function displaceRectangle(vertices, x, y)
    local displace = function(vertex) 
        vertex.x = vertex.x + x
        vertex.y = vertex.y + y
    end

    displace(vertices.topLeft)
    displace(vertices.topRight)
    displace(vertices.bottomLeft)
    displace(vertices.bottomRight)
end

local function onHudElementBitmapRender(event)
    if event.time == "before" then
        local tagHandle = event.args.bitmapData.bitmapTagHandle
        local tagEntry = Engine.tag.getTag(tagHandle) 

        local found = false
        for _, path in pairs(paths) do
            if(startsWith(tagEntry.path, path)) then
                found = true
                break
            end
        end
        
        if found and not isRenderingNavpoints then
            -- Get the angle to rotate the rectangle
            local screenResolution = Engine.core.getResolution()
            local actualWidth = (640 / (4 / 3)) * (screenResolution.width / screenResolution.height)
            local rightSide = actualWidth / 2 < event.args.vertices.topLeft.x

            if tagEntry.path == paths.numbers then
                if not lastDigitVertices then
                    saveDigitVertices(event.args.vertices)
                    digitCount = 1
                else
                    rightSide = actualWidth / 2 < lastDigitVertices.topLeft.x
                    local rectangleHeight = lastDigitVertices.bottomLeft.y - lastDigitVertices.topLeft.y
                    local offset = (rectangleHeight * 0.05) * digitCount * -1
                    if rightSide then
                        offset = -offset
                    end
                    displaceRectangle(event.args.vertices, 0, offset)
                    digitCount = digitCount + 1
                end
            else
                lastDigitVertices = nil
            end

            local rotationAngle = 6
            local skewAngle = -6 
            if rightSide then
                rotationAngle = -6
                skewAngle = 6
            end
            
            -- Rotate the rectangle
            rotateRectangle(event.args.vertices, math.rad(rotationAngle))

            -- Skew the rectangle horizontally
            skewRectangleHorizontally(event.args.vertices, math.rad(skewAngle))
        end
    end
end

local function onNavpointsRender(event) 
    if event.time == "before" then
        isRenderingNavpoints = true
    else 
        isRenderingNavpoints = false
    end
end

local function onMapLoad(event)
    if event.time == "before" then
        if(event.args.mapName == "forge_island_dev") then
            hudElementRenderEventListener = Balltze.event.hudElementBitmapRender.subscribe(onHudElementBitmapRender, "highest")
            navpointsRenderEventListener = Balltze.event.navpointsRender.subscribe(onNavpointsRender, "highest")
        else 
            if(hudElementRenderEventListener) then
                hudElementRenderEventListener:remove()
            end
            if(navpointsRenderEventListener) then
                navpointsRenderEventListener:remove()
            end
        end
    end
end

function PluginInit() 
    return true
end

function PluginLoad() 
    Balltze.event.mapLoad.subscribe(onMapLoad, "highest")
end
