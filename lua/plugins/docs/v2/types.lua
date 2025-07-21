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
