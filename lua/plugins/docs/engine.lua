-- SPDX-License-Identifier: GPL-3.0-only
-- This file is used to document the Lua plugins engine API. It should not be included.

---@meta _

---@diagnostic disable: missing-return 
---@diagnostic disable: unused-local 

Engine = {}

-------------------------------------------------------
-- Data types
-------------------------------------------------------

---@class EngineColorARGB
---@field alpha number
---@field red number
---@field green number
---@field blue number

---@class MetaEngineColorARGB: EngineColorARGB

---@class EngineColorARGBInt
---@field alpha integer
---@field red integer
---@field green integer
---@field blue integer

---@class MetaEngineColorARGBInt: EngineColorARGBInt
---@class MetaEngineTagDataColorARGBInt: EngineColorARGBInt

---@class EngineColorRGB
---@field red number
---@field green number
---@field blue number

---@class MetaEngineColorRGB: EngineColorRGB

---@class EnginePoint2D
---@field x number
---@field y number

---@class MetaEnginePoint2D: EnginePoint2D

---@class EnginePoint2DInt
---@field x integer
---@field y integer

---@class MetaEnginePoint2DInt: EnginePoint2DInt

---@class EnginePoint3D
---@field x number
---@field y number
---@field z number

---@class MetaEnginePoint3D: EnginePoint3D

---@class EngineVector2D
---@field i number
---@field j number

---@class MetaEngineVector2D: EngineVector2D

---@class EngineVector3D
---@field i number
---@field j number
---@field k number

---@class MetaEngineVector3D: EngineVector3D 

---@class EngineResourceHandle
---@field handle integer
---@field index integer
---@field id integer

---@class EngineTagHandle: EngineResourceHandle

---@class EngineObjectHandle: EngineResourceHandle

---@class EnginePlayerHandle: EngineResourceHandle

---@class EngineEuler3DPYR
---@field pitch number
---@field yaw number
---@field roll number

---@class MetaEngineEuler3DPYR: EngineEuler3DPYR

---@class EngineEuler2D
---@field yaw number
---@field pitch number

---@class MetaEngineEuler2D: EngineEuler2D

---@class EngineEuler3D
---@field yaw number
---@field pitch number
---@field roll number

---@class MetaEngineEuler3D: EngineEuler3D

---@class EngineRectangle2D
---@field top number
---@field left number
---@field bottom number
---@field right number

---@class MetaEngineRectangle2D: EngineRectangle2D

---@class EngineRectangle2DF
---@field top number
---@field left number
---@field bottom number
---@field right number

---@class MetaEngineRectangle2DF: EngineRectangle2DF

---@class EnginePlane2D
---@field i number
---@field j number

---@class MetaEnginePlane2D: EnginePlane2D

---@class EnginePlane3D: EngineVector3D
---@field w number

---@class MetaEnginePlane3D: EnginePlane3D

---@class MetaEngineTagString : string

---@class EngineTagDependency
---@field handle integer
---@field class string
---@field path string

---@class MetaEngineTagDependency: EngineTagDependency

---@class MetaEngineIndex: integer

---@class EngineTagDataOffset
---@field size integer
---@field external boolean
---@field fileOffset integer | nil
---@field pointer integer | nil

---@class MetaEngineTagDataOffset: EngineTagDataOffset

---@class EngineAngle: number

---@class MetaEngineAngle: EngineAngle

---@class EngineFraction

---@class MetaEngineFraction: EngineFraction

---@class EngineQuaternion
---@field i number
---@field j number
---@field k number
---@field w number

---@class MetaEngineQuaternion: EngineQuaternion

---@class EngineMatrix: table<table<number>>

---@class MetaEngineMatrix: EngineMatrix

---@class EngineScenarioScriptNodeValue
---@field float number
---@field integer integer
---@field tag EngineTagHandle

---@class MetaEngineScenarioScriptNodeValue: EngineScenarioScriptNodeValue

---@alias EngineGenericFont
---| "console"
---| "system"
---| "small"
---| "smaller"
---| "large"
---| "ticker"

---@alias EngineInputDevice
---| "keyboard"
---| "mouse"
---| "gamepad"
---| "unknown"

---@class EngineWidgetRenderVertex
---@field x number
---@field y number
---@field z number
---@field rhw number
---@field u number
---@field v number

---@class EngineUIWidgetRenderVertices
---@field topLeft EngineWidgetRenderVertex
---@field topRight EngineWidgetRenderVertex
---@field bottomLeft EngineWidgetRenderVertex
---@field bottomRight EngineWidgetRenderVertex

-------------------------------------------------------
-- Engine.core
-------------------------------------------------------

Engine.core = {}

-- Print a message of a color in the console; It uses the fmt library to format the message
---@param color EngineColorARGB @The color of the message
---@param format string @The format string in case the first argument is a color
---@param ... string @The arguments to the format string
---@overload fun(format: string, ...: string)
function Engine.core.consolePrint(color, format, ...) end

-- Get the number of ticks since the engine started
---@return integer
function Engine.core.getTickCount() end

---@alias EngineEdition
---| 'retail'
---| 'demo'
---| 'custom'

-- Get the game engine used
---@return EngineEdition
function Engine.core.getEngineEdition() end

-------------------------------------------------------
-- Engine.gameState
-------------------------------------------------------

Engine.gameState = {}

---@class MetaEngineObjectFlags
---@field noCollision boolean
---@field onGround boolean
---@field ignoreGravity boolean
---@field inWater boolean
---@field stationary boolean
---@field noCollision2 boolean
---@field hasSoundLoopingAttachment boolean
---@field connectedToMap boolean
---@field notPlacedAutomatically boolean
---@field isDeviceMachine boolean
---@field isElevator boolean
---@field isElevator2 boolean
---@field isGarbage boolean
---@field noShadow boolean
---@field deleteAtDeactivation boolean
---@field doNotReactivate boolean
---@field outsideOfMap boolean
---@field collidable boolean
---@field hasCollisionModel boolean

---@class MetaEngineObjectNetwork
---@field validPosition boolean
---@field position MetaEngineVector3D
---@field validForwardAndUp boolean
---@field orientation table<MetaEnginePoint3D>
---@field validTransitionalVelocity boolean
---@field transitionalVelocity MetaEnginePoint3D
---@field validTimestamp boolean
---@field timestamp integer

---@class MetaEngineScenarioLocation
---@field leafId integer
---@field clusterIndex integer

---@class MetaEngineObjectVitalsFlags
---@field healthDamageEffectApplied boolean
---@field shieldDamageEffectApplied boolean
---@field healthDepleted boolean
---@field shieldDepleted boolean
---@field killed boolean
---@field killedSilent boolean
---@field cannotMeleeAttack boolean
---@field invulnerable boolean
---@field shieldRecharging boolean
---@field killedNoStats boolean

---@class MetaEngineObjectAttachmentsData
---@field types table<string>
---@field attachments table<integer>
---@field firstWidget integer

---@class MetaEngineObjectRegionDestroyeds
---@field region0 boolean
---@field region1 boolean
---@field region2 boolean
---@field region3 boolean
---@field region4 boolean
---@field region5 boolean
---@field region6 boolean
---@field region7 boolean

---@class MetaEngineObjectBlockReference
---@field size integer
---@field offset integer

---@class MetaEngineObject
---@field tagHandle EngineTagHandle
---@field networkRole string
---@field shouldForceBaselineUpdate boolean
---@field existenceTime integer
---@field flags MetaEngineObjectFlags
---@field objectMarkerId integer
---@field network MetaEngineObjectNetwork
---@field position MetaEnginePoint3D
---@field velocity MetaEnginePoint3D
---@field orientation table<MetaEnginePoint3D>
---@field rotationVelocity MetaEngineEuler3DPYR
---@field scenarioLocation MetaEngineScenarioLocation
---@field centerPosition MetaEnginePoint3D
---@field boundingRadius number
---@field scale number
---@field type string
---@field teamOwner string
---@field multiplayerTeamOwner string
---@field nameListIndex integer
---@field movingTime integer
---@field variantIndex integer
---@field player integer
---@field ownerObject integer
---@field animationTagHandle integer
---@field animationIndex integer
---@field animationFrame integer
---@field animationInterpolationFrame integer
---@field animationInterpolationFrameCount integer
---@field vitals MetaEngineObjectVitalsFlags
---@field clusterPartition integer
---@field unknownObject integer
---@field nextObject integer
---@field firstObject integer
---@field parentObject integer
---@field parentAttachmentNode integer
---@field forceShieldUpdate boolean
---@field attachmentData MetaEngineObjectAttachmentsData
---@field cachedRenderState integer
---@field regionDestroyeds MetaEngineObjectRegionDestroyeds
---@field shaderPermutation integer
---@field regionHealths table<integer>
---@field regionPermutationIds table<integer>
---@field colorChange table<MetaEngineColorRGB>
---@field colorChange2 table<MetaEngineColorRGB>
---@field nodeOrientations table<MetaEngineObjectBlockReference>
---@field nodeMatricesBlock MetaEngineObjectBlockReference

---@class MetaEngineUnitFlags
---@field unknownBipedSpeechRelated boolean
---@field powerUp boolean
---@field powerUpAddition boolean
---@field controllable boolean
---@field berserking boolean
---@field unknownIntegratedLightRelated boolean
---@field willNotDropItems boolean
---@field unknown boolean
---@field canBlink boolean
---@field impervious boolean
---@field suspended boolean
---@field blind boolean
---@field unknownNvRelated boolean
---@field possessed boolean
---@field desiresFlashlightOn boolean
---@field desiresFlashlightOff boolean

---@class MetaEngineUnitControlFlags
---@field crouch boolean
---@field jump boolean
---@field user1 boolean
---@field user2 boolean
---@field light boolean
---@field exactFacing boolean
---@field action boolean
---@field melee boolean
---@field lookDontTurn boolean
---@field forceAlert boolean
---@field reload boolean
---@field primaryTrigger boolean
---@field secondaryTrigger boolean
---@field grenade boolean
---@field exchangeWeapon boolean

---@class MetaEngineUnitAnimationFlags
---@field animationBit0Unknown boolean
---@field animationBit1Unknown boolean
---@field animationBit2Unknown boolean
---@field animationBit3Unknown boolean

---@class MetaEngineUnitAnimationStateData
---@field animationIndex integer
---@field frameIndex integer

---@class MetaEngineUnitAnimationData
---@field flags MetaEngineUnitAnimationFlags
---@field unknownSomeAnimationIndexMaybe integer
---@field unknownSomeAnimationIndex integer
---@field seatIndex integer
---@field seatWeaponIndex integer
---@field weaponTypeIndex integer
---@field state string
---@field replacementState string
---@field overlayState string
---@field desiredState string
---@field baseSeat string
---@field emotion integer
---@field replacementAnimation MetaEngineUnitAnimationStateData
---@field overlayStateAnimation MetaEngineUnitAnimationStateData
---@field weaponIK MetaEngineUnitAnimationStateData
---@field updateLook boolean
---@field updateAim boolean
---@field lookingBounds MetaEngineRectangle2DF
---@field aimingBounds MetaEngineRectangle2DF

---@class MetaEngineAiCommunicationPacket
---@field type string
---@field broken boolean

---@class MetaEngineUnitSpeech
---@field priority string
---@field screamType string
---@field soundTag integer
---@field ticks integer
---@field unknown2 integer
---@field unknown integer
---@field aiCommunicationInfo MetaEngineAiCommunicationPacket

---@class MetaEngineUnitSpeechData
---@field current MetaEngineUnitSpeech
---@field next MetaEngineUnitSpeech
---@field unknown0 integer
---@field unknown1 integer
---@field unknown2 integer
---@field unknown3 integer
---@field unknown4 integer
---@field unknown6 boolean
---@field unknown7 boolean
---@field unknown8 boolean
---@field unknown9 integer
---@field unknown10 integer
---@field unknown11 integer
---@field unknown12 integer
---@field unknown13 integer

---@class MetaEngineUnitRecentDamager
---@field lastDamageTime integer
---@field totalDamage number
---@field object integer
---@field player integer

---@class MetaEngineUnitControlData
---@field animationState integer
---@field aimingSpeed integer
---@field controlFlags MetaEngineUnitControlFlags
---@field weaponIndex integer
---@field grenadeIndex integer
---@field zoomIndex integer
---@field throttle MetaEngineVector3D
---@field primaryTrigger number
---@field facingVector MetaEngineVector3D
---@field aimingVector MetaEngineVector3D
---@field lookingVector MetaEngineVector3D

---@class MetaEngineUnitObject: MetaEngineObject
---@field actor integer
---@field swarmActor integer
---@field swarmNextUnit integer
---@field swarmPreviousUnit integer
---@field unitFlags MetaEngineUnitFlags
---@field unitControlFlags MetaEngineUnitControlFlags
---@field shieldSnapping integer
---@field baseSeatIndex integer
---@field persistentControlTicksRemaining integer
---@field persistentControlControlFlags MetaEngineUnitControlFlags
---@field controllingPlayer integer
---@field aiEffectType integer
---@field emotionAnimationIndex integer
---@field nextAiEffectTick integer
---@field desiredFacingVector MetaEngineVector3D
---@field desiredAimingVector MetaEngineVector3D
---@field aimingVector MetaEngineVector3D
---@field aimingVelocity MetaEngineVector3D
---@field lookingAngles MetaEngineEuler3D
---@field lookingVector MetaEngineVector3D
---@field lookingVelocity MetaEngineVector3D
---@field throttle MetaEngineVector3D
---@field primaryTrigger number
---@field aimingSpeed integer
---@field meleeState integer
---@field meleeTimer integer
---@field ticksUntilFlameToDeath integer
---@field pingAnimationTicksLeft integer
---@field grenadeState string
---@field unknown725 integer
---@field unknown726 integer
---@field grenadeProjectile integer
---@field animation MetaEngineUnitAnimationData
---@field ambient number
---@field illumination number
---@field mouthFactor number
---@field vehicleSeatId integer
---@field currentWeaponId integer
---@field nextWeaponId integer
---@field weapons table<integer>
---@field weaponReadyTicks table<integer>
---@field equipmentHandle integer
---@field currentGrenadeIndex integer
---@field nextGrenadeIndex integer
---@field grenadeCounts table<integer>
---@field zoomLevel integer
---@field desiredZoomLevel integer
---@field ticksSinceLastVehicleSpeech integer
---@field aimingChange integer
---@field poweredSeatsRiders table<integer>
---@field unknown22 integer
---@field someTickTime integer
---@field encounterId integer
---@field squadId integer
---@field poweredSeatsPower table<number>
---@field integratedLightPower number
---@field integratedLightTogglePower number
---@field integratedNightVisionTogglePower number
---@field seatRelated table<MetaEngineVector3D>
---@field camoPower number
---@field fullSpectrumVisionPower number
---@field dialogueDefinition integer
---@field speech MetaEngineUnitSpeechData
---@field damageResultCategory string
---@field damageResultAiTicksUntilHandle integer
---@field damageResultAmount number
---@field damageResultResponsibleUnit integer
---@field objectFlameCauser integer
---@field unknown23 number
---@field diedAtTick integer
---@field feignDeathTimer integer
---@field camoRegrowth boolean
---@field stun number
---@field stunTicks integer
---@field spreeCount integer
---@field spreeStartingTime integer
---@field recentDamage table<MetaEngineUnitRecentDamager>
---@field opensauceZoomLevel integer
---@field opensauceDesiredZoomLevel integer
---@field controlData MetaEngineUnitControlData
---@field lastCompletedClientUpdateValid boolean
---@field lastCompletedClientUpdateId integer

---@class MetaEngineBipedFlags
---@field airborne boolean
---@field slipping boolean
---@field absoluteMovement boolean
---@field noCollision boolean
---@field passesThroughOtherBipeds boolean
---@field limping2 boolean

---@alias BipedMovementState
---| 'moving'
---| 'idle'
---| 'gesturing'
---| 'unknown'

---@class MetaEngineBipedNetworkDelta
---@field grenadeCounts table<number>
---@field bodyVitality number
---@field shieldVitality number
---@field shieldStunTicksGreaterThanZero boolean

---@class MetaEngineBipedNetwork
---@field baselineValid boolean
---@field baselineId integer
---@field messageId integer
---@field updateBaseline MetaEngineBipedNetworkDelta
---@field deltaValid boolean
---@field updateDelta MetaEngineBipedNetworkDelta

---@class MetaEngineBipedObject: MetaEngineUnitObject
---@field bipedFlags MetaEngineBipedFlags
---@field landingTimer integer
---@field landingForce integer
---@field movementState BipedMovementState
---@field bipedUnknown3 integer
---@field actionFlags integer
---@field bipedUnknown4 integer
---@field bipedPosition MetaEngineVector3D
---@field walkingCounter integer
---@field bumpObject integer
---@field ticksSinceLastBump integer
---@field airborneTicks integer
---@field slippingTicks integer
---@field digitalThrottle integer
---@field jumpTicks integer
---@field meleeTicks integer
---@field meleeInflictTicks integer
---@field unknownBiped2 integer
---@field crouchScale number
---@field unknownBiped1 number
---@field unknownBipedPhysicsRelated MetaEnginePlane3D
---@field network MetaEngineBipedNetwork

---@class MetaEngineVehicleFlags
---@field vehicleUnknown0 boolean
---@field hovering boolean
---@field crouched boolean
---@field jumping boolean
---@field unknownVehicle1 boolean
---@field unknownVehicle2 integer

---@class MetaEngineVehicleNetworkData
---@field atRest boolean
---@field position MetaEngineVector3D
---@field transitionalVelocity MetaEngineVector3D
---@field angularVelocity MetaEngineVector3D
---@field forward MetaEngineVector3D
---@field up MetaEngineVector3D

---@class MetaEngineVehicleNetwork
---@field timeValid boolean
---@field baselineValid boolean
---@field baselineId integer
---@field messageId integer
---@field updateBaseline MetaEngineVehicleNetworkData
---@field deltaValid boolean
---@field updateDelta MetaEngineVehicleNetworkData
---@field lastMovedAtTick integer
---@field scenarioRespawnId integer
---@field respawnPosition EngineVector3D

---@class MetaEngineVehicleObject
---@field vehicleFlags MetaEngineVehicleFlags
---@field speed number
---@field slide number
---@field turn number
---@field tirePosition number
---@field threadPositionLeft number
---@field threadPositionRight number
---@field hover number
---@field thrust number
---@field suspensionStates table<integer>
---@field hoverPosition MetaEngineVector3D
---@field unknownVehicle3 MetaEngineVector3D
---@field unknownVehicle4 MetaEngineVector3D
---@field unknownVehicle5 integer
---@field network MetaEngineVehicleNetwork

---@class MetaEngineItemObject
---@field flags integer
---@field ticksUntilDetonation integer
---@field bspCollisionSurfaceId integer
---@field bspCollisionReferenceId integer
---@field droppedByUnit integer
---@field lastUpdateTick integer
---@field objectCollisionObject integer
---@field objectCollisionObjectPosition MetaEngineVector3D
---@field unknownCollisionPosition MetaEngineVector3D
---@field unknownCollisionAngle MetaEngineEuler2D

---@class MetaEngineWeaponTrigger
---@field idleTime integer
---@field state string
---@field triggerTime integer
---@field notFiring integer
---@field autoReload integer
---@field roundsSinceLastTracer integer
---@field rateOfFire number
---@field ejectionPortRecoveryTime number
---@field illuminationRecoveryTime number
---@field projectileErrorRelated number
---@field charingEffect integer
---@field networkDelayTicks integer

---@class MetaEngineWeaponMagazine
---@field state string
---@field reloadTicksRemaining integer
---@field reloadTicks integer
---@field roundsUnloaded integer
---@field roundsLoaded integer
---@field roundsLeftToRecharge integer
---@field unknown integer
---@field unknown2 integer

---@class MetaEngineWeaponReloadStartData
---@field totalRounds table<integer>
---@field loadedRounds table<integer>

---@class MetaEngineWeaponNetworkData
---@field position MetaEngineVector3D
---@field transitionalVelocity MetaEngineVector3D
---@field angularVelocity MetaEngineVector3D
---@field magazineRoundsTotal table<number>
---@field age number

---@class MetaEngineWeaponNetwork
---@field baselineValid boolean
---@field baselineIndex integer
---@field messageIndex integer
---@field updateBaseline MetaEngineWeaponNetworkData
---@field deltaValid boolean
---@field updateDelta MetaEngineWeaponNetworkData

---@class MetaEngineWeaponObject
---@field flags integer
---@field ownerUnitFlags integer
---@field primaryTrigger number
---@field weaponState string
---@field readyTicks integer
---@field heat number
---@field age number
---@field illuminationFraction number
---@field integratedLightPower number
---@field trackedObject integer
---@field altShotsLoaded integer
---@field triggers table<MetaEngineWeaponTrigger>
---@field magazines table<MetaEngineWeaponMagazine>
---@field lastTriggerFireTick integer
---@field reloadStartingPoint MetaEngineWeaponReloadStartData
---@field network MetaEngineWeaponNetwork

---@class MetaEngineEquipmentNetworkData
---@field position MetaEngineVector3D
---@field transitionalVelocity MetaEngineVector3D
---@field angularVelocity MetaEngineVector3D

---@class MetaEngineEquipmentNetwork
---@field baselineValid boolean
---@field baselineIndex integer
---@field messageIndex integer
---@field updateBaseline MetaEngineEquipmentNetworkData
---@field deltaValid boolean
---@field updateDelta MetaEngineEquipmentNetworkData

---@class MetaEngineEquipmentObject
---@field network MetaEngineEquipmentNetwork

---@class MetaEngineGarbageObject
---@field ticksUntilGarbageCollection integer

---@class MetaEngineProjectileObjectFlags
---@field tracer boolean
---@field projectileUnknownBit boolean
---@field attached boolean

---@class MetaEngineProjectileNetworkData
---@field position MetaEngineVector3D
---@field transitionalVelocity MetaEngineVector3D

---@class MetaEngineProjectileNetwork
---@field unknown boolean
---@field baselineValid boolean
---@field baselineIndex integer
---@field messageIndex integer
---@field updateBaseline MetaEngineProjectileNetworkData
---@field deltaValid boolean
---@field updateDelta MetaEngineProjectileNetworkData

---@class MetaEngineProjectileObject
---@field projectileFlags MetaEngineProjectileObjectFlags
---@field actionEnum integer
---@field materialId integer
---@field sourceUnit integer
---@field targetObject integer
---@field contrailAttachmentBlockId integer
---@field timeRemaining number
---@field armingRate number
---@field unknownProjFloat1 number
---@field unknownProjFloat2 number
---@field distanceTravelled number
---@field transitionalVelocity MetaEngineVector3D
---@field waterDamageUpperBound number
---@field angularVelocity MetaEngineVector3D
---@field unknownEuler MetaEngineEuler2D
---@field network MetaEngineProjectileNetwork

---@class MetaEngineSceneryObject: MetaEngineObject

---@class MetaEngineDeviceMachineObjectFlags
---@field doesNotOperateAutomatically boolean
---@field machineOneSided boolean
---@field neverAppearsLocked boolean
---@field openedByMeleeAttack boolean

---@class MetaEngineDeviceMachineObject
---@field deviceFlags MetaEngineDeviceMachineObjectFlags
---@field ticksSinceStartedOpening integer
---@field elevatorPosition MetaEngineVector3D

---@class MetaEngineDeviceControlObjectFlags
---@field usableFromBothSides boolean

---@class MetaEngineDeviceControlObject
---@field deviceControlFlags MetaEngineDeviceControlObjectFlags
---@field customNameId integer

---@class MetaEngineDeviceLightFixtureObject
---@field lightColor MetaEngineColorRGB
---@field lightIntensity number
---@field lightFalloffAngle number
---@field lightCutoffAngle number

---@class MetaEnginePlaceholderObject: MetaEngineObject

---@class MetaEngineSoundSceneryObject: MetaEngineObject

-- Get an object of the current game
---@param handle integer @The handle of the object
---@return MetaEngineObject
---@overload fun(handle: integer, objectType: "biped"): MetaEngineBipedObject
---@overload fun(handle: integer, objectType: "vehicle"): MetaEngineVehicleObject
---@overload fun(handle: integer, objectType: "weapon"): MetaEngineWeaponObject
---@overload fun(handle: integer, objectType: "equipment"): MetaEngineEquipmentObject
---@overload fun(handle: integer, objectType: "garbage"): MetaEngineGarbageObject
---@overload fun(handle: integer, objectType: "projectile"): MetaEngineProjectileObject
---@overload fun(handle: integer, objectType: "scenery"): MetaEngineSceneryObject
---@overload fun(handle: integer, objectType: "device_machine"): MetaEngineDeviceMachineObject
---@overload fun(handle: integer, objectType: "device_control"): MetaEngineDeviceControlObject
---@overload fun(handle: integer, objectType: "device_light_fixture"): MetaEngineDeviceLightFixtureObject
---@overload fun(handle: integer, objectType: "placeholder"): MetaEnginePlaceholderObject
---@overload fun(handle: integer, objectType: "sound_scenery"): MetaEngineSoundSceneryObject
function Engine.gameState.getObject(handle) end

-- Spawn an object
---@param tagHandle EngineTagHandle @The tag handle of the object
---@param parentObjectHandle? integer @The handle of the parent object
---@param position EnginePoint3D @The position of the object
---@return EngineObjectHandle @The handle of the object
function Engine.gameState.createObject(tagHandle, parentObjectHandle, position) end

-- Delete the object
---@param objectHandle integer @The handle of the object
function Engine.gameState.deleteObject(objectHandle) end

-- Make a unit enter a vehicle
---@param unitObjectHandle integer @The handle of the unit
---@param vehicleObjectHandle integer @The handle of the vehicle
---@param seatIndex integer @The index of the seat
function Engine.gameState.unitEnterVehicle(unitObjectHandle, vehicleObjectHandle, seatIndex) end

---@alias EngineNetworkPlayerColor
---| 'white'
---| 'black'
---| 'red'
---| 'blue'
---| 'gray'
---| 'yellow'
---| 'green'
---| 'pink'
---| 'purple'
---| 'cyan'
---| 'cobalt'
---| 'orange'
---| 'teal'
---| 'sage'
---| 'brown'
---| 'tan'
---| 'maroon'
---| 'salmon'
---| 'unknown'

---@alias EnginePlayerObjectiveMode
---| 'race'
---| 'hill'
---| 'juggernaut'
---| 'ball'
---| 'none'
---| 'unknown'

---@class MetaEnginePlayerMultiplayerStatistics
---@field ctfFlagGrabs integer
---@field ctfFlagReturns integer
---@field ctfFlagScores integer
---@field oddballTargetKills integer
---@field oddballKills integer
---@field oddballUnknown integer
---@field kingHillScore integer
---@field raceTime integer
---@field raceLaps integer
---@field raceBestTime integer

---@class MetaEnginePlayer
---@field playerId integer
---@field localHandle integer
---@field name string
---@field unknownHandle integer
---@field team integer
---@field interactionObjectHandle integer
---@field interactionObjectType integer
---@field interactionObjectSeat integer
---@field respawnTime integer
---@field respawnTimeGrowth integer
---@field objectHandle integer
---@field prevObjectHandle integer
---@field bspClusterId integer
---@field weaponSwapResult boolean
---@field autoAimTargetObject integer
---@field lastFireTime integer
---@field name2 string
---@field color EngineNetworkPlayerColor
---@field iconIndex integer
---@field machineIndex integer
---@field controllerIndex integer
---@field team2 integer
---@field index integer
---@field invisibilityTime integer
---@field otherPowerupTimeLeft integer
---@field speed number
---@field teleporterFlagHandle integer
---@field objectiveMode EnginePlayerObjectiveMode
---@field objectivePlayerHandle integer
---@field targetPlayer integer
---@field targetTime integer
---@field lastDeathTime integer
---@field slayerTarget integer
---@field oddManOut boolean
---@field killStreak integer
---@field multikill integer
---@field lastKillTime integer
---@field kills table
---@field assists table
---@field betrays integer
---@field deaths integer
---@field suicides integer
---@field teamKills integer
---@field multiplayerStatistics MetaEnginePlayerMultiplayerStatistics
---@field telefragTimer integer
---@field quitTime integer
---@field telefragDanger boolean
---@field quit boolean
---@field ping integer
---@field teamKillCount integer
---@field teamKillTicksSinceLast integer
---@field position MetaEnginePoint3D
---@field melee boolean
---@field action boolean
---@field flashlight boolean
---@field reload boolean
---@field baselineUpdateXYAim number
---@field baselineUpdateZAim number
---@field baselineUpdateForward number
---@field baselineUpdateLeft number
---@field baselineUpdateRateOfFire number
---@field baselineUpdateWeaponSlot integer
---@field baselineUpdateGrenadeSlot integer
---@field updateAiming MetaEnginePoint3D
---@field updatePosition MetaEnginePoint3D

-- Get a player
---@param playerIndexOrHandle? integer @The index or the handle of the player; If nil, the local player is returned
---@return MetaEnginePlayer @The player
function Engine.gameState.getPlayer(playerIndexOrHandle) end

-- Get a player by their RCON handle
---@param playerRconHandle integer @The RCON handle of the player
---@return MetaEnginePlayer @The player
function Engine.gameState.getPlayerByRconHandle(playerRconHandle) end

---@alias EngineCameraType
---| 'first_person'
---| 'vehicle'
---| 'cinematic'
---| 'debug'
---| 'unknown'

-- Get the camera type
---@return EngineCameraType
function Engine.core.getCameraType() end

---@class EngineCameraData
---@field position EnginePoint3D
---@field orientation table<EnginePoint3D>
---@field fov number

-- Get the camera data
---@return EngineCameraData
function Engine.core.getCameraData() end

-------------------------------------------------------
-- Engine.map
-------------------------------------------------------

Engine.map = {}

---@alias EngineCacheFileEngine
---| 'xbox'
---| 'demo'
---| 'retail'
---| 'custom'
---| 'invader'
---| 'demo compressed'
---| 'retail compressed'
---| 'custom compressed'
---| 'unknown'

---@alias EngineMapGameType
---| 'single_player'
---| 'multiplayer'
---| 'user_interface'

---@class EngineMapHeader
---@field engineType EngineCacheFileEngine
---@field fileSize integer
---@field tagDataOffset integer
---@field tagDataSize integer
---@field name string
---@field build string
---@field gameType EngineMapGameType
---@field crc32 integer

-- Get the map header of the currently loaded map
---@return EngineMapHeader
function Engine.map.getCurrentMapHeader() end

-- Get the map list
---@return table<string> @The map list
function Engine.map.getMapList() end

-------------------------------------------------------
-- Engine.netgame
-------------------------------------------------------

Engine.netgame = {}

---@alias EngineNetworkGameServerType
---| 'none'
---| 'dedicated'
---| 'local'
---| 'unknown'

-- Get the server type
---@return EngineNetworkGameServerType
function Engine.netgame.getServerType() end

---@alias EngineNetworkGameType
---| 'ctf'
---| 'slayer'
---| 'oddball'
---| 'king'
---| 'race'
---| 'none'

---@alias EngineNetworkGameMessageHudChatType
---| 'none'
---| 'all'
---| 'team'
---| 'vehicle'
---| 'unknown'

-- Get the server gametype
---@return EngineNetworkGameType @The server gametype
function Engine.netgame.getServerGametype() end

-- Get if the current game is a team game
---@return boolean @If the current game is a team game
function Engine.netgame.currentGameIsTeam() end

-------------------------------------------------------
-- Engine.tag
-------------------------------------------------------

Engine.tag = {}

---@class EngineTagDataHeader
---@field tagArrayAddress integer
---@field scenarioTagHandle integer 
---@field tagCount integer
---@field modelPartCount integer
---@field modelDataFileOffset integer
---@field vertexSize integer
---@field modelDataSize integer

-- Get the tag data header
---@return EngineTagDataHeader @The tag data header
function Engine.tag.getTagDataHeader() end

---@alias EngineTagClass
---| 'actor'
---| 'actor_variant'
---| 'antenna'
---| 'model_animations'
---| 'biped'
---| 'bitmap'
---| 'spheroid'
---| 'continuous_damage_effect'
---| 'model_collision_geometry'
---| 'color_table'
---| 'contrail'
---| 'device_control'
---| 'decal'
---| 'ui_widget_definition'
---| 'input_device_defaults'
---| 'device'
---| 'detail_object_collection'
---| 'effect'
---| 'equipment'
---| 'flag'
---| 'fog'
---| 'font'
---| 'lightning'
---| 'material_effects'
---| 'garbage'
---| 'glow'
---| 'grenade_hud_interface'
---| 'hud_message_text'
---| 'hud_number'
---| 'hud_globals'
---| 'item'
---| 'item_collection'
---| 'damage_effect'
---| 'lens_flare'
---| 'device_light_fixture'
---| 'light'
---| 'sound_looping'
---| 'device_machine'
---| 'globals'
---| 'meter'
---| 'light_volume'
---| 'gbxmodel'
---| 'model'
---| 'multiplayer_scenario_description'
---| 'preferences_network_game'
---| 'object'
---| 'particle'
---| 'particle_system'
---| 'physics'
---| 'placeholder'
---| 'point_physics'
---| 'projectile'
---| 'weather'
---| 'scenario_structure_bsp'
---| 'scenery'
---| 'shader_transparent_chicago_extended'
---| 'shader_transparent_chicago'
---| 'scenario'
---| 'shader_environment'
---| 'transparent_glass'
---| 'shader'
---| 'sky'
---| 'shader_transparent_meter'
---| 'sound'
---| 'sound_environment'
---| 'shader_model'
---| 'shader_transparent_generic'
---| 'ui_widget_collection'
---| 'shader_transparent_plasma'
---| 'sound_scenery'
---| 'string_list'
---| 'shader_transparent_water'
---| 'tag_collection'
---| 'camera_track'
---| 'unit_dialogue'
---| 'unit_hud_interface'
---| 'unit'
---| 'unicode_string_list'
---| 'virtual_keyboard'
---| 'vehicle'
---| 'weapon'
---| 'wind'
---| 'weapon_hud_interface'
---| 'none'

---@class EngineTag
---@field primaryClass EngineTagClass
---@field secondaryClass integer
---@field tertiaryClass integer
---@field handle EngineTagHandle
---@field path string
---@field dataAddress integer
---@field indexed boolean

---@class MetaEngineTag: EngineTag

---@class EngineActorTag: MetaEngineTag
---@field data MetaEngineTagDataActor

---@class EngineActorVariantTag: MetaEngineTag
---@field data MetaEngineTagDataActorVariant

---@class EngineAntennaTag: MetaEngineTag
---@field data MetaEngineTagDataAntenna

---@class EngineModelAnimationsTag: MetaEngineTag
---@field data MetaEngineTagDataModelAnimations

---@class EngineBipedTag: MetaEngineTag
---@field data MetaEngineTagDataBiped

---@class EngineBitmapTag: MetaEngineTag
---@field data MetaEngineTagDataBitmap

---@class EngineContinuousDamageEffectTag: MetaEngineTag
---@field data MetaEngineTagDataContinuousDamageEffect

---@class EngineModelCollisionGeometryTag: MetaEngineTag
---@field data MetaEngineTagDataModelCollisionGeometry

---@class EngineColorTableTag: MetaEngineTag
---@field data MetaEngineTagDataColorTable

---@class EngineContrailTag: MetaEngineTag
---@field data MetaEngineTagDataContrail

---@class EngineDeviceControlTag: MetaEngineTag
---@field data MetaEngineTagDataDeviceControl

---@class EngineDecalTag: MetaEngineTag
---@field data MetaEngineTagDataDecal

---@class EngineUiWidgetDefinitionTag: MetaEngineTag
---@field data MetaEngineTagDataUiWidgetDefinition

---@class EngineInputDeviceDefaultsTag: MetaEngineTag
---@field data MetaEngineTagDataInputDeviceDefaults

---@class EngineDeviceTag: MetaEngineTag
---@field data MetaEngineTagDataDevice

---@class EngineDetailObjectCollectionTag: MetaEngineTag
---@field data MetaEngineTagDataDetailObjectCollection

---@class EngineEffectTag: MetaEngineTag
---@field data MetaEngineTagDataEffect

---@class EngineEquipmentTag: MetaEngineTag
---@field data MetaEngineTagDataEquipment

---@class EngineFlagTag: MetaEngineTag
---@field data MetaEngineTagDataFlag

---@class EngineFogTag: MetaEngineTag
---@field data MetaEngineTagDataFog

---@class EngineFontTag: MetaEngineTag
---@field data MetaEngineTagDataFont

---@class EngineLightningTag: MetaEngineTag
---@field data MetaEngineTagDataLightning

---@class EngineMaterialEffectsTag: MetaEngineTag
---@field data MetaEngineTagDataMaterialEffects

---@class EngineGarbageTag: MetaEngineTag
---@field data MetaEngineTagDataGarbage

---@class EngineGlowTag: MetaEngineTag
---@field data MetaEngineTagDataGlow

---@class EngineGrenadeHudInterfaceTag: MetaEngineTag
---@field data MetaEngineTagDataGrenadeHudInterface

---@class EngineHudMessageTextTag: MetaEngineTag
---@field data MetaEngineTagDataHudMessageText

---@class EngineHudNumberTag: MetaEngineTag
---@field data MetaEngineTagDataHudNumber

---@class EngineHudGlobalsTag: MetaEngineTag
---@field data MetaEngineTagDataHudGlobals

---@class EngineItemTag: MetaEngineTag
---@field data MetaEngineTagDataItem

---@class EngineItemCollectionTag: MetaEngineTag
---@field data MetaEngineTagDataItemCollection

---@class EngineDamageEffectTag: MetaEngineTag
---@field data MetaEngineTagDataDamageEffect

---@class EngineLensFlareTag: MetaEngineTag
---@field data MetaEngineTagDataLensFlare

---@class EngineDeviceLightFixtureTag: MetaEngineTag
---@field data MetaEngineTagDataDeviceLightFixture

---@class EngineLightTag: MetaEngineTag
---@field data MetaEngineTagDataLight

---@class EngineSoundLoopingTag: MetaEngineTag
---@field data MetaEngineTagDataSoundLooping

---@class EngineDeviceMachineTag: MetaEngineTag
---@field data MetaEngineTagDataDeviceMachine

---@class EngineGlobalsTag: MetaEngineTag
---@field data MetaEngineTagDataGlobals

---@class EngineMeterTag: MetaEngineTag
---@field data MetaEngineTagDataMeter

---@class EngineLightVolumeTag: MetaEngineTag
---@field data MetaEngineTagDataLightVolume

---@class EngineGbxmodelTag: MetaEngineTag
---@field data MetaEngineTagDataGbxmodel

---@class EngineModelTag: MetaEngineTag
---@field data MetaEngineTagDataModel

---@class EngineMultiplayerScenarioDescriptionTag: MetaEngineTag
---@field data MetaEngineTagDataMultiplayerScenarioDescription

---@class EnginePreferencesNetworkGameTag: MetaEngineTag
---@field data MetaEngineTagDataPreferencesNetworkGame

---@class EngineObjectTag: MetaEngineTag
---@field data MetaEngineTagDataObject

---@class EngineParticleTag: MetaEngineTag
---@field data MetaEngineTagDataParticle

---@class EngineParticleSystemTag: MetaEngineTag
---@field data MetaEngineTagDataParticleSystem

---@class EnginePhysicsTag: MetaEngineTag
---@field data MetaEngineTagDataPhysics

---@class EnginePlaceholderTag: MetaEngineTag
---@field data MetaEngineTagDataPlaceholder

---@class EnginePointPhysicsTag: MetaEngineTag
---@field data MetaEngineTagDataPointPhysics

---@class EngineProjectileTag: MetaEngineTag
---@field data MetaEngineTagDataProjectile

---@class EngineWeatherParticleSystemTag: MetaEngineTag
---@field data MetaEngineTagDataWeatherParticleSystem

---@class EngineScenarioStructureBspTag: MetaEngineTag
---@field data MetaEngineTagDataScenarioStructureBsp

---@class EngineSceneryTag: MetaEngineTag
---@field data MetaEngineTagDataScenery

---@class EngineShaderTransparentChicagoExtendedTag: MetaEngineTag
---@field data MetaEngineTagDataShaderTransparentChicagoExtended

---@class EngineShaderTransparentChicagoTag: MetaEngineTag
---@field data MetaEngineTagDataShaderTransparentChicago

---@class EngineScenarioTag: MetaEngineTag
---@field data MetaEngineTagDataScenario

---@class EngineShaderEnvironmentTag: MetaEngineTag
---@field data MetaEngineTagDataShaderEnvironment

---@class EngineShaderTransparentGlassTag: MetaEngineTag
---@field data MetaEngineTagDataShaderTransparentGlass

---@class EngineShaderTag: MetaEngineTag
---@field data MetaEngineTagDataShader

---@class EngineSkyTag: MetaEngineTag
---@field data MetaEngineTagDataSky

---@class EngineShaderTransparentMeterTag: MetaEngineTag
---@field data MetaEngineTagDataShaderTransparentMeter

---@class EngineSoundTag: MetaEngineTag
---@field data MetaEngineTagDataSound

---@class EngineSoundEnvironmentTag: MetaEngineTag
---@field data MetaEngineTagDataSoundEnvironment

---@class EngineShaderModelTag: MetaEngineTag
---@field data MetaEngineTagDataShaderModel

---@class EngineShaderTransparentGenericTag: MetaEngineTag
---@field data MetaEngineTagDataShaderTransparentGeneric

---@class MetaEngineTagDataUiWidgetCollection: MetaEngineTagDataTagCollection

---@class EngineUiWidgetCollectionTag: MetaEngineTag
---@field data MetaEngineTagDataUiWidgetCollection

---@class EngineShaderTransparentPlasmaTag: MetaEngineTag
---@field data MetaEngineTagDataShaderTransparentPlasma

---@class EngineSoundSceneryTag: MetaEngineTag
---@field data MetaEngineTagDataSoundScenery

---@class EngineStringListTag: MetaEngineTag
---@field data MetaEngineTagDataStringList

---@class EngineShaderTransparentWaterTag: MetaEngineTag
---@field data MetaEngineTagDataShaderTransparentWater

---@class EngineTagCollectionTag: MetaEngineTag
---@field data MetaEngineTagDataTagCollection

---@class EngineCameraTrackTag: MetaEngineTag
---@field data MetaEngineTagDataCameraTrack

---@class EngineUnitHudInterfaceTag: MetaEngineTag
---@field data MetaEngineTagDataUnitHudInterface

---@class EngineUnitTag: MetaEngineTag
---@field data MetaEngineTagDataUnit

---@class EngineUnicodeStringListTag: MetaEngineTag
---@field data MetaEngineTagDataUnicodeStringList

---@class EngineVirtualKeyboardTag: MetaEngineTag
---@field data MetaEngineTagDataVirtualKeyboard

---@class EngineVehicleTag: MetaEngineTag
---@field data MetaEngineTagDataVehicle

---@class EngineWeaponTag: MetaEngineTag
---@field data MetaEngineTagDataWeapon

---@class EngineWindTag: MetaEngineTag
---@field data MetaEngineTagDataWind

---@class EngineWeaponHudInterfaceTag: MetaEngineTag
---@field data MetaEngineTagDataWeaponHudInterface

-- Get a tag
---@param tagHandleOrPath integer @The handle or path of the tag
---@param tagClass? EngineTagClass @The class of the tag; If first argument is a handle, this argument can be skipped
---@return EngineTag|nil @The tag; nil if not found
---@overload fun(tagHandleOrPath: string|integer, tagClass: "actor"): EngineActorTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "actor_variant"): EngineActorVariantTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "antenna"): EngineAntennaTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "model_animations"): EngineModelAnimationsTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "biped"): EngineBipedTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "bitmap"): EngineBitmapTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "continuous_damage_effect"): EngineContinuousDamageEffectTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "model_collision_geometry"): EngineModelCollisionGeometryTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "color_table"): EngineColorTableTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "contrail"): EngineContrailTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "device_control"): EngineDeviceControlTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "decal"): EngineDecalTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "ui_widget_definition"): EngineUiWidgetDefinitionTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "input_device_defaults"): EngineInputDeviceDefaultsTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "device"): EngineDeviceTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "detail_object_collection"): EngineDetailObjectCollectionTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "effect"): EngineEffectTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "equipment"): EngineEquipmentTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "flag"): EngineFlagTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "fog"): EngineFogTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "font"): EngineFontTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "lightning"): EngineLightningTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "material_effects"): EngineMaterialEffectsTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "garbage"): EngineGarbageTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "glow"): EngineGlowTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "grenade_hud_interface"): EngineGrenadeHudInterfaceTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "hud_message_text"): EngineHudMessageTextTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "hud_number"): EngineHudNumberTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "hud_globals"): EngineHudGlobalsTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "item"): EngineItemTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "item_collection"): EngineItemCollectionTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "damage_effect"): EngineDamageEffectTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "lens_flare"): EngineLensFlareTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "device_light_fixture"): EngineDeviceLightFixtureTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "light"): EngineLightTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "sound_looping"): EngineSoundLoopingTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "device_machine"): EngineDeviceMachineTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "globals"): EngineGlobalsTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "meter"): EngineMeterTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "light_volume"): EngineLightVolumeTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "gbxmodel"): EngineGbxmodelTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "model"): EngineModelTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "multiplayer_scenario_description"): EngineMultiplayerScenarioDescriptionTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "preferences_network_game"): EnginePreferencesNetworkGameTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "object"): EngineObjectTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "particle"): EngineParticleTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "particle_system"): EngineParticleSystemTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "physics"): EnginePhysicsTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "placeholder"): EnginePlaceholderTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "point_physics"): EnginePointPhysicsTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "projectile"): EngineProjectileTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "weather"): EngineWeatherParticleSystemTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "scenario_structure_bsp"): EngineScenarioStructureBspTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "scenery"): EngineSceneryTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "shader_transparent_chicago_extended"): EngineShaderTransparentChicagoExtendedTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "shader_transparent_chicago"): EngineShaderTransparentChicagoTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "scenario"): EngineScenarioTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "shader_environment"): EngineShaderEnvironmentTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "transparent_glass"): EngineShaderTransparentGlassTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "shader"): EngineShaderTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "sky"): EngineSkyTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "shader_transparent_meter"): EngineShaderTransparentMeterTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "sound"): EngineSoundTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "sound_environment"): EngineSoundEnvironmentTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "shader_model"): EngineShaderModelTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "shader_transparent_generic"): EngineShaderTransparentGenericTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "ui_widget_collection"): EngineUiWidgetCollectionTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "shader_transparent_plasma"): EngineShaderTransparentPlasmaTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "sound_scenery"): EngineSoundSceneryTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "string_list"): EngineStringListTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "shader_transparent_water"): EngineShaderTransparentWaterTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "tag_collection"): EngineTagCollectionTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "camera_track"): EngineCameraTrackTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "unit_hud_interface"): EngineUnitHudInterfaceTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "unit"): EngineUnitTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "unicode_string_list"): EngineUnicodeStringListTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "virtual_keyboard"): EngineVirtualKeyboardTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "vehicle"): EngineVehicleTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "weapon"): EngineWeaponTag |nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "wind"): EngineWindTag|nil
---@overload fun(tagHandleOrPath: string|integer, tagClass: "weapon_hud_interface"): EngineWeaponHudInterfaceTag|nil
function Engine.tag.getTag(tagHandleOrPath, tagClass) end

-------------------------------------------------------
-- Engine.userInterface
-------------------------------------------------------

Engine.userInterface = {}

---@class EngineWidget
---@field definitionTagHandle EngineTagHandle
---@field name string
---@field hidden boolean
---@field leftBound integer
---@field topBound integer
---@field type EngineTagDataUIWidgetType
---@field msToClose integer
---@field msToCloseFadeTime integer
---@field opacity number
---@field previousWidget MetaEngineWidget|nil
---@field nextWidget MetaEngineWidget|nil
---@field parentWidget MetaEngineWidget|nil
---@field childWidget MetaEngineWidget|nil
---@field focusedWidget MetaEngineWidget|nil
---@field textAddress integer @The address of the text; null (0) if the widget is not a text widget, be careful!
---@field cursorIndex integer
---@field bitmapIndex integer

---@class MetaEngineWidget: EngineWidget

---@alias EngineWidgetNavigationSound
---| 'cursor'
---| 'forward'
---| 'back'
---| 'flag_failure'

-- Find a widget from a given widget definition; it only returns the first coincidence
---@param definitionTagHandleOrPath integer|string @The handle or path of the widget definition
---@param baseWidget? MetaEngineWidget @The base widget to start the search from; If nil, the search will start from the root widget
---@return MetaEngineWidget|nil @Found widget; nil if not found
function Engine.userInterface.findWidget(definitionTagHandleOrPath, baseWidget) end

-- Find all widgets from a given widget definition
---@param definitionTagHandleOrPath integer|string @The handle or path of the widget definition
---@param baseWidget? MetaEngineWidget @The base widget to start the search from; If nil, the search will start from the root widget
---@return table<MetaEngineWidget>|nil @Found widgets; nil if any widget was found
function Engine.userInterface.findWidgets(definitionTagHandleOrPath, baseWidget) end

-- Open a widget
---@param definitionTagHandleOrPath integer|string @The handle or path of the widget definition
---@param pushHistory? boolean @If the widget should be pushed to the history; false by default
---@return MetaEngineWidget|nil @Created widget; nil if failed
function Engine.userInterface.openWidget(definitionTagHandleOrPath, pushHistory) end

-- Close current root widget; return to the previous one in history
function Engine.userInterface.closeWidget() end

-- Replace a widget
---@param targetWidget MetaEngineWidget @The widget to replace
---@param widgetDefinition integer|string @The handle or path of the widget definition
---@return MetaEngineWidget|nil @Created widget; nil if failed
function Engine.userInterface.replaceWidget(targetWidget, widgetDefinition) end

-- Reload a widget; replaces the widget with a new one with the same definition and state 
---@param targetWidget MetaEngineWidget @The widget to reload
---@return MetaEngineWidget|nil @New widget; nil if failed
function Engine.userInterface.reloadWidget(targetWidget) end

-- Focus a widget
---@param targetWidget MetaEngineWidget @The widget to focus
function Engine.userInterface.focusWidget(targetWidget) end

-- Open the pause menu
function Engine.userInterface.openPauseMenu() end

-- Gets the HUD globals tag data
---@return MetaEngineTagDataHudGlobals @The HUD globals tag data of the current map
function Engine.userInterface.getHudGlobals() end

-- Gets the size of a sprite in a bitmap
---@param bitmap MetaEngineTagDataBitmap @Bitmap data
---@param sequenceIndex integer @A valid sequence index of the bitmap
---@param spriteIndex integer @A valid sprite index of the bitmap
---@return EngineResolution|nil @The size of the sprite; nil if failed
function Engine.userInterface.getBitmapSpriteResolution(bitmap, sequenceIndex, spriteIndex) end

-- Draws a icon bitmap on a HUD message.
---@param bitmap MetaEngineTagDataBitmap @Bitmap data
---@param sequenceIndex integer @A valid sequence index of the bitmap
---@param spriteIndex integer @A valid sprite index of the bitmap
---@param position EnginePoint2D @The position of the sprite
---@param color EngineColorARGBInt @The color of the sprite
function Engine.userInterface.drawHudMessageSprite(bitmap, sequenceIndex, spriteIndex, position, color) end

-- Play a sound from a given tag
---@param soundTagHandleOrPath integer|string @The handle or path of the sound tag
function Engine.userInterface.playSound(soundTagHandleOrPath) end

-- Get the duration of a sound permutation
---@param soundPermutation MetaEngineTagDataSoundPermutation @The sound permutation
function Engine.userInterface.getSoundPermutationSamplesDuration(soundPermutation) end

-------------------------------------------------------
-- Engine.renderer
-------------------------------------------------------

---@class EngineResolution
---@field height integer
---@field width integer

-- Get the current resolution of the game
---@return EngineResolution
function Engine.core.getResolution() end
