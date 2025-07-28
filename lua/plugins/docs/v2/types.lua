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
---@field mapName fun(): string Get the name of the map being loaded

---@class PlayerInputEvent
---@field getDevice fun(): string Get the name of the input device
---@field getKeyCode fun(): integer Get the key code if the input device is a keyboard
---@field getGamepadButton fun(): integer Get the gamepad button if the input device is a gamepad
---@field isMapped fun(): boolean Check if the input event is mapped to a game action
---@field cancel fun() Cancel the input event
