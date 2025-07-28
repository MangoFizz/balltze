-- SPDX-License-Identifier: GPL-3.0-only

---@meta _

-------------------------------------------------------
-- Events types
-------------------------------------------------------

---@alias EventListenerPriority
---| "highest" 
---| "above_default"
---| "default" 
---| "lowest" 

---@alias EventNoContextCallback fun(): nil

---@class EventListener
---@field event string Name of the event this listener is subscribed to
---@field priority EventListenerPriority Priority of the listener
---@field remove fun(): nil Function to remove the listener

---@class MapLoadEvent
local MapLoadEvent = {}

-- Get the name of the map being loaded
---@return string
---@nodiscard
function MapLoadEvent:mapName() end

---@class PlayerInputEvent
local PlayerInputEvent = {}

-- Get the name of the input device
---@return "keyboard"|"gamepad"
---@nodiscard
function PlayerInputEvent:getDevice() end

-- Get the key code if the input device is a keyboard
---@return integer
---@nodiscard
function PlayerInputEvent:getKeyCode() end

-- Get the gamepad button if the input device is a gamepad
---@return integer
---@nodiscard
function PlayerInputEvent:getGamepadButton() end

-- Check if the input event is mapped to a game action
---@return boolean
---@nodiscard
function PlayerInputEvent:isMapped() end

-- Cancel the input event
function PlayerInputEvent:cancel() end
