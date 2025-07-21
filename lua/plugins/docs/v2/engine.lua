-- SPDX-License-Identifier: GPL-3.0-only
-- This file is used to document the Lua plugins engine API. It should not be included.

---@meta _
---@diagnostic disable: missing-return 
---@diagnostic disable: unused-local 

Engine = {}


-------------------------------------------------------
-- Engine.cacheFile
-------------------------------------------------------

Engine.cacheFile = {}

-- Get the header of the currently loaded cache file
---@return CacheFileHeader
function Engine.cacheFile.getLoadedCacheFileHeader() end

-- Get the available cache files list
---@return string[] @The cache files list
function Engine.cacheFile.getList() end


-------------------------------------------------------
-- Engine.console
-------------------------------------------------------

Engine.console = {}

-- Print a message of a color in the console; It uses the fmt library to format the message
---@param color ColorARGB @The color of the message
---@param format string @The format string in case the first argument is a color
---@param ... any @The arguments to the format string
---@overload fun(format: string, ...: any)
function Engine.console.print(color, format, ...) end


-------------------------------------------------------
-- Engine.game
-------------------------------------------------------

Engine.game = {}

-- Get the number of ticks since the engine started
---@return integer
function Engine.game.getTickCount() end

-- Get the game engine type
---@return GameEngineType @The game engine type
function Engine.game.getGameEngineType() end


-------------------------------------------------------
-- Engine.script
-------------------------------------------------------

Engine.script = {}

-- Execute a script
---@param script string @The script to execute
function Engine.script.execute(script) end
