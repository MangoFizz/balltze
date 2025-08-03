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


-------------------------------------------------------
-- Engine.tag
-------------------------------------------------------

Engine.tag = {}

-- Look up a tag by its path
---@param path string @The path of the tag to look up
---@param group TagGroup @The group of the tag to look up
---@return TagHandle @The tag handle if found, or nil if not found; error if the map is not loaded
function Engine.tag.lookupTag(path, group) end

-- Get the data of a tag
---@param handle TagHandle @The handle of the tag to get the data from
---@param group TagGroup @The group of the tag to get the data from; error if the map is not loaded
---@overload fun(handle: TagHandle, group: "weapon_hud_interface"): WeaponHUDInterface
---@overload fun(handle: TagHandle, group: "wind"): Wind
---@overload fun(handle: TagHandle, group: "weapon"): Weapon
---@overload fun(handle: TagHandle, group: "vehicle"): Vehicle
---@overload fun(handle: TagHandle, group: "virtual_keyboard"): VirtualKeyboard
---@overload fun(handle: TagHandle, group: "unicode_string_list"): UnicodeStringList
---@overload fun(handle: TagHandle, group: "unit"): Unit
---@overload fun(handle: TagHandle, group: "unit_hud_interface"): UnitHUDInterface
---@overload fun(handle: TagHandle, group: "dialogue"): Dialogue
---@overload fun(handle: TagHandle, group: "camera_track"): CameraTrack
---@overload fun(handle: TagHandle, group: "tag_collection"): TagCollection
---@overload fun(handle: TagHandle, group: "shader_transparent_water"): ShaderTransparentWater
---@overload fun(handle: TagHandle, group: "string_list"): StringList
---@overload fun(handle: TagHandle, group: "sound_scenery"): SoundScenery
---@overload fun(handle: TagHandle, group: "shader_transparent_plasma"): ShaderTransparentPlasma
---@overload fun(handle: TagHandle, group: "ui_widget_collection"): UIWidgetCollection
---@overload fun(handle: TagHandle, group: "shader_transparent_generic"): ShaderTransparentGeneric
---@overload fun(handle: TagHandle, group: "shader_model"): ShaderModel
---@overload fun(handle: TagHandle, group: "sound_environment"): SoundEnvironment
---@overload fun(handle: TagHandle, group: "sound"): Sound
---@overload fun(handle: TagHandle, group: "shader_transparent_meter"): ShaderTransparentMeter
---@overload fun(handle: TagHandle, group: "sky"): Sky
---@overload fun(handle: TagHandle, group: "shader"): Shader
---@overload fun(handle: TagHandle, group: "shader_transparent_glass"): ShaderTransparentGlass
---@overload fun(handle: TagHandle, group: "shader_environment"): ShaderEnvironment
---@overload fun(handle: TagHandle, group: "scenario"): Scenario
---@overload fun(handle: TagHandle, group: "shader_transparent_chicago"): ShaderTransparentChicago
---@overload fun(handle: TagHandle, group: "shader_transparent_chicago_extended"): ShaderTransparentChicagoExtended
---@overload fun(handle: TagHandle, group: "scenery"): Scenery
---@overload fun(handle: TagHandle, group: "scenario_structure_bsp"): ScenarioStructureBSP
---@overload fun(handle: TagHandle, group: "weather_particle_system"): WeatherParticleSystem
---@overload fun(handle: TagHandle, group: "projectile"): Projectile
---@overload fun(handle: TagHandle, group: "point_physics"): PointPhysics
---@overload fun(handle: TagHandle, group: "placeholder"): Placeholder
---@overload fun(handle: TagHandle, group: "physics"): Physics
---@overload fun(handle: TagHandle, group: "particle_system"): ParticleSystem 
---@overload fun(handle: TagHandle, group: "particle"):  Particle
---@overload fun(handle: TagHandle, group: "object"): Object
---@overload fun(handle: TagHandle, group: "preferences_network_game"): PreferencesNetworkGame
---@overload fun(handle: TagHandle, group: "multiplayer_scenario_description"): MultiplayerScenarioDescription
---@overload fun(handle: TagHandle, group: "model"): Model
---@overload fun(handle: TagHandle, group: "gbxmodel"): GBXModel
---@overload fun(handle: TagHandle, group: "light_volume"): LightVolume
---@overload fun(handle: TagHandle, group: "meter"): Meter
---@overload fun(handle: TagHandle, group: "globals"): Globals
---@overload fun(handle: TagHandle, group: "device_machine"): DeviceMachine
---@overload fun(handle: TagHandle, group: "sound_looping"): SoundLooping
---@overload fun(handle: TagHandle, group: "light"): Light
---@overload fun(handle: TagHandle, group: "device_light_fixture"): DeviceLightFixture
---@overload fun(handle: TagHandle, group: "lightning"): Lightning
---@overload fun(handle: TagHandle, group: "lens_flare"): LensFlare
---@overload fun(handle: TagHandle, group: "damage_effect"): DamageEffect
---@overload fun(handle: TagHandle, group: "item_collection"): ItemCollection
---@overload fun(handle: TagHandle, group: "item"): Item
---@overload fun(handle: TagHandle, group: "hud_globals"): HUDGlobals
---@overload fun(handle: TagHandle, group: "hud_number"): HUDNumber
---@overload fun(handle: TagHandle, group: "hud_message_text"): HUDMessageText
---@overload fun(handle: TagHandle, group: "grenade_hud_interface"): GrenadeHUDInterface
---@overload fun(handle: TagHandle, group: "glow"): Glow
---@overload fun(handle: TagHandle, group: "garbage"): Garbage
---@overload fun(handle: TagHandle, group: "material_effects"): MaterialEffects
---@overload fun(handle: TagHandle, group: "font"): Font
---@overload fun(handle: TagHandle, group: "fog"): Fog
---@overload fun(handle: TagHandle, group: "flag"): Flag
---@overload fun(handle: TagHandle, group: "equipment"): Equipment
---@overload fun(handle: TagHandle, group: "effect"): Effect
---@overload fun(handle: TagHandle, group: "detail_object_collection"): DetailObjectCollection
---@overload fun(handle: TagHandle, group: "device"): Device
---@overload fun(handle: TagHandle, group: "input_device_defaults"): InputDeviceDefaults
---@overload fun(handle: TagHandle, group: "ui_widget_definition"): UIWidgetDefinition
---@overload fun(handle: TagHandle, group: "decal"): Decal
---@overload fun(handle: TagHandle, group: "device_control"): DeviceControl
---@overload fun(handle: TagHandle, group: "contrail"): Contrail
---@overload fun(handle: TagHandle, group: "color_table"): ColorTable
---@overload fun(handle: TagHandle, group: "model_collision_geometry"):  ModelCollisionGeometry
---@overload fun(handle: TagHandle, group: "continuous_damage_effect"): ContinuousDamageEffect
---@overload fun(handle: TagHandle, group: "bitmap"): Bitmap
---@overload fun(handle: TagHandle, group: "biped"): Biped
---@overload fun(handle: TagHandle, group: "model_animations"): ModelAnimations
---@overload fun(handle: TagHandle, group: "antenna"): Antenna
---@overload fun(handle: TagHandle, group: "actor_variant"): ActorVariant
---@overload fun(handle: TagHandle, group: "actor"): Actor
function Engine.tag.getTagData(handle, group) end

-- Get the tag entry of a tag handle
---@param handle TagHandle @The handle of the tag to get the entry from
---@return TagEntry @The tag entry if found, or nil if not found; error if the map is not loaded
function Engine.tag.getTagEntry(handle, group) end

---Return a tag entry list given a keyword and a tag group
---Leave the keyword empty to get all tags, if tag group is nil, it will return all tags
---@param tagGroup? string @The tag group to filter the tags by
---@param pathKeyword string @The keyword to search for in the tag paths
---@return TagEntry[] @The list of tag entries that match the keyword and tag group; error if the map is not loaded
function Engine.tag.filterTags(tagGroup, pathKeyword) end

-------------------------------------------------------
-- Engine.object
-------------------------------------------------------

Engine.object = {}

-- Get an object of the current game
---@param handle ObjectHandle|integer @The handle of the object to get
---@return DynamicObjectBase|nil
---@overload fun(handle: ObjectHandle|integer, type: "biped"): BipedObject|nil
---@overload fun(handle: ObjectHandle|integer, type: "vehicle"): VehicleObject|nil
---@overload fun(handle: ObjectHandle|integer, type: "garbage"): GarbageObject|nil
---@overload fun(handle: ObjectHandle|integer, type: "weapon"): WeaponObject|nil
---@overload fun(handle: ObjectHandle|integer, type: "equipment"): EquipmentObject|nil
---@overload fun(handle: ObjectHandle|integer, type: "projectile"): ProjectileObject|nil
---@overload fun(handle: ObjectHandle|integer, type: "device_machine"): DeviceMachineObject|nil
---@overload fun(handle: ObjectHandle|integer, type: "device_control"): DeviceControlObject|nil
---@overload fun(handle: ObjectHandle|integer, type: "device_light_fixture"): DeviceLightFixtureObject|nil
function Engine.object.getObject(handle, type) end

-- Spawn an object
---@param tagHandle TagHandle|integer @The tag handle of the object
---@param parentObjectHandle? ObjectHandle|integer @The handle of the parent object
---@param position VectorXYZ @The position of the object
---@return ObjectHandle @The handle of the object
function Engine.object.createObject(tagHandle, parentObjectHandle, position) end

-- Delete the object
---@param objectHandle ObjectHandle|integer @The handle of the object
function Engine.object.deleteObject(objectHandle) end

-- Attach an object to another object
---@param objectHandle ObjectHandle|integer @The handle of the object to attach
---@param objectMarker string|nil @The marker of the object to attach
---@param attachmentObjectHandle ObjectHandle|integer @The handle of the object to attach to
---@param attachmentMarker string|nil @The marker of the attachment object
function Engine.object.objectAttachToMarker(objectHandle, objectMarker, attachmentObjectHandle, attachmentMarker) end


-------------------------------------------------------
-- Engine.player
-------------------------------------------------------

Engine.player = {}

-- Get a player
---@param playerIndexOrHandle? PlayerHandle|integer @The index or the handle of the player; If nil, the local player is returned
---@return Player @The player
function Engine.player.getPlayer(playerIndexOrHandle) end


-------------------------------------------------------
-- Engine.uiWidget
-------------------------------------------------------

Engine.uiWidget = {}

-- Launch a UI widget 
---@param uiWidgetDefinition TagHandle|integer @The tag handle of the UI widget definition
function Engine.uiWidget.launchWidget(uiWidgetDefinition) end

-- Get the currently active UI widget
---@return Widget|nil @The active widget, or nil if no widget is active
function Engine.uiWidget.getActiveWidget() end

-- Close the currently active UI widget
function Engine.uiWidget.closeWidget() end

-- Replace the currently active UI widget with a new one
---@param widget Widget @The new widget to replace the current one
---@param uiWidgetDefinition TagHandle|integer @The tag handle of the definition for the new widget
---@return Widget @The new widget that was created
function Engine.uiWidget.replaceWidget(widget, uiWidgetDefinition) end

-- Reload a UI widget
---@param widget Widget @The widget to reload
---@return Widget @The reloaded widget
function Engine.uiWidget.reloadWidget(widget) end

-- Focus a UI widget
---@param widget Widget @The widget to focus
function Engine.uiWidget.focusWidget(widget) end

