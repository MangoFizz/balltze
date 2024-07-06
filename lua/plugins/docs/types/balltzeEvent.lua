-- SPDX-License-Identifier: GPL-3.0-only

---@meta _

-------------------------------------------------------
-- Balltze common event types
-------------------------------------------------------

---@alias BalltzeEventTime
---| "before" #Before the event is triggered
---| "after" #After the event is triggered

---@alias BalltzeEventListenerPriority
---| "highest" 
---| "above_default"
---| "default" 
---| "lowest" 

---@class BalltzeEventListener
---@field remove fun(self: BalltzeEventListener) @Remove the listener from the event

---@class BalltzeEvent
---@field cancelled boolean @Whether the event is already cancelled
---@field time BalltzeEventTime @The time the event was triggered 
---@field cancel fun(self: BalltzeEvent) @Cancel the event

-------------------------------------------------------
-- Balltze camera event types
-------------------------------------------------------

---@class BalltzeCameraEventArgs
---@field position EngineCameraData @The position of the camera
---@field type EngineCameraType @The type of the camera

---@class MetaBalltzeCameraEventArgs: BalltzeCameraEventArgs

---@class BalltzeCameraEvent: BalltzeEvent
---@field args MetaBalltzeCameraEventArgs @The arguments of the event

---@alias BalltzeCameraEventCallback fun(event: BalltzeCameraEvent)

-------------------------------------------------------
-- Balltze frame event types
-------------------------------------------------------

---@class BalltzeFrameEvent: BalltzeEvent

---@alias BalltzeFrameEventCallback fun(event: BalltzeFrameEvent)

-------------------------------------------------------
-- Balltze game input event types
-------------------------------------------------------

---@class BalltzeGameInputEventArgs
---@field device EngineInputDevice @The device that triggered the event
---@field mapped boolean @Whether the input is mapped to a game action
---@field button string @The button that was pressed

---@class MetaBalltzeGameInputEventArgs: BalltzeGameInputEventArgs

---@class BalltzeGameInputEvent: BalltzeEvent
---@field args MetaBalltzeGameInputEventArgs @The arguments of the event

---@alias BalltzeGameInputEventCallback fun(event: BalltzeGameInputEvent)

-------------------------------------------------------
-- Balltze keyboard input event types
-------------------------------------------------------

---@class BalltzeKeyboardInputEventArgs
---@field key EngineInputBufferedKey

---@class MetaBalltzeKeyboardInputEventArgs: BalltzeKeyboardInputEventArgs

---@class BalltzeKeyboardInputEvent: BalltzeEvent
---@field args BalltzeKeyboardInputEventArgs @The arguments of the event

---@alias BalltzeKeyboardInputEventCallback fun(event: BalltzeKeyboardInputEvent)

-------------------------------------------------------
-- Balltze HUD-hold-for-action message event types
-------------------------------------------------------

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

-------------------------------------------------------
-- Balltze Map file load event types
-------------------------------------------------------

---@class BalltzeMapFileLoadEventArgs
---@field mapPath string @The path of the map that was loaded
---@field mapName string @The name of the map that was loaded

---@class MetaBalltzeMapFileLoadEventArgs: BalltzeMapFileLoadEventArgs

---@class BalltzeMapFileLoadEvent: BalltzeEvent
---@field args MetaBalltzeMapFileLoadEventArgs @The arguments of the event

---@alias BalltzeMapFileLoadEventCallback fun(event: BalltzeMapFileLoadEvent)

-------------------------------------------------------
-- Balltze Map load event types
-------------------------------------------------------

---@class BalltzeMapLoadEventArgs
---@field mapName string Name of the map that was loaded

---@class MetaBalltzeMapLoadEventArgs: BalltzeMapLoadEventArgs

---@class BalltzeMapLoadEvent: BalltzeEvent
---@field args MetaBalltzeMapLoadEventArgs @The arguments of the event

---@alias BalltzeMapLoadEventCallback fun(event: BalltzeMapLoadEvent)

-------------------------------------------------------
-- Balltze Network game chat event types
-------------------------------------------------------

---@class BalltzeNetworkGameChatMessageEventArgs
---@field message lightuserdata @The message that was sent
---@field playerId integer @The player that sent the message
---@field type EngineNetworkGameMessageHudChatType @The type of the message

---@class MetaBalltzeNetworkGameChatMessageEventArgs: BalltzeNetworkGameChatMessageEventArgs

---@class BalltzeNetworkGameChatMessageEvent: BalltzeEvent
---@field args MetaBalltzeNetworkGameChatMessageEventArgs @The arguments of the event

---@alias BalltzeNetworkGameChatMessageEventCallback fun(event: BalltzeNetworkGameChatMessageEvent)

-------------------------------------------------------
-- Balltze Object damage event types
-------------------------------------------------------

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

-------------------------------------------------------
-- Balltze RCON message event types
-------------------------------------------------------

---@class BalltzeRconMessageEventArgs
---@field message string @The message that was sent

---@class MetaBalltzeRconMessageEventArgs: BalltzeRconMessageEventArgs

---@class BalltzeRconMessageEvent: BalltzeEvent
---@field args MetaBalltzeRconMessageEventArgs @The arguments of the event

---@alias BalltzeRconMessageEventCallback fun(event: BalltzeRconMessageEvent)

-------------------------------------------------------
-- Balltze User interface render event types
-------------------------------------------------------

---@class BalltzeUiRenderEventArgs
---@field playerIndex integer @The index of the player of the UI that was rendered

---@class BalltzeUiRenderEvent: BalltzeEvent
---@field args BalltzeUiRenderEventArgs @The arguments of the event

---@alias BalltzeUiRenderEventCallback fun(event: BalltzeUiRenderEvent)

-------------------------------------------------------
-- Balltze HUD render event types
-------------------------------------------------------

---@class BalltzeHudRenderEvent: BalltzeEvent

---@alias BalltzeHudRenderEventCallback fun(event: BalltzeHudRenderEvent)

-------------------------------------------------------
-- Balltze Post carnage report render event types
-------------------------------------------------------

---@class BalltzePostCarnageReportRenderEvent: BalltzeEvent

---@alias BalltzePostCarnageReportRenderEventCallback fun(event: BalltzePostCarnageReportRenderEvent)

-------------------------------------------------------
-- Balltze HUD element bitmap render event types
-------------------------------------------------------

---@class BalltzeHudElementBitmapRenderEventArgs
---@field vertices EngineUIWidgetRenderVertices @The vertices of the bitmap
---@field bitmapData MetaEngineTagDataBitmapData @The bitmap data of the bitmap

---@class MetaBalltzeHudElementBitmapRenderEventArgs: BalltzeHudElementBitmapRenderEventArgs

---@class BalltzeHudElementBitmapRenderEvent: BalltzeEvent
---@field args MetaBalltzeHudElementBitmapRenderEventArgs @The arguments of the event

---@alias BalltzeHudElementBitmapRenderEventCallback fun(event: BalltzeHudElementBitmapRenderEvent)

-------------------------------------------------------
-- Balltze UI widget background render event types
-------------------------------------------------------

---@class BalltzeUIWidgetBackgroundRenderEventArgs
---@field vertices EngineUIWidgetRenderVertices @The vertices of the background
---@field widget MetaEngineWidget @The widget of the background

---@class MetaBalltzeUIWidgetBackgroundRenderEventArgs: BalltzeUIWidgetBackgroundRenderEventArgs

---@class BalltzeUIWidgetBackgroundRenderEvent: BalltzeEvent
---@field args MetaBalltzeUIWidgetBackgroundRenderEventArgs @The arguments of the event

---@alias BalltzeUIWidgetBackgroundRenderEventCallback fun(event: BalltzeUIWidgetBackgroundRenderEvent)

-------------------------------------------------------
-- Balltze HUD navpoints render event types
-------------------------------------------------------

---@class BalltzeNavpointsRenderEvent: BalltzeEvent

---@alias BalltzeNavpointsRenderEventCallback fun(event: BalltzeNavpointsRenderEvent)

-------------------------------------------------------
-- Balltze Server connect event types
-------------------------------------------------------

---@class BalltzeServerConnectEventArgs
---@field address string @The address of the server that was connected to
---@field port integer @The port of the server that was connected to
---@field password string @The password used to connect to the server

---@class MetaBalltzeServerConnectEventArgs: BalltzeServerConnectEventArgs

---@class BalltzeServerConnectEvent: BalltzeEvent
---@field args MetaBalltzeServerConnectEventArgs @The arguments of the event

---@alias BalltzeServerConnectEventCallback fun(event: BalltzeServerConnectEvent)

-------------------------------------------------------
-- Balltze Sound playback event types
-------------------------------------------------------

---@class BalltzeSoundPlaybackEventArgs
---@field sound MetaEngineTagDataSound @The sound that was played
---@field permutation MetaEngineTagDataSoundPermutation @The permutation of the sound that was played

---@class MetaBalltzeSoundPlaybackEventArgs: BalltzeSoundPlaybackEventArgs

---@class BalltzeSoundPlaybackEvent: BalltzeEvent
---@field args MetaBalltzeSoundPlaybackEventArgs @The arguments of the event

---@alias BalltzeSoundPlaybackEventCallback fun(event: BalltzeSoundPlaybackEvent)

-------------------------------------------------------
-- Balltze Tick event types
-------------------------------------------------------

---@class BalltzeTickEventArgs
---@field tickCount integer @The current tick count
---@field deltaTimeMS integer @The number of ticks since the last tick event

---@class MetaBalltzeTickEventArgs: BalltzeTickEventArgs

---@class BalltzeTickEvent : BalltzeEvent
---@field args MetaBalltzeTickEventArgs @The arguments of the event

---@alias BalltzeTickEventCallback fun(event: BalltzeTickEvent)

-------------------------------------------------------
-- Balltze UI widget create event types
-------------------------------------------------------

---@class BalltzeUIWidgetCreateEventArgs
---@field widget MetaEngineWidget @The widget that is being created
---@field definitionTagHandle EngineTagHandle @The tag handle of the widget definition
---@field isRootWidget boolean @Whether the widget is a root widget

---@class MetaBalltzeUIWidgetCreateEventArgs: BalltzeUIWidgetCreateEventArgs

---@class BalltzeUIWidgetCreateEvent : BalltzeEvent
---@field args MetaBalltzeUIWidgetCreateEventArgs @The arguments of the event

---@alias BalltzeUIWidgetCreateEventCallback fun(event: BalltzeUIWidgetCreateEvent)

-------------------------------------------------------
-- Balltze UI widget back event types
-------------------------------------------------------

---@class BalltzeUIWidgetBackEventArgs
---@field widget MetaEngineWidget @The widget that is being created

---@class MetaBalltzeUIWidgetBackEventArgs: BalltzeUIWidgetBackEventArgs

---@class BalltzeUIWidgetBackEvent : BalltzeEvent
---@field args MetaBalltzeUIWidgetBackEventArgs @The arguments of the event

---@alias BalltzeUIWidgetBackEventCallback fun(event: BalltzeUIWidgetBackEvent)

-------------------------------------------------------
-- Balltze UI widget focus event types
-------------------------------------------------------

---@class BalltzeUIWidgetFocusEventArgs
---@field widget MetaEngineWidget @The widget that is being created

---@class MetaBalltzeUIWidgetFocusEventArgs: BalltzeUIWidgetFocusEventArgs

---@class BalltzeUIWidgetFocusEvent : BalltzeEvent
---@field args MetaBalltzeUIWidgetFocusEventArgs @The arguments of the event

---@alias BalltzeUIWidgetFocusEventCallback fun(event: BalltzeUIWidgetFocusEvent)

-------------------------------------------------------
-- Balltze UI widget accept event types
-------------------------------------------------------

---@class BalltzeUIWidgetAcceptEventArgs
---@field widget MetaEngineWidget @The widget that is being created

---@class MetaBalltzeUIWidgetAcceptEventArgs: BalltzeUIWidgetAcceptEventArgs

---@class BalltzeUIWidgetAcceptEvent : BalltzeEvent
---@field args MetaBalltzeUIWidgetAcceptEventArgs @The arguments of the event

---@alias BalltzeUIWidgetAcceptEventCallback fun(event: BalltzeUIWidgetAcceptEvent)

-------------------------------------------------------
-- Balltze UI widget sound event types
-------------------------------------------------------

---@class BalltzeUIWidgetSoundEventArgs
---@field sound EngineWidgetNavigationSound @The sound that was played

---@class MetaBalltzeUIWidgetSoundEventArgs: BalltzeUIWidgetSoundEventArgs

---@class BalltzeUIWidgetSoundEvent : BalltzeEvent
---@field args MetaBalltzeUIWidgetSoundEventArgs @The arguments of the event

---@alias BalltzeUIWidgetSoundEventCallback fun(event: BalltzeUIWidgetSoundEvent)

-------------------------------------------------------
-- Balltze UI widget list tab event types
-------------------------------------------------------

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
