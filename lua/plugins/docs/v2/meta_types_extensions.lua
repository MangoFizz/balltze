-- SPDX-License-Identifier: GPL-3.0-only

---@meta _

---@class TableResourceHandle
local TableResourceHandle = {}

-- Get whether the table resource handle is null
---@return boolean
function TableResourceHandle:isNull() end

---@class ObjectHandle
local ObjectHandle = {}

-- Get whether the object handle is null
---@return boolean
function ObjectHandle:isNull() end

---@class PlayerHandle
local PlayerHandle = {}

-- Get whether the player handle is null
---@return boolean
function PlayerHandle:isNull() end

---@class TagHandle
local TagHandle = {}

-- Get whether the tag handle is null
---@return boolean
function TagHandle:isNull() end

---@alias TagData
---| Actor
---| ActorVariant
---| Antenna
---| ModelAnimations
---| Biped
---| Bitmap
---| ContinuousDamageEffect
---| ModelCollisionGeometry
---| ColorTable
---| Contrail
---| DeviceControl
---| Decal
---| UIWidgetDefinition
---| InputDeviceDefaults
---| Device
---| DetailObjectCollection
---| Effect
---| Equipment
---| Flag
---| Fog
---| Font
---| Lightning
---| MaterialEffects
---| Garbage
---| Glow
---| GrenadeHUDInterface
---| HUDMessageText
---| HUDNumber
---| HUDGlobals
---| Item
---| ItemCollection
---| DamageEffect
---| LensFlare
---| DeviceLightFixture
---| Light
---| SoundLooping
---| DeviceMachine
---| Globals
---| Meter
---| LightVolume
---| GBXModel
---| Model
---| MultiplayerScenarioDescription
---| PreferencesNetworkGame
---| Object
---| Particle
---| ParticleSystem
---| Physics
---| Placeholder
---| PointPhysics
---| Projectile
---| WeatherParticleSystem
---| ScenarioStructureBSP
---| Scenery
---| ShaderTransparentChicagoExtended
---| ShaderTransparentChicago
---| Scenario
---| ShaderEnvironment
---| ShaderTransparentGlass
---| Shader
---| Sky
---| ShaderTransparentMeter
---| Sound
---| SoundEnvironment
---| ShaderModel
---| ShaderTransparentGeneric
---| UIWidgetCollection
---| UIWidgetCollection
---| ShaderTransparentPlasma
---| SoundScenery
---| StringList
---| ShaderTransparentWater
---| TagCollection
---| CameraTrack
---| UnitHUDInterface
---| Unit
---| UnicodeStringList
---| VirtualKeyboard
---| Vehicle
---| Weapon
---| Wind
---| WeaponHUDInterface

---@class TagEntry
local TagEntry = {}

-- Returns the data of the tag.
---@return TagData
function TagEntry:getData() end
