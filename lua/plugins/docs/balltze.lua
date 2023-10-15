-- SPDX-License-Identifier: GPL-3.0-only
-- This file is used to document the Lua plugins engine API. It should not be included.

---@meta _

---@diagnostic disable: missing-return 
---@diagnostic disable: unused-local 

Balltze = {}

-------------------------------------------------------
-- Balltze.command
-------------------------------------------------------

Balltze.command = {}

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
---@param callbackFunction function @The function to call when the command is executed
function Balltze.command.registerCommand(name, category, help, paramsHelp, autosave, minArgs, maxArgs, canCallFromConsole, public, callbackFunction) end

-------------------------------------------------------
-- Balltze.event
-------------------------------------------------------

Balltze.event = {}

---@alias BalltzeEventTime
---| "before" #Before the event is triggered
---| "after" #After the event is triggered

---@class BalltzeEventListener
local eventListener = {}

-- Remove the listener from the event
function eventListener:remove() end

---@class BalltzeEvent
---@field cancelled boolean @Whether the event is already cancelled
---@field time BalltzeEventTime @The time the event was triggered 
---@field cancel fun(self: BalltzeEvent) @Cancel the event

Balltze.event.tick = {}

---@class BalltzeTickEventArgs
---@field tickCount integer @The current tick count
---@field deltaTimeMs integer @The number of ticks since the last tick event

---@class BalltzeTickEvent : BalltzeEvent
---@field args BalltzeTickEventArgs @The arguments of the event

---@alias BalltzeTickEventCallback fun(event: BalltzeTickEvent)

-- Subscribe a listener to the tick event
---@param callbackFunction BalltzeTickEventCallback @The function to call when the event is triggered
---@param priority BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.tick.subscribe(callbackFunction, priority) end

-- Remove a listener from the tick event
---@param handle integer @The handle of the event listener
function Balltze.event.tick.removeListener(handle) end

-- Remove all listeners from the tick event
function Balltze.event.tick.removeAllListeners() end

Balltze.event.frame = {}

---@class BalltzeFrameEvent: BalltzeEvent

---@alias BalltzeFrameEventCallback fun(event: BalltzeFrameEvent)

-- Subscribe a listener to the frame event
---@param callbackFunction BalltzeFrameEventCallback @The function to call when the event is triggered
---@param priority BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.frame.subscribe(callbackFunction, priority) end

-- Remove a listener from the frame event
---@param handle integer @The handle of the event listener
function Balltze.event.frame.removeListener(handle) end

-- Remove all listeners from the frame event
function Balltze.event.frame.removeAllListeners() end

Balltze.event.mapFileLoad = {}

---@class BalltzeMapFileLoadEventArgs
---@field mapPath string @The path of the map that was loaded
---@field mapName string @The name of the map that was loaded

---@class BalltzeMapFileLoadEvent: BalltzeEvent
---@field args BalltzeMapFileLoadEventArgs @The arguments of the event

---@alias BalltzeEventListenerPriority
---| "highest" 
---| "above_default"
---| "default" 
---| "lowest" 

---@alias BalltzeMapFileLoadEventCallback fun(event: BalltzeMapFileLoadEvent)

-- Subscribe a listener to the mapFileLoad event
---@param callbackFunction BalltzeMapFileLoadEventCallback @The function to call when the event is triggered
---@param priority BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.mapFileLoad.subscribe(callbackFunction, priority) end

-- Remove a listener from the mapFileLoad event
---@param handle integer @The handle of the event listener
function Balltze.event.mapFileLoad.removeListener(handle) end

-- Remove all listeners from the mapFileLoad event
function Balltze.event.mapFileLoad.removeAllListeners() end

Balltze.event.serverConnect = {}

---@class BalltzeServerConnectEventArgs
---@field address string @The address of the server that was connected to
---@field port integer @The port of the server that was connected to
---@field password string @The password used to connect to the server

---@class BalltzeServerConnectEvent: BalltzeEvent
---@field args BalltzeServerConnectEventArgs @The arguments of the event

---@alias BalltzeServerConnectEventCallback fun(event: BalltzeServerConnectEvent)

-- Subscribe a listener to the serverConnect event
---@param callbackFunction BalltzeServerConnectEventCallback @The function to call when the event is triggered
---@param priority BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.serverConnect.subscribe(callbackFunction, priority) end

-- Remove a listener from the serverConnect event
---@param handle integer @The handle of the event listener
function Balltze.event.serverConnect.removeListener(handle) end

-- Remove all listeners from the serverConnect event
function Balltze.event.serverConnect.removeAllListeners() end

Balltze.event.camera = {}

---@class BalltzeCameraEventArgs
---@field position MetaEnginePoint3D @The position of the camera
---@field orientation table<MetaEnginePoint3D> @The orientation of the camera
---@field pov number @The point of view of the camera

---@class BalltzeCameraEvent: BalltzeEvent
---@field args BalltzeCameraEventArgs @The arguments of the event

---@alias BalltzeCameraEventCallback fun(event: BalltzeCameraEvent)

-- Subscribe a listener to the camera event
---@param callbackFunction BalltzeCameraEventCallback @The function to call when the event is triggered
---@param priority BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.camera.subscribe(callbackFunction, priority) end

-- Remove a listener from the camera event
---@param handle integer @The handle of the event listener
function Balltze.event.camera.removeListener(handle) end

-- Remove all listeners from the camera event
function Balltze.event.camera.removeAllListeners() end

Balltze.event.gameInput = {}

---@class BalltzeGameInputEventArgs
---@field device EngineInputDevice @The device that triggered the event
---@field mapped boolean @Whether the input is mapped
---@field button string @The button that was pressed

---@class BalltzeGameInputEvent: BalltzeEvent
---@field args BalltzeGameInputEventArgs @The arguments of the event

---@alias BalltzeGameInputEventCallback fun(event: BalltzeGameInputEvent)

-- Subscribe a listener to the gameInput event
---@param callbackFunction BalltzeGameInputEventCallback @The function to call when the event is triggered
---@param priority BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.gameInput.subscribe(callbackFunction, priority) end

-- Remove a listener from the gameInput event
---@param handle integer @The handle of the event listener
function Balltze.event.gameInput.removeListener(handle) end

-- Remove all listeners from the gameInput event
function Balltze.event.gameInput.removeAllListeners() end

Balltze.event.mapLoad = {}

---@class BalltzeMapLoadEventArgs
---@field mapPath string @The path of the map that was loaded

---@class BalltzeMapLoadEvent: BalltzeEvent
---@field args BalltzeMapLoadEventArgs @The arguments of the event

---@alias BalltzeMapLoadEventCallback fun(event: BalltzeMapLoadEvent)

-- Subscribe a listener to the mapLoad event
---@param callbackFunction BalltzeMapLoadEventCallback @The function to call when the event is triggered
---@param priority BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.mapLoad.subscribe(callbackFunction, priority) end

-- Remove a listener from the mapLoad event
---@param handle integer @The handle of the event listener
function Balltze.event.mapLoad.removeListener(handle) end

-- Remove all listeners from the mapLoad event
function Balltze.event.mapLoad.removeAllListeners() end

Balltze.event.uiRender = {}

---@class BalltzeUiRenderEvent: BalltzeEvent

---@alias BalltzeUiRenderEventCallback fun(event: BalltzeUiRenderEvent)

-- Subscribe a listener to the uiRender event
---@param callbackFunction BalltzeUiRenderEventCallback @The function to call when the event is triggered
---@param priority BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.uiRender.subscribe(callbackFunction, priority) end

-- Remove a listener from the uiRender event
---@param handle integer @The handle of the event listener
function Balltze.event.uiRender.removeListener(handle) end

-- Remove all listeners from the uiRender event
function Balltze.event.uiRender.removeAllListeners() end

Balltze.event.hudRender = {}

---@class BalltzeHudRenderEvent: BalltzeEvent

---@alias BalltzeHudRenderEventCallback fun(event: BalltzeHudRenderEvent)

-- Subscribe a listener to the hudRender event
---@param callbackFunction BalltzeHudRenderEventCallback @The function to call when the event is triggered
---@param priority BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.hudRender.subscribe(callbackFunction, priority) end

-- Remove a listener from the hudRender event
---@param handle integer @The handle of the event listener
function Balltze.event.hudRender.removeListener(handle) end

-- Remove all listeners from the hudRender event
function Balltze.event.hudRender.removeAllListeners() end

Balltze.event.postCarnageReportRender = {}

---@class BalltzePostCarnageReportRenderEvent: BalltzeEvent

---@alias BalltzePostCarnageReportRenderEventCallback fun(event: BalltzePostCarnageReportRenderEvent)

-- Subscribe a listener to the postCarnageReportRender event
---@param callbackFunction BalltzePostCarnageReportRenderEventCallback @The function to call when the event is triggered
---@param priority BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.postCarnageReportRender.subscribe(callbackFunction, priority) end

-- Remove a listener from the postCarnageReportRender event
---@param handle integer @The handle of the event listener
function Balltze.event.postCarnageReportRender.removeListener(handle) end

-- Remove all listeners from the postCarnageReportRender event
function Balltze.event.postCarnageReportRender.removeAllListeners() end

Balltze.event.hudElementBitmapRender = {}

---@class BalltzeHudElementBitmapRenderEventArgs
---@field vertices EngineWidgetRenderVertices @The vertices of the bitmap
---@field bitmapData MetaEngineTagDataBitmapData @The bitmap data of the bitmap

---@class BalltzeHudElementBitmapRenderEvent: BalltzeEvent
---@field args BalltzeHudElementBitmapRenderEventArgs @The arguments of the event

---@alias BalltzeHudElementBitmapRenderEventCallback fun(event: BalltzeHudElementBitmapRenderEvent)

-- Subscribe a listener to the hudElementBitmapRender event
---@param callbackFunction BalltzeHudElementBitmapRenderEventCallback @The function to call when the event is triggered
---@param priority BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.hudElementBitmapRender.subscribe(callbackFunction, priority) end

-- Remove a listener from the hudElementBitmapRender event
---@param handle integer @The handle of the event listener
function Balltze.event.hudElementBitmapRender.removeListener(handle) end

-- Remove all listeners from the hudElementBitmapRender event
function Balltze.event.hudElementBitmapRender.removeAllListeners() end

Balltze.event.widgetBackgroundRender = {}

---@class BalltzeWidgetBackgroundRenderEventArgs
---@field vertices EngineWidgetRenderVertices @The vertices of the background
---@field widget MetaEngineWidget @The widget of the background

---@class BalltzeWidgetBackgroundRenderEvent: BalltzeEvent
---@field args BalltzeWidgetBackgroundRenderEventArgs @The arguments of the event

---@alias BalltzeWidgetBackgroundRenderEventCallback fun(event: BalltzeWidgetBackgroundRenderEvent)

-- Subscribe a listener to the widgetBackgroundRender event
---@param callbackFunction BalltzeWidgetBackgroundRenderEventCallback @The function to call when the event is triggered
---@param priority BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.widgetBackgroundRender.subscribe(callbackFunction, priority) end

-- Remove a listener from the widgetBackgroundRender event
---@param handle integer @The handle of the event listener
function Balltze.event.widgetBackgroundRender.removeListener(handle) end

-- Remove all listeners from the widgetBackgroundRender event
function Balltze.event.widgetBackgroundRender.removeAllListeners() end

Balltze.event.navpointsRender = {}

---@class BalltzeNavpointsRenderEvent: BalltzeEvent

---@alias BalltzeNavpointsRenderEventCallback fun(event: BalltzeNavpointsRenderEvent)

-- Subscribe a listener to the navpointsRender event
---@param callbackFunction BalltzeNavpointsRenderEventCallback @The function to call when the event is triggered
---@param priority BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.navpointsRender.subscribe(callbackFunction, priority) end

-- Remove a listener from the navpointsRender event
---@param handle integer @The handle of the event listener
function Balltze.event.navpointsRender.removeListener(handle) end

-- Remove all listeners from the navpointsRender event
function Balltze.event.navpointsRender.removeAllListeners() end

-------------------------------------------------------
-- Balltze.features
-------------------------------------------------------

Balltze.features = {}

-- Indexes a tag from another map to load it at the next map load
---@param mapPath string @The path of the map to index the tag from
---@param tagPath string @The path of the tag to index
---@param tagClass EngineTagClass @The class of the tag to index
function Balltze.features.importTagFromMap(mapPath, tagPath, tagClass) end

-- Indexes all tags from another map to load them at the next map load
---@param mapPath string @The path of the map to index the tags from
function Balltze.features.importTagsFromMap(mapPath) end

-- Clears all tag imports
function Balltze.features.clearTagImports() end

-- Reloads the data of a tag
---@param tagHandleOrPath EngineTagHandle|string @The handle or path of the tag to reload
---@param tagClass? EngineTagClass @The class of the tag to reload
function Balltze.features.reloadTagData(tagHandleOrPath, tagClass) end

-- Replace all tag references to a tag by references to another tag
---@param tagHandle EngineTagHandle @The handle of the tag to replace
---@param newTagHandle string @The handle of the tag to replace with
function Balltze.features.replaceTagReferences(tagHandle, newTagHandle) end

-- Copy a tag
---@param tagHandle EngineTagHandle @The handle of the tag to copy
---@param copyName string @The name of the copy
---@return EngineTagHandle @The handle of the copy
function Balltze.features.cloneTag(tagHandle, copyName) end

-- Get a copy of a tag
---@param tagHandle EngineTagHandle @The handle of the tag to get a copy of
---@param copyName string @The name of the copy
---@return EngineTagHandle|nil @The handle of the copy; nil if the copy does not exist
function Balltze.features.getTagCopy(tagHandle, copyName) end

-- Get a an imported tag from another map
---@param mapPath string @The path of the map to get the tag from
---@param tagPath string @The path of the tag to get
---@param tagClass EngineTagClass @The class of the tag to get
---@return EngineTagHandle|nil @The handle of the tag; nil if the tag does not exist
function Balltze.features.getImportedTag(mapPath, tagPath, tagClass) end

-------------------------------------------------------
-- Balltze.logger
-------------------------------------------------------

Balltze.logger = {}

---@class Logger
local logger = {}

-- Log a debug message
---@param message string @The message to log
function logger:debug(message) end

-- Log an info message
---@param message string @The message to log
function logger:info(message) end

-- Log a warning message
---@param message string @The message to log
function logger:warning(message) end

-- Log an error message
---@param message string @The message to log
function logger:error(message) end

-- Log a fatal message
---@param message string @The message to log
function logger:fatal(message) end

-- Set a file to log to
---@param filePath string @The path of the file to log to
function logger:setFile(filePath) end

-- Create a new logger
---@param name string @The name of the logger
---@return Logger @The logger
function Balltze.logger.createLogger(name) end

-------------------------------------------------------
-- Balltze.output
-------------------------------------------------------

Balltze.output = {}

-- Get the font tag for a specific generic font
---@param fontName EngineGenericFont @The name of the generic font 
function Balltze.output.getGenericFont(fontName) end

-- Get the number of pixels a string takes up given a font
---@param text string @The text to measure
---@param font EngineTagHandle @The font to use
---@return integer @The number of pixels the text takes up
function Balltze.output.getTextPixelLength(text, font) end

-- Get the number of pixels a string takes up given a font
---@param font EngineTagHandle @The font to use
---@return integer @The height of the font in pixels
function Balltze.output.getFontPixelHeight(font) end

-- Show text as a subtitle on the screen
---@param text string @The text to show
---@param duration integer @The duration of the subtitle in ticks
---@param color integer @The color of the subtitle
function Balltze.output.addSubtitle(text, duration, color) end

-- Play a BIK video
---@param bikFilePath string @The path of the BIK video to play
function Balltze.output.playBikVideo(bikFilePath) end

-------------------------------------------------------
-- Balltze.chimera
-------------------------------------------------------

Balltze.chimera = {}

function Balltze.chimera.console_is_open() end
function Balltze.chimera.console_out(text, alpha, red, green, blue) end
function Balltze.chimera.delete_object(objectId) end
function Balltze.chimera.draw_text(text, x, y, alpha, red, green, blue, font) end
function Balltze.chimera.execute_script(command) end
function Balltze.chimera.get_dynamic_player(playerIndex) end
function Balltze.chimera.get_global(name) end
function Balltze.chimera.get_object(objectId) end
function Balltze.chimera.get_player(playerIndex) end
function Balltze.chimera.get_tag(tagPath) end
function Balltze.chimera.hud_message(playerIndex, text, duration, alpha, red, green, blue) end
function Balltze.chimera.load_ui_widget(widgetPath) end
function Balltze.chimera.set_callback(event, callback) end
function Balltze.chimera.set_global(name, value) end
function Balltze.chimera.set_timer(timerName, duration, callback) end
function Balltze.chimera.stop_timer(timerName) end
function Balltze.chimera.spawn_object(tagPath, playerIndex, x, y, z, pitch, yaw, roll) end
function Balltze.chimera.tick_rate() end
function Balltze.chimera.ticks() end
function Balltze.chimera.create_font_override(fontName, fontPath) end
function Balltze.chimera.read_bit(address, bit) end
function Balltze.chimera.write_bit(address, bit, value) end
function Balltze.chimera.read_byte(addres) end
function Balltze.chimera.write_byte(address, value) end
function Balltze.chimera.read_short(address) end
function Balltze.chimera.write_short(address, value) end
function Balltze.chimera.read_word(address) end
function Balltze.chimera.write_word(address, value) end
function Balltze.chimera.read_int(address) end
function Balltze.chimera.write_int(address, value) end
function Balltze.chimera.read_dword(address) end
function Balltze.chimera.write_dword(address, value) end
function Balltze.chimera.read_float(address) end
function Balltze.chimera.write_float(address, value) end
function Balltze.chimera.read_char(address) end
function Balltze.chimera.write_char(address, value) end
function Balltze.chimera.read_string(address, length) end
function Balltze.chimera.write_string(address, value) end
