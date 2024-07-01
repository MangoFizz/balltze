-- SPDX-License-Identifier: GPL-3.0-only
-- This file is used to document the Lua plugins engine API. It should not be included.

---@meta _

---@diagnostic disable: missing-return 
---@diagnostic disable: unused-local 

Balltze = {}

Balltze.apiVersion = "1.1.0"

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
---@param commandFunction fun(args: table<integer, string>): boolean
function Balltze.command.registerCommand(name, category, help, paramsHelp, autosave, minArgs, maxArgs, canCallFromConsole, public, commandFunction) end

-------------------------------------------------------
-- Balltze.event
-------------------------------------------------------

Balltze.event = {}

---@alias BalltzeEventTime
---| "before" #Before the event is triggered
---| "after" #After the event is triggered

---@alias BalltzeEventListenerPriority
---| "highest" 
---| "above_default"
---| "default" 
---| "lowest" 

---@class BalltzeEventListener
local eventListener = {}

-- Remove the listener from the event
function eventListener:remove() end

---@class BalltzeEvent
---@field cancelled boolean @Whether the event is already cancelled
---@field time BalltzeEventTime @The time the event was triggered 
---@field cancel fun(self: BalltzeEvent) @Cancel the event

Balltze.event.camera = {}

---@class BalltzeCameraEventArgs
---@field position EngineCameraData @The position of the camera
---@field type EngineCameraType @The type of the camera

---@class MetaBalltzeCameraEventArgs: BalltzeCameraEventArgs

---@class BalltzeCameraEvent: BalltzeEvent
---@field args MetaBalltzeCameraEventArgs @The arguments of the event

---@alias BalltzeCameraEventCallback fun(event: BalltzeCameraEvent)

-- Subscribe a listener to the camera event
---@param callbackFunction BalltzeCameraEventCallback @The function to call when the event is triggered
---@param priority? BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.camera.subscribe(callbackFunction, priority) end

-- Remove a listener from the camera event
---@param handle integer @The handle of the event listener
function Balltze.event.camera.removeListener(handle) end

-- Remove all listeners from the camera event
function Balltze.event.camera.removeAllListeners() end

Balltze.event.frame = {}

---@class BalltzeFrameEvent: BalltzeEvent

---@alias BalltzeFrameEventCallback fun(event: BalltzeFrameEvent)

-- Subscribe a listener to the frame event
---@param callbackFunction BalltzeFrameEventCallback @The function to call when the event is triggered
---@param priority? BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.frame.subscribe(callbackFunction, priority) end

-- Remove a listener from the frame event
---@param handle integer @The handle of the event listener
function Balltze.event.frame.removeListener(handle) end

-- Remove all listeners from the frame event
function Balltze.event.frame.removeAllListeners() end

Balltze.event.gameInput = {}

---@class BalltzeGameInputEventArgs
---@field device EngineInputDevice @The device that triggered the event
---@field mapped boolean @Whether the input is mapped to a game action
---@field button string @The button that was pressed

---@class MetaBalltzeGameInputEventArgs: BalltzeGameInputEventArgs

---@class BalltzeGameInputEvent: BalltzeEvent
---@field args MetaBalltzeGameInputEventArgs @The arguments of the event

---@alias BalltzeGameInputEventCallback fun(event: BalltzeGameInputEvent)

-- Subscribe a listener to the gameInput event
---@param callbackFunction BalltzeGameInputEventCallback @The function to call when the event is triggered
---@param priority? BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.gameInput.subscribe(callbackFunction, priority) end

-- Remove a listener from the gameInput event
---@param handle integer @The handle of the event listener
function Balltze.event.gameInput.removeListener(handle) end

-- Remove all listeners from the gameInput event
function Balltze.event.gameInput.removeAllListeners() end

Balltze.event.hudHoldForActionMessage = {}

---@alias BalltzeHudHoldForActionMessageSlice
---| "message" 
---| "button_name_left_quote"
---| "button_name_right_quote"
---| "button_name"
---| "weapon_icon"

---@class BalltzeHudHoldForActionMessageOffset
---@field y integer 
---@field x integer 

---@alias BalltzeHudHoldForActionMessageButtonType
---| "button" 
---| "axis"

---@alias BalltzeHudHoldForActionMessageButtonAxisDirection
---| "positive"
---| "negative"

---@class BalltzeHudHoldForActionMessageButton
---@field input EngineInputDevice
---@field type BalltzeHudHoldForActionMessageButtonType
---@field index integer
---@field direction BalltzeHudHoldForActionMessageButtonAxisDirection

---@class MetaBalltzeHudHoldForActionMessageButton: BalltzeHudHoldForActionMessageButton

---@class BalltzeHudHoldForActionMessageEventArgs
---@field slice BalltzeHudHoldForActionMessageSlice
---@field offset BalltzeHudHoldForActionMessageOffset
---@field color EngineColorARGBInt
---@field text lightuserdata
---@field button MetaBalltzeHudHoldForActionMessageButton?

---@class MetaBalltzeHudHoldForActionMessageEventArgs: BalltzeHudHoldForActionMessageEventArgs

---@class BalltzeHudHoldForActionMessageEvent: BalltzeEvent
---@field args MetaBalltzeHudHoldForActionMessageEventArgs @The arguments of the event

---@alias BalltzeHudHoldForActionMessageEventArgsCallback fun(event: BalltzeHudHoldForActionMessageEvent)

-- Subscribe a listener to the hudHoldForActionMessage event
---@param callbackFunction BalltzeHudHoldForActionMessageEventArgsCallback @The function to call when the event is triggered
---@param priority? BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.hudHoldForActionMessage.subscribe(callbackFunction, priority) end

-- Remove a listener from the hudHoldForActionMessage event
---@param handle integer @The handle of the event listener
function Balltze.event.hudHoldForActionMessage.removeListener(handle) end

-- Remove all listeners from the hudHoldForActionMessage event
function Balltze.event.hudHoldForActionMessage.removeAllListeners() end

Balltze.event.mapFileLoad = {}

---@class BalltzeMapFileLoadEventArgs
---@field mapPath string @The path of the map that was loaded
---@field mapName string @The name of the map that was loaded

---@class MetaBalltzeMapFileLoadEventArgs: BalltzeMapFileLoadEventArgs

---@class BalltzeMapFileLoadEvent: BalltzeEvent
---@field args MetaBalltzeMapFileLoadEventArgs @The arguments of the event

---@alias BalltzeMapFileLoadEventCallback fun(event: BalltzeMapFileLoadEvent)

-- Subscribe a listener to the mapFileLoad event
---@param callbackFunction BalltzeMapFileLoadEventCallback @The function to call when the event is triggered
---@param priority? BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.mapFileLoad.subscribe(callbackFunction, priority) end

-- Remove a listener from the mapFileLoad event
---@param handle integer @The handle of the event listener
function Balltze.event.mapFileLoad.removeListener(handle) end

-- Remove all listeners from the mapFileLoad event
function Balltze.event.mapFileLoad.removeAllListeners() end

Balltze.event.mapLoad = {}

---@class BalltzeMapLoadEventArgs
---@field name string @The name of the map that was loaded

---@class MetaBalltzeMapLoadEventArgs: BalltzeMapLoadEventArgs

---@class BalltzeMapLoadEvent: BalltzeEvent
---@field args MetaBalltzeMapLoadEventArgs @The arguments of the event

---@alias BalltzeMapLoadEventCallback fun(event: BalltzeMapLoadEvent)

-- Subscribe a listener to the mapLoad event
---@param callbackFunction BalltzeMapLoadEventCallback @The function to call when the event is triggered
---@param priority? BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.mapLoad.subscribe(callbackFunction, priority) end

-- Remove a listener from the mapLoad event
---@param handle integer @The handle of the event listener
function Balltze.event.mapLoad.removeListener(handle) end

-- Remove all listeners from the mapLoad event
function Balltze.event.mapLoad.removeAllListeners() end

Balltze.event.networkGameChatMessage = {}

---@class BalltzeNetworkGameChatMessageEventArgs
---@field message lightuserdata @The message that was sent
---@field playerId integer @The player that sent the message
---@field type EngineNetworkGameMessageHudChatType @The type of the message

---@class MetaBalltzeNetworkGameChatMessageEventArgs: BalltzeNetworkGameChatMessageEventArgs

---@class BalltzeNetworkGameChatMessageEvent: BalltzeEvent
---@field args MetaBalltzeNetworkGameChatMessageEventArgs @The arguments of the event

---@alias BalltzeNetworkGameChatMessageEventCallback fun(event: BalltzeNetworkGameChatMessageEvent)

-- Subscribe a listener to the networkGameChatMessage event
---@param callbackFunction BalltzeNetworkGameChatMessageEventCallback @The function to call when the event is triggered
---@param priority? BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.networkGameChatMessage.subscribe(callbackFunction, priority) end

-- Remove a listener from the networkGameChatMessage event
---@param handle integer @The handle of the event listener
function Balltze.event.networkGameChatMessage.removeListener(handle) end

-- Remove all listeners from the networkGameChatMessage event
function Balltze.event.networkGameChatMessage.removeAllListeners() end

Balltze.event.objectDamage = {}

---@class BalltzeObjectDamageEventArgs
---@field object EngineObjectHandle @The object that was damaged
---@field damageEffect EngineTagHandle @The damage effect that was applied
---@field multiplier number @The damage multiplier that was applied
---@field causerPlayer EnginePlayerHandle @The player that caused the damage
---@field causerObject EngineObjectHandle @The object that caused the damage

---@class MetaBalltzeObjectDamageEventArgs: BalltzeObjectDamageEventArgs

---@class BalltzeObjectDamageEvent: BalltzeEvent
---@field args MetaBalltzeObjectDamageEventArgs @The arguments of the event

---@alias BalltzeObjectDamageEventCallback fun(event: BalltzeObjectDamageEvent)

-- Subscribe a listener to the objectDamage event
---@param callbackFunction BalltzeObjectDamageEventCallback @The function to call when the event is triggered
---@param priority? BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.objectDamage.subscribe(callbackFunction, priority) end

-- Remove a listener from the objectDamage event
---@param handle integer @The handle of the event listener
function Balltze.event.objectDamage.removeListener(handle) end

-- Remove all listeners from the objectDamage event
function Balltze.event.objectDamage.removeAllListeners() end

Balltze.event.rconMessage = {}

---@class BalltzeRconMessageEventArgs
---@field message string @The message that was sent

---@class MetaBalltzeRconMessageEventArgs: BalltzeRconMessageEventArgs

---@class BalltzeRconMessageEvent: BalltzeEvent
---@field args MetaBalltzeRconMessageEventArgs @The arguments of the event

---@alias BalltzeRconMessageEventCallback fun(event: BalltzeRconMessageEvent)

-- Subscribe a listener to the rconMessage event
---@param callbackFunction BalltzeRconMessageEventCallback @The function to call when the event is triggered
---@param priority? BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.rconMessage.subscribe(callbackFunction, priority) end

-- Remove a listener from the rconMessage event
---@param handle integer @The handle of the event listener
function Balltze.event.rconMessage.removeListener(handle) end

-- Remove all listeners from the rconMessage event
function Balltze.event.rconMessage.removeAllListeners() end

Balltze.event.uiRender = {}

---@class BalltzeUiRenderEvent: BalltzeEvent

---@alias BalltzeUiRenderEventCallback fun(event: BalltzeUiRenderEvent)

-- Subscribe a listener to the uiRender event
---@param callbackFunction BalltzeUiRenderEventCallback @The function to call when the event is triggered
---@param priority? BalltzeEventListenerPriority @The priority of the callback function
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
---@param priority? BalltzeEventListenerPriority @The priority of the callback function
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
---@param priority? BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.postCarnageReportRender.subscribe(callbackFunction, priority) end

-- Remove a listener from the postCarnageReportRender event
---@param handle integer @The handle of the event listener
function Balltze.event.postCarnageReportRender.removeListener(handle) end

-- Remove all listeners from the postCarnageReportRender event
function Balltze.event.postCarnageReportRender.removeAllListeners() end

Balltze.event.hudElementBitmapRender = {}

---@class BalltzeHudElementBitmapRenderEventArgs
---@field vertices EngineUIWidgetRenderVertices @The vertices of the bitmap
---@field bitmapData MetaEngineTagDataBitmapData @The bitmap data of the bitmap

---@class MetaBalltzeHudElementBitmapRenderEventArgs: BalltzeHudElementBitmapRenderEventArgs

---@class BalltzeHudElementBitmapRenderEvent: BalltzeEvent
---@field args MetaBalltzeHudElementBitmapRenderEventArgs @The arguments of the event

---@alias BalltzeHudElementBitmapRenderEventCallback fun(event: BalltzeHudElementBitmapRenderEvent)

-- Subscribe a listener to the hudElementBitmapRender event
---@param callbackFunction BalltzeHudElementBitmapRenderEventCallback @The function to call when the event is triggered
---@param priority? BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.hudElementBitmapRender.subscribe(callbackFunction, priority) end

-- Remove a listener from the hudElementBitmapRender event
---@param handle integer @The handle of the event listener
function Balltze.event.hudElementBitmapRender.removeListener(handle) end

-- Remove all listeners from the hudElementBitmapRender event
function Balltze.event.hudElementBitmapRender.removeAllListeners() end

Balltze.event.uiWidgetBackgroundRender = {}

---@class BalltzeUIWidgetBackgroundRenderEventArgs
---@field vertices EngineUIWidgetRenderVertices @The vertices of the background
---@field widget MetaEngineWidget @The widget of the background

---@class MetaBalltzeUIWidgetBackgroundRenderEventArgs: BalltzeUIWidgetBackgroundRenderEventArgs

---@class BalltzeUIWidgetBackgroundRenderEvent: BalltzeEvent
---@field args MetaBalltzeUIWidgetBackgroundRenderEventArgs @The arguments of the event

---@alias BalltzeUIWidgetBackgroundRenderEventCallback fun(event: BalltzeUIWidgetBackgroundRenderEvent)

-- Subscribe a listener to the uiWidgetBackgroundRender event
---@param callbackFunction BalltzeUIWidgetBackgroundRenderEventCallback @The function to call when the event is triggered
---@param priority? BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.uiWidgetBackgroundRender.subscribe(callbackFunction, priority) end

-- Remove a listener from the uiWidgetBackgroundRender event
---@param handle integer @The handle of the event listener
function Balltze.event.uiWidgetBackgroundRender.removeListener(handle) end

-- Remove all listeners from the uiWidgetBackgroundRender event
function Balltze.event.uiWidgetBackgroundRender.removeAllListeners() end

Balltze.event.navpointsRender = {}

---@class BalltzeNavpointsRenderEvent: BalltzeEvent

---@alias BalltzeNavpointsRenderEventCallback fun(event: BalltzeNavpointsRenderEvent)

-- Subscribe a listener to the navpointsRender event
---@param callbackFunction BalltzeNavpointsRenderEventCallback @The function to call when the event is triggered
---@param priority? BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.navpointsRender.subscribe(callbackFunction, priority) end

-- Remove a listener from the navpointsRender event
---@param handle integer @The handle of the event listener
function Balltze.event.navpointsRender.removeListener(handle) end

-- Remove all listeners from the navpointsRender event
function Balltze.event.navpointsRender.removeAllListeners() end

Balltze.event.serverConnect = {}

---@class BalltzeServerConnectEventArgs
---@field address string @The address of the server that was connected to
---@field port integer @The port of the server that was connected to
---@field password string @The password used to connect to the server

---@class MetaBalltzeServerConnectEventArgs: BalltzeServerConnectEventArgs

---@class BalltzeServerConnectEvent: BalltzeEvent
---@field args MetaBalltzeServerConnectEventArgs @The arguments of the event

---@alias BalltzeServerConnectEventCallback fun(event: BalltzeServerConnectEvent)

-- Subscribe a listener to the serverConnect event
---@param callbackFunction BalltzeServerConnectEventCallback @The function to call when the event is triggered
---@param priority? BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.serverConnect.subscribe(callbackFunction, priority) end

-- Remove a listener from the serverConnect event
---@param handle integer @The handle of the event listener
function Balltze.event.serverConnect.removeListener(handle) end

-- Remove all listeners from the serverConnect event
function Balltze.event.serverConnect.removeAllListeners() end

Balltze.event.soundPlayback = {}

---@class BalltzeSoundPlaybackEventArgs
---@field sound MetaEngineTagDataSound @The sound that was played
---@field permutation MetaEngineTagDataSoundPermutation @The permutation of the sound that was played

---@class MetaBalltzeSoundPlaybackEventArgs: BalltzeSoundPlaybackEventArgs

---@class BalltzeSoundPlaybackEvent: BalltzeEvent
---@field args MetaBalltzeSoundPlaybackEventArgs @The arguments of the event

---@alias BalltzeSoundPlaybackEventCallback fun(event: BalltzeSoundPlaybackEvent)

-- Subscribe a listener to the soundPlayback event
---@param callbackFunction BalltzeSoundPlaybackEventCallback @The function to call when the event is triggered
---@param priority? BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.soundPlayback.subscribe(callbackFunction, priority) end

-- Remove a listener from the soundPlayback event
---@param handle integer @The handle of the event listener
function Balltze.event.soundPlayback.removeListener(handle) end

-- Remove all listeners from the soundPlayback event
function Balltze.event.soundPlayback.removeAllListeners() end

Balltze.event.tick = {}

---@class BalltzeTickEventArgs
---@field tickCount integer @The current tick count
---@field deltaTimeMS integer @The number of ticks since the last tick event

---@class MetaBalltzeTickEventArgs: BalltzeTickEventArgs

---@class BalltzeTickEvent : BalltzeEvent
---@field args MetaBalltzeTickEventArgs @The arguments of the event

---@alias BalltzeTickEventCallback fun(event: BalltzeTickEvent)

-- Subscribe a listener to the tick event
---@param callbackFunction BalltzeTickEventCallback @The function to call when the event is triggered
---@param priority? BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.tick.subscribe(callbackFunction, priority) end

-- Remove a listener from the tick event
---@param handle integer @The handle of the event listener
function Balltze.event.tick.removeListener(handle) end

-- Remove all listeners from the tick event
function Balltze.event.tick.removeAllListeners() end

Balltze.event.uiWidgetCreate = {}

---@class BalltzeUIWidgetCreateEventArgs
---@field widget MetaEngineWidget @The widget that is being created
---@field definitionTagHandle EngineTagHandle @The tag handle of the widget definition
---@field isRootWidget boolean @Whether the widget is a root widget

---@class MetaBalltzeUIWidgetCreateEventArgs: BalltzeUIWidgetCreateEventArgs

---@class BalltzeUIWidgetCreateEvent : BalltzeEvent
---@field args MetaBalltzeUIWidgetCreateEventArgs @The arguments of the event

---@alias BalltzeUIWidgetCreateEventCallback fun(event: BalltzeUIWidgetCreateEvent)

-- Subscribe a listener to the uiWidgetCreate event
---@param callbackFunction BalltzeUIWidgetCreateEventCallback @The function to call when the event is triggered
---@param priority? BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.uiWidgetCreate.subscribe(callbackFunction, priority) end

-- Remove a listener from the uiWidgetCreate event
---@param handle integer @The handle of the event listener
function Balltze.event.uiWidgetCreate.removeListener(handle) end

-- Remove all listeners from the uiWidgetCreate event
function Balltze.event.uiWidgetCreate.removeAllListeners() end

Balltze.event.uiWidgetBack = {}

---@class BalltzeUIWidgetBackEventArgs
---@field widget MetaEngineWidget @The widget that is being created

---@class MetaBalltzeUIWidgetBackEventArgs: BalltzeUIWidgetBackEventArgs

---@class BalltzeUIWidgetBackEvent : BalltzeEvent
---@field args MetaBalltzeUIWidgetBackEventArgs @The arguments of the event

---@alias BalltzeUIWidgetBackEventCallback fun(event: BalltzeUIWidgetBackEvent)

-- Subscribe a listener to the uiWidgetBack event
---@param callbackFunction BalltzeUIWidgetBackEventCallback @The function to call when the event is triggered
---@param priority? BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.uiWidgetBack.subscribe(callbackFunction, priority) end

-- Remove a listener from the uiWidgetBack event
---@param handle integer @The handle of the event listener
function Balltze.event.uiWidgetBack.removeListener(handle) end

-- Remove all listeners from the uiWidgetBack event
function Balltze.event.uiWidgetBack.removeAllListeners() end

Balltze.event.uiWidgetFocus = {}

---@class BalltzeUIWidgetFocusEventArgs
---@field widget MetaEngineWidget @The widget that is being created

---@class MetaBalltzeUIWidgetFocusEventArgs: BalltzeUIWidgetFocusEventArgs

---@class BalltzeUIWidgetFocusEvent : BalltzeEvent
---@field args MetaBalltzeUIWidgetFocusEventArgs @The arguments of the event

---@alias BalltzeUIWidgetFocusEventCallback fun(event: BalltzeUIWidgetFocusEvent)

-- Subscribe a listener to the uiWidgetFocus event
---@param callbackFunction BalltzeUIWidgetFocusEventCallback @The function to call when the event is triggered
---@param priority? BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.uiWidgetFocus.subscribe(callbackFunction, priority) end

-- Remove a listener from the uiWidgetFocus event
---@param handle integer @The handle of the event listener
function Balltze.event.uiWidgetFocus.removeListener(handle) end

-- Remove all listeners from the uiWidgetFocus event
function Balltze.event.uiWidgetFocus.removeAllListeners() end

Balltze.event.uiWidgetAccept = {}

---@class BalltzeUIWidgetAcceptEventArgs
---@field widget MetaEngineWidget @The widget that is being created

---@class MetaBalltzeUIWidgetAcceptEventArgs: BalltzeUIWidgetAcceptEventArgs

---@class BalltzeUIWidgetAcceptEvent : BalltzeEvent
---@field args MetaBalltzeUIWidgetAcceptEventArgs @The arguments of the event

---@alias BalltzeUIWidgetAcceptEventCallback fun(event: BalltzeUIWidgetAcceptEvent)

-- Subscribe a listener to the uiWidgetAccept event
---@param callbackFunction BalltzeUIWidgetAcceptEventCallback @The function to call when the event is triggered
---@param priority? BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.uiWidgetAccept.subscribe(callbackFunction, priority) end

-- Remove a listener from the uiWidgetAccept event
---@param handle integer @The handle of the event listener
function Balltze.event.uiWidgetAccept.removeListener(handle) end

-- Remove all listeners from the uiWidgetAccept event
function Balltze.event.uiWidgetAccept.removeAllListeners() end

Balltze.event.uiWidgetSound = {}

---@class BalltzeUIWidgetSoundEventArgs
---@field sound EngineWidgetNavigationSound @The sound that was played

---@class MetaBalltzeUIWidgetSoundEventArgs: BalltzeUIWidgetSoundEventArgs

---@class BalltzeUIWidgetSoundEvent : BalltzeEvent
---@field args MetaBalltzeUIWidgetSoundEventArgs @The arguments of the event

---@alias BalltzeUIWidgetSoundEventCallback fun(event: BalltzeUIWidgetSoundEvent)

-- Subscribe a listener to the uiWidgetSound event
---@param callbackFunction BalltzeUIWidgetSoundEventCallback @The function to call when the event is triggered
---@param priority? BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.uiWidgetSound.subscribe(callbackFunction, priority) end

-- Remove a listener from the uiWidgetSound event
---@param handle integer @The handle of the event listener
function Balltze.event.uiWidgetSound.removeListener(handle) end

-- Remove all listeners from the uiWidgetSound event
function Balltze.event.uiWidgetSound.removeAllListeners() end

Balltze.event.uiWidgetListTab = {}

---@alias BalltzeUIWidgetListTabEventTabType
---| "tab_thru_item_list_items_next_vertical"
---| "tab_thru_item_list_items_next_horizontal"
---| "tab_thru_item_list_items_prev_vertical"
---| "tab_thru_item_list_items_prev_horizontal"
---| "tab_thru_children_next_vertical"
---| "tab_thru_children_next_horizontal"
---| "tab_thru_children_prev"
---| "unknown"

---@class BalltzeUIWidgetListTabEventArgs
---@field widgetList MetaEngineWidget @The widget list that is being navigated
---@field tabType BalltzeUIWidgetListTabEventTabType @The type of the tab

---@class MetaBalltzeUIWidgetListTabEventArgs: BalltzeUIWidgetListTabEventArgs

---@class BalltzeUIWidgetListTabEvent : BalltzeEvent
---@field args MetaBalltzeUIWidgetListTabEventArgs @The arguments of the event

---@alias BalltzeUIWidgetListTabEventCallback fun(event: BalltzeUIWidgetListTabEvent)

-- Subscribe a listener to the uiWidgetListTab event
---@param callbackFunction BalltzeUIWidgetListTabEventCallback @The function to call when the event is triggered
---@param priority? BalltzeEventListenerPriority @The priority of the callback function
---@return BalltzeEventListener @The handle of the event listener
function Balltze.event.uiWidgetListTab.subscribe(callbackFunction, priority) end

-- Remove a listener from the uiWidgetListTab event
---@param handle integer @The handle of the event listener
function Balltze.event.uiWidgetListTab.removeListener(handle) end

-- Remove all listeners from the uiWidgetListTab event
function Balltze.event.uiWidgetListTab.removeAllListeners() end

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
---@param tagHandleOrPath EngineTagHandle|integer|string @The handle or path of the tag to reload
---@param tagClass? EngineTagClass @The class of the tag to reload
function Balltze.features.reloadTagData(tagHandleOrPath, tagClass) end

-- Replace all tag references to a tag by references to another tag
---@param tagHandle EngineTagHandle|integer @The handle of the tag to replace
---@param newTagHandle EngineTagHandle|integer @The handle of the tag to replace with
function Balltze.features.replaceTagReferences(tagHandle, newTagHandle) end

-- Copy a tag
---@param tagHandle EngineTagHandle|integer @The handle of the tag to copy
---@param copyName string @The name of the copy
---@return EngineTagHandle @The handle of the copy
function Balltze.features.cloneTag(tagHandle, copyName) end

-- Get a copy of a tag
---@param tagHandle EngineTagHandle|integer @The handle of the tag to get a copy of
---@param copyName string @The name of the copy
---@return EngineTagHandle|nil @The handle of the copy; nil if the copy does not exist
function Balltze.features.getTagCopy(tagHandle, copyName) end

-- Get a an imported tag from another map
---@param mapPath string @The path of the map to get the tag from
---@param tagPath string @The path of the tag to get
---@param tagClass EngineTagClass @The class of the tag to get
---@return EngineTagHandle|nil @The handle of the tag; nil if the tag does not exist
function Balltze.features.getImportedTag(mapPath, tagPath, tagClass) end

-- Sets the aspect ratio of the user interface
function Balltze.features.setUIAspectRatio(x, y) end

-- Resets the aspect ratio of the user interface
function Balltze.features.resetUIAspectRatio() end

-------------------------------------------------------
-- Balltze.logger
-------------------------------------------------------

Balltze.logger = {}

---@class Logger
local logger = {}

-- Log a debug message
---@param format string @The format of the message
---@vararg any @The arguments of the message
function logger:debug(format, ...) end

-- Log an info message
---@param format string @The format of the message
---@vararg any @The arguments of the message
function logger:info(format, ...) end

-- Log a warning message
---@param format string @The format of the message
---@vararg any @The arguments of the message
function logger:warning(format, ...) end

-- Log an error message
---@param format string @The format of the message
---@vararg any @The arguments of the message
function logger:error(format, ...) end

-- Log a fatal message
---@param format string @The format of the message
---@vararg any @The arguments of the message
function logger:fatal(format, ...) end

-- Set a file to log to
---@param filePath string @The path of the file to log to
function logger:setFile(filePath) end

-- Set whether to log to the in-game console
---@param setting? boolean @Whether to log to the in-game console
---@return boolean|nil @The current setting if no argument is provided
function logger:muteIngame(setting) end

-- Set whether to log debug messages
---@param setting? boolean @Whether to log debug messages
---@return boolean|nil @The current setting if no argument is provided
function logger:muteDebug(setting) end

-- Create a new logger
---@param name string @The name of the logger
---@return Logger @The logger
function Balltze.logger.createLogger(name) end

-------------------------------------------------------
-- Balltze.math
-------------------------------------------------------

Balltze.math = {}

---@class BalltzeMathBezierCurve
---@field getPoint fun(t: BalltzeMathBezierCurve, initialValue: number, finalValue: number, t: number, reverse?: boolean): number @Get a point on the curve

-- Create a bezier curve
---@param x1 number @The x coordinate of the first point
---@param y1 number @The y coordinate of the first point
---@param x2 number @The x coordinate of the second point
---@param y2 number @The y coordinate of the second point
---@return BalltzeMathBezierCurve @The bezier curve
---@overload fun(curvePresetName: string): BalltzeMathBezierCurve
function Balltze.math.createBezierCurve(x1, y1, x2, y2) end

-- Get a point on a bezier curve
---@param bezier BalltzeMathBezierCurve @The bezier curve
---@param initialValue number @The starting point
---@param finalValue number @The destination point
---@param t number @The t value
---@param reverse? boolean @Whether to reverse the curve
---@return number @The interpolated value 
function Balltze.math.getBezierCurvePoint(bezier, initialValue, finalValue, t, reverse) end

-------------------------------------------------------
-- Balltze.misc
-------------------------------------------------------

Balltze.misc = {}

---@class BalltzeMiscTimestamp
---@field getElapsedMilliseconds fun(): integer @Get the number of milliseconds since the timestamp was set
---@field getElapsedSeconds fun(): integer @Get the number of seconds since the timestamp was set
---@field reset fun() @Reset the timestamp

-- Create a timestamp
---@return BalltzeMiscTimestamp @The timestamp
function Balltze.misc.setTimestamp() end

-- Get the elapsed milliseconds since the timestamp was set
---@param timestamp BalltzeMiscTimestamp @The timestamp
---@return integer @The number of milliseconds since the timestamp was set
function Balltze.misc.getElapsedMilliseconds(timestamp) end

-- Get the elapsed seconds since the timestamp was set
---@param timestamp BalltzeMiscTimestamp @The timestamp
---@return integer @The number of seconds since the timestamp was set
function Balltze.misc.getElapsedSeconds(timestamp) end

-- Reset the timestamp
---@param timestamp BalltzeMiscTimestamp @The timestamp
function Balltze.misc.resetTimestamp(timestamp) end

---Set content to the clipboard
---@param content string Content to copy
function Balltze.misc.setClipboard(content) end

---Get content from the clipboard
---@return string Content from the clipboard
function Balltze.misc.getClipboard() end

-------------------------------------------------------
-- Balltze.output
-------------------------------------------------------

Balltze.output = {}

-- Get the font tag for a specific generic font
---@param fontName EngineGenericFont @The name of the generic font 
function Balltze.output.getGenericFont(fontName) end

-- Get the number of pixels a string takes up given a font
---@param text string @The text to measure
---@param font string|integer|EngineTagHandle @The font to use
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
function Balltze.chimera.create_font_override(name, path, size, weight, shadowOffsetX, shadowOffsetY, offsetX, offsetY) end
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
