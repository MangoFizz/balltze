-- SPDX-License-Identifier: GPL-3.0-only
-- This file is used to document the Lua plugins engine API. It should not be included.

---@meta _

---@diagnostic disable: missing-return 
---@diagnostic disable: unused-local 

Balltze = {}

Balltze.apiVersion = "2.0.0-beta"

-------------------------------------------------------
-- Commands
-------------------------------------------------------

-- Register a command
---@param name string @The name of the command
---@param category string @The category of the command
---@param help string @The help text of the command
---@param paramsHelp string|nil @The help text of the command parameters
---@param autosave boolean @Whether the command should be saved in the configuration file
---@param minArgs integer @The minimum number of arguments the command accepts
---@param maxArgs integer @The maximum number of arguments the command accepts
---@param canCallFromConsole boolean @Whether the command can be called from the console
---@param public boolean @Whether the command can be called by other plugins
---@param commandFunction fun(args: table<integer, string>): boolean
function Balltze.registerCommand(name, category, help, paramsHelp, autosave, minArgs, maxArgs, canCallFromConsole, public, commandFunction) end

-- Execute a registered command
---@param command string @The command to call
function Balltze.executeCommand(command) end

-- Load command settings
function Balltze.loadSettings() end

-------------------------------------------------------
-- Events
-------------------------------------------------------

-- Add an event listener
---@param eventName string @The name of the event to listen to
---@param callbackFunction EventNoContextCallback @The function to call when the event is triggered
---@param priority? EventListenerPriority @The priority of the callback function; default is "default" duh
---@return EventListener @The handle of the event listener
function Balltze.addEventListener(eventName, callbackFunction, priority) end

-- Remove all event listeners for a specific event
---@param eventName string @The name of the event to remove listeners for
function Balltze.removeEventListeners(eventName) end
