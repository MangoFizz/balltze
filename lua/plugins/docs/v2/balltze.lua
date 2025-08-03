-- SPDX-License-Identifier: GPL-3.0-only
-- This file is used to document the Lua plugins engine API. It should not be included.

---@meta _

---@diagnostic disable: missing-return 
---@diagnostic disable: unused-local 

Balltze = {}

Balltze.apiVersion = "2.0.0-beta"


-------------------------------------------------------
-- Miscellaneous functions
-------------------------------------------------------

---@class BalltzeTimestamp
local BalltzeTimestamp = {}

-- Get the number of milliseconds since the timestamp was set
---@return integer @The number of milliseconds
function BalltzeTimestamp:getElapsedMilliseconds() end

-- Get the number of seconds since the timestamp was set
---@return integer @The number of seconds
function BalltzeTimestamp:getElapsedSeconds() end

-- Reset the timestamp
function BalltzeTimestamp:reset() end

-- Create a timestamp
---@return BalltzeTimestamp @The timestamp
function Balltze.setTimestamp() end

---Set content to the clipboard
---@param content string Content to copy
function Balltze.setClipboard(content) end

---Get content from the clipboard
---@return string Content from the clipboard
function Balltze.getClipboard() end


-------------------------------------------------------
-- Configurations functions
-------------------------------------------------------

---@class BalltzeConfig
local BalltzeConfig = {}

-- Save configs to its file
function BalltzeConfig:save() end

-- Load configs from its file
function BalltzeConfig:load() end

-- Check if a config exists
---@param key string @The key of the config
---@return boolean @Whether the config exists
function BalltzeConfig:exists(key) end

-- Remove a config
---@param key string @The key of the config
function BalltzeConfig:remove(key) end

-- Get an integer config
---@param key string @The key of the config
---@return integer|nil @The value of the config
function BalltzeConfig:getInteger(key) end

-- Get a float config
---@param key string @The key of the config
---@return number|nil @The value of the config
function BalltzeConfig:getNumber(key) end

-- Get a string config
---@param key string @The key of the config
---@return string|nil @The value of the config
function BalltzeConfig:getString(key) end

-- Get a boolean config
---@param key string @The key of the config
---@return boolean|nil @The value of the config
function BalltzeConfig:getBoolean(key) end

-- Set a config
---@param key string @The key of the config
---@param value any @The value of the config
function BalltzeConfig:set(key, value) end

-- Get a configuration file 
---@param path string @The path of the configuration file
---@return BalltzeConfig @The configuration object
function Balltze.openConfigFile(path) end


-------------------------------------------------------
-- Console command functions
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

---@alias NoContextCallback fun()
---@alias MapLoadEventCallback fun(event: MapLoadEvent)
---@alias PlayerInputEventCallback fun(event: PlayerInputEvent)
---@alias WidgetEventDispatchEventCallback fun(event: WidgetEventDispatchEvent)

-- Add an event listener
---@param eventName string @The name of the event to listen to
---@param callbackFunction EventNoContextCallback @The function to call when the event is triggered
---@param priority? EventListenerPriority @The priority of the callback function; default is "default" duh
---@return EventListener @The handle of the event listener
---@overload fun(eventName: "frame", callbackFunction: NoContextCallback, priority?: EventListenerPriority): EventListener
---@overload fun(eventName: "frame_begin", callbackFunction: NoContextCallback, priority?: EventListenerPriority): EventListener
---@overload fun(eventName: "frame_end", callbackFunction: NoContextCallback, priority?: EventListenerPriority): EventListener
---@overload fun(eventName: "tick", callbackFunction: NoContextCallback, priority?: EventListenerPriority): EventListener
---@overload fun(eventName: "map_load", callbackFunction: MapLoadEventCallback, priority?: EventListenerPriority): EventListener
---@overload fun(eventName: "map_loaded", callbackFunction: MapLoadEventCallback, priority?: EventListenerPriority): EventListener
---@overload fun(eventName: "player_input", callbackFunction: PlayerInputEventCallback, priority?: EventListenerPriority): EventListener
---@overload fun(eventName: "widget_event_dispatch", callbackFunction: WidgetEventDispatchEventCallback, priority?: EventListenerPriority): EventListener
function Balltze.addEventListener(eventName, callbackFunction, priority) end

-- Remove all event listeners for a specific event
---@param eventName string @The name of the event to remove listeners for
function Balltze.removeEventListeners(eventName) end


-------------------------------------------------------
-- Miscellaneous
-------------------------------------------------------

---@class Timer
---@field stop fun() @Stop the timer and removes it

-- Create a timer
---@param interval integer @The interval of the timer in milliseconds
---@param callback fun() @The function to call when the timer elapses
---@return Timer @Handle of the timer
function Balltze.setTimer(interval, callback) end


-------------------------------------------------------
-- Filesystem functions
-------------------------------------------------------

Balltze.filesystem = {}

-- Create a directory
---@param path string @The path of the directory to create
function Balltze.filesystem.createDirectory(path) end

-- Remove a directory
---@param path string @The path of the directory to remove
function Balltze.filesystem.removeDirectory(path) end

-- List the files in a directory
---@param path string @The path of the directory to list
---@return table<string> @The list of files in the directory
function Balltze.filesystem.listDirectory(path) end

-- Check if a directory exists
---@param path string @The path of the directory to check
---@return boolean @Whether the directory exists
function Balltze.filesystem.directoryExists(path) end

-- Write a file
---@param path string @The path of the file to write
---@param data string @The data to write
function Balltze.filesystem.writeFile(path, data) end

-- Read a file
---@param path string @The path of the file to read
---@return string @The data read
function Balltze.filesystem.readFile(path) end

-- Delete a file
---@param path string @The path of the file to delete
function Balltze.filesystem.deleteFile(path) end

-- Check if a file exists
---@param path string @The path of the file to check
---@return boolean @Whether the file exists
function Balltze.filesystem.fileExists(path) end

-- Get the game directory path
---@return string @The game directory path
function Balltze.filesystem.getGamePath() end

-- Get the player profiles directory path
---@return string @The player profiles path
function Balltze.filesystem.getProfilesPath() end

-- Get the game data directory path
---@return string @The game data path
function Balltze.filesystem.getPluginPath() end


-------------------------------------------------------
-- Balltze.logger
-------------------------------------------------------

Balltze.logger = {}

-- Log a debug message
---@param format string @The format of the message
---@vararg any @The arguments of the message
function Balltze.logger.debug(format, ...) end

-- Log an info message
---@param format string @The format of the message
---@vararg any @The arguments of the message
function Balltze.logger.info(format, ...) end

-- Log a warning message
---@param format string @The format of the message
---@vararg any @The arguments of the message
function Balltze.logger.warning(format, ...) end

-- Log an error message
---@param format string @The format of the message
---@vararg any @The arguments of the message
function Balltze.logger.error(format, ...) end

-- Log a fatal message
---@param format string @The format of the message
---@vararg any @The arguments of the message
function Balltze.logger.fatal(format, ...) end

-- Set whether to log to the in-game console
---@param setting? boolean @Whether to log to the in-game console
---@return boolean|nil @The current setting if no argument is provided
function Balltze.logger.muteConsole(setting) end

-- Set whether to log debug messages
---@param setting? boolean @Whether to log debug messages
---@return boolean|nil @The current setting if no argument is provided
function Balltze.logger.muteDebug(setting) end
