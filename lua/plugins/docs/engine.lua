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

---@class EngineMatrix: table<integer, table<integer, number>>

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
function Engine.core.consolePrint(color, format, ...)
end

-- Get the number of ticks since the engine started
---@return integer
function Engine.core.getTickCount()
end

---@alias EngineEdition
---| 'retail'
---| 'demo'
---| 'custom'

-- Get the game engine used
---@return EngineEdition
function Engine.core.getEngineEdition()
end

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
---@field orientation table<integer, MetaEnginePoint3D>
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
---@field types table<integer, string>
---@field attachments table<integer, integer>
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

---@class MetaEngineBaseObject
---@field tagHandle EngineTagHandle
---@field networkRole string
---@field shouldForceBaselineUpdate boolean
---@field existenceTime integer
---@field flags MetaEngineObjectFlags
---@field objectMarkerId integer
---@field network MetaEngineObjectNetwork
---@field position MetaEnginePoint3D
---@field velocity MetaEnginePoint3D
---@field orientation table<integer, MetaEnginePoint3D>
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
---@field regionHealths table<integer, integer>
---@field regionPermutationIds table<integer, integer>
---@field colorChange table<integer, MetaEngineColorRGB>
---@field colorChange2 table<integer, MetaEngineColorRGB>
---@field nodeOrientations table<integer, MetaEngineObjectBlockReference>
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

---@class MetaEngineUnitObject: MetaEngineBaseObject
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
---@field weapons table<integer, integer>
---@field weaponReadyTicks table<integer, integer>
---@field equipmentHandle integer
---@field currentGrenadeIndex integer
---@field nextGrenadeIndex integer
---@field grenadeCounts table<integer, integer>
---@field zoomLevel integer
---@field desiredZoomLevel integer
---@field ticksSinceLastVehicleSpeech integer
---@field aimingChange integer
---@field poweredSeatsRiders table<integer, integer>
---@field unknown22 integer
---@field someTickTime integer
---@field encounterId integer
---@field squadId integer
---@field poweredSeatsPower table<integer, number>
---@field integratedLightPower number
---@field integratedLightTogglePower number
---@field integratedNightVisionTogglePower number
---@field seatRelated table<integer, MetaEngineVector3D>
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
---@field recentDamage table<integer, MetaEngineUnitRecentDamager>
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
---@field grenadeCounts table<integer, number>
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
---@field suspensionStates table<integer, integer>
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
---@field totalRounds table<integer, integer>
---@field loadedRounds table<integer, integer>

---@class MetaEngineWeaponNetworkData
---@field position MetaEngineVector3D
---@field transitionalVelocity MetaEngineVector3D
---@field angularVelocity MetaEngineVector3D
---@field magazineRoundsTotal table<integer, number>
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
---@field triggers table<integer, MetaEngineWeaponTrigger>
---@field magazines table<integer, MetaEngineWeaponMagazine>
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

---@class MetaEngineSceneryObject: MetaEngineBaseObject

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

---@class MetaEnginePlaceholderObject: MetaEngineBaseObject

---@class MetaEngineSoundSceneryObject: MetaEngineBaseObject

-- Get an object of the current game
---@param handle EngineObjectHandle|integer @The handle of the object
---@return MetaEngineBaseObject
function Engine.gameState.getObject(handle)
end

-- Get a biped object
---@param handle EngineObjectHandle|integer @The handle of the object
---@return MetaEngineBipedObject
function Engine.gameState.getBipedObject(handle)
end

-- Get a vehicle object
---@param handle EngineObjectHandle|integer @The handle of the object
---@return MetaEngineVehicleObject
function Engine.gameState.getVehicleObject(handle)
end

-- Get a garbage object
---@param handle EngineObjectHandle|integer @The handle of the object
---@return MetaEngineGarbageObject
function Engine.gameState.getGarbageObject(handle)
end

-- Get a weapon object
---@param handle EngineObjectHandle|integer @The handle of the object
---@return MetaEngineWeaponObject
function Engine.gameState.getWeaponObject(handle)
end

-- Get an equipment object
---@param handle EngineObjectHandle|integer @The handle of the object
---@return MetaEngineEquipmentObject
function Engine.gameState.getEquipmentObject(handle)
end

-- Get a projectile object
---@param handle EngineObjectHandle|integer @The handle of the object
---@return MetaEngineProjectileObject
function Engine.gameState.getProjectileObject(handle)
end

-- Get a device machine object
---@param handle EngineObjectHandle|integer @The handle of the object
---@return MetaEngineDeviceMachineObject
function Engine.gameState.getDeviceMachineObject(handle)
end

-- Get a device control object
---@param handle EngineObjectHandle|integer @The handle of the object
---@return MetaEngineDeviceControlObject
function Engine.gameState.getDeviceControlObject(handle)
end

-- Get a device light fixture object
---@param handle EngineObjectHandle|integer @The handle of the object
---@return MetaEngineDeviceLightFixtureObject
function Engine.gameState.getDeviceLightFixtureObject(handle)
end

-- Spawn an object
---@param tagHandle EngineTagHandle|integer @The tag handle of the object
---@param parentObjectHandle? EngineObjectHandle|integer @The handle of the parent object
---@param position EnginePoint3D @The position of the object
---@return EngineObjectHandle @The handle of the object
function Engine.gameState.createObject(tagHandle, parentObjectHandle, position)
end

-- Delete the object
---@param objectHandle EngineObjectHandle|integer @The handle of the object
function Engine.gameState.deleteObject(objectHandle)
end

-- Make a unit enter a vehicle
---@param unitObjectHandle EngineObjectHandle|integer @The handle of the unit
---@param vehicleObjectHandle EngineObjectHandle|integer @The handle of the vehicle
---@param seatIndex integer @The index of the seat
function Engine.gameState.unitEnterVehicle(unitObjectHandle,
                                           vehicleObjectHandle,
                                           seatIndex)
end

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
---@param playerIndexOrHandle? EnginePlayerHandle|integer @The index or the handle of the player; If nil, the local player is returned
---@return MetaEnginePlayer @The player
function Engine.gameState.getPlayer(playerIndexOrHandle)
end

-- Get a player by their RCON handle
---@param playerRconHandle integer @The RCON handle of the player
---@return MetaEnginePlayer @The player
function Engine.gameState.getPlayerByRconHandle(playerRconHandle)
end

---@alias EngineCameraType
---| 'first_person'
---| 'vehicle'
---| 'cinematic'
---| 'debug'
---| 'unknown'

-- Get the camera type
---@return EngineCameraType
function Engine.core.getCameraType()
end

---@class EngineCameraData
---@field position EnginePoint3D
---@field orientation table<integer, EnginePoint3D>
---@field fov number

-- Get the camera data
---@return EngineCameraData
function Engine.core.getCameraData()
end

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
function Engine.map.getCurrentMapHeader()
end

-- Get the map list
---@return table<integer, string> @The map list
function Engine.map.getMapList()
end

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
function Engine.netgame.getServerType()
end

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
function Engine.netgame.getServerGametype()
end

-- Get if the current game is a team game
---@return boolean @If the current game is a team game
function Engine.netgame.currentGameIsTeam()
end

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
function Engine.tag.getTagDataHeader()
end

---@alias EngineTagClass
---| EngineTagClassesActor
---| EngineTagClassesActorVariant
---| EngineTagClassesAntenna
---| EngineTagClassesModelAnimations
---| EngineTagClassesBiped
---| EngineTagClassesBitmap
---| EngineTagClassesSpheroid
---| EngineTagClassesContinuousDamageEffect
---| EngineTagClassesModelCollisionGeometry
---| EngineTagClassesColorTable
---| EngineTagClassesContrail
---| EngineTagClassesDeviceControl
---| EngineTagClassesDecal
---| EngineTagClassesUiWidgetDefinition
---| EngineTagClassesInputDeviceDefaults
---| EngineTagClassesDevice
---| EngineTagClassesDetailObjectCollection
---| EngineTagClassesEffect
---| EngineTagClassesEquipment
---| EngineTagClassesFlag
---| EngineTagClassesFog
---| EngineTagClassesFont
---| EngineTagClassesMaterialEffects
---| EngineTagClassesGarbage
---| EngineTagClassesGlow
---| EngineTagClassesGrenadeHudInterface
---| EngineTagClassesHudMessageText
---| EngineTagClassesHudNumber
---| EngineTagClassesHudGlobals
---| EngineTagClassesItem
---| EngineTagClassesItemCollection
---| EngineTagClassesDamageEffect
---| EngineTagClassesLensFlare
---| EngineTagClassesLightning
---| EngineTagClassesDeviceLightFixture
---| EngineTagClassesLight
---| EngineTagClassesSoundLooping
---| EngineTagClassesDeviceMachine
---| EngineTagClassesGlobals
---| EngineTagClassesMeter
---| EngineTagClassesLightVolume
---| EngineTagClassesGbxmodel
---| EngineTagClassesModel
---| EngineTagClassesMultiplayerScenarioDescription
---| EngineTagClassesNull
---| EngineTagClassesPreferencesNetworkGame
---| EngineTagClassesObject
---| EngineTagClassesParticle
---| EngineTagClassesParticleSystem
---| EngineTagClassesPhysics
---| EngineTagClassesPlaceholder
---| EngineTagClassesPointPhysics
---| EngineTagClassesProjectile
---| EngineTagClassesWeatherParticleSystem
---| EngineTagClassesScenarioStructureBsp
---| EngineTagClassesScenery
---| EngineTagClassesShaderTransparentChicagoExtended
---| EngineTagClassesShaderTransparentChicago
---| EngineTagClassesScenario
---| EngineTagClassesShaderEnvironment
---| EngineTagClassesShaderTransparentGlass
---| EngineTagClassesShader
---| EngineTagClassesSky
---| EngineTagClassesShaderTransparentMeter
---| EngineTagClassesSound
---| EngineTagClassesSoundEnvironment
---| EngineTagClassesShaderModel
---| EngineTagClassesShaderTransparentGeneric
---| EngineTagClassesUiWidgetCollection
---| EngineTagClassesShaderTransparentPlasma
---| EngineTagClassesSoundScenery
---| EngineTagClassesStringList
---| EngineTagClassesShaderTransparentWater
---| EngineTagClassesTagCollection
---| EngineTagClassesCameraTrack
---| EngineTagClassesDialogue
---| EngineTagClassesUnitHudInterface
---| EngineTagClassesUnit
---| EngineTagClassesUnicodeStringList
---| EngineTagClassesVirtualKeyboard
---| EngineTagClassesVehicle
---| EngineTagClassesWeapon
---| EngineTagClassesWind
---| EngineTagClassesWeaponHudInterface

---@class EngineTag
---@field primaryClass EngineTagClass
---@field secondaryClass EngineTagClass
---@field tertiaryClass EngineTagClass
---@field handle EngineTagHandle
---@field path string
---@field dataAddress integer
---@field indexed boolean

---@class MetaEngineTag: EngineTag

---@class MetaEngineActorTag: MetaEngineTag
---@field data MetaEngineTagDataActor

---@class MetaEngineActorVariantTag: MetaEngineTag
---@field data MetaEngineTagDataActorVariant

---@class MetaEngineAntennaTag: MetaEngineTag
---@field data MetaEngineTagDataAntenna

---@class MetaEngineModelAnimationsTag: MetaEngineTag
---@field data MetaEngineTagDataModelAnimations

---@class MetaEngineBipedTag: MetaEngineTag
---@field data MetaEngineTagDataBiped

---@class MetaEngineBitmapTag: MetaEngineTag
---@field data MetaEngineTagDataBitmap

---@class MetaEngineContinuousDamageEffectTag: MetaEngineTag
---@field data MetaEngineTagDataContinuousDamageEffect

---@class MetaEngineModelCollisionGeometryTag: MetaEngineTag
---@field data MetaEngineTagDataModelCollisionGeometry

---@class MetaEngineColorTableTag: MetaEngineTag
---@field data MetaEngineTagDataColorTable

---@class MetaEngineContrailTag: MetaEngineTag
---@field data MetaEngineTagDataContrail

---@class MetaEngineDeviceControlTag: MetaEngineTag
---@field data MetaEngineTagDataDeviceControl

---@class MetaEngineDecalTag: MetaEngineTag
---@field data MetaEngineTagDataDecal

---@class MetaEngineUiWidgetDefinitionTag: MetaEngineTag
---@field data MetaEngineTagDataUiWidgetDefinition

---@class MetaEngineInputDeviceDefaultsTag: MetaEngineTag
---@field data MetaEngineTagDataInputDeviceDefaults

---@class MetaEngineDeviceTag: MetaEngineTag
---@field data MetaEngineTagDataDevice

---@class MetaEngineDetailObjectCollectionTag: MetaEngineTag
---@field data MetaEngineTagDataDetailObjectCollection

---@class MetaEngineEffectTag: MetaEngineTag
---@field data MetaEngineTagDataEffect

---@class MetaEngineEquipmentTag: MetaEngineTag
---@field data MetaEngineTagDataEquipment

---@class MetaEngineFlagTag: MetaEngineTag
---@field data MetaEngineTagDataFlag

---@class MetaEngineFogTag: MetaEngineTag
---@field data MetaEngineTagDataFog

---@class MetaEngineFontTag: MetaEngineTag
---@field data MetaEngineTagDataFont

---@class MetaEngineLightningTag: MetaEngineTag
---@field data MetaEngineTagDataLightning

---@class MetaEngineMaterialEffectsTag: MetaEngineTag
---@field data MetaEngineTagDataMaterialEffects

---@class MetaEngineGarbageTag: MetaEngineTag
---@field data MetaEngineTagDataGarbage

---@class MetaEngineGlowTag: MetaEngineTag
---@field data MetaEngineTagDataGlow

---@class MetaEngineGrenadeHudInterfaceTag: MetaEngineTag
---@field data MetaEngineTagDataGrenadeHudInterface

---@class MetaEngineHudMessageTextTag: MetaEngineTag
---@field data MetaEngineTagDataHudMessageText

---@class MetaEngineHudNumberTag: MetaEngineTag
---@field data MetaEngineTagDataHudNumber

---@class MetaEngineHudGlobalsTag: MetaEngineTag
---@field data MetaEngineTagDataHudGlobals

---@class MetaEngineItemTag: MetaEngineTag
---@field data MetaEngineTagDataItem

---@class MetaEngineItemCollectionTag: MetaEngineTag
---@field data MetaEngineTagDataItemCollection

---@class MetaEngineDamageEffectTag: MetaEngineTag
---@field data MetaEngineTagDataDamageEffect

---@class MetaEngineLensFlareTag: MetaEngineTag
---@field data MetaEngineTagDataLensFlare

---@class MetaEngineDeviceLightFixtureTag: MetaEngineTag
---@field data MetaEngineTagDataDeviceLightFixture

---@class MetaEngineLightTag: MetaEngineTag
---@field data MetaEngineTagDataLight

---@class MetaEngineSoundLoopingTag: MetaEngineTag
---@field data MetaEngineTagDataSoundLooping

---@class MetaEngineDeviceMachineTag: MetaEngineTag
---@field data MetaEngineTagDataDeviceMachine

---@class MetaEngineGlobalsTag: MetaEngineTag
---@field data MetaEngineTagDataGlobals

---@class MetaEngineMeterTag: MetaEngineTag
---@field data MetaEngineTagDataMeter

---@class MetaEngineLightVolumeTag: MetaEngineTag
---@field data MetaEngineTagDataLightVolume

---@class MetaEngineGbxmodelTag: MetaEngineTag
---@field data MetaEngineTagDataGbxmodel

---@class MetaEngineModelTag: MetaEngineTag
---@field data MetaEngineTagDataModel

---@class MetaEngineMultiplayerScenarioDescriptionTag: MetaEngineTag
---@field data MetaEngineTagDataMultiplayerScenarioDescription

---@class MetaEnginePreferencesNetworkGameTag: MetaEngineTag
---@field data MetaEngineTagDataPreferencesNetworkGame

---@class MetaEngineObjectTag: MetaEngineTag
---@field data MetaEngineTagDataObject

---@class MetaEngineParticleTag: MetaEngineTag
---@field data MetaEngineTagDataParticle

---@class MetaEngineParticleSystemTag: MetaEngineTag
---@field data MetaEngineTagDataParticleSystem

---@class MetaEnginePhysicsTag: MetaEngineTag
---@field data MetaEngineTagDataPhysics

---@class MetaEnginePlaceholderTag: MetaEngineTag
---@field data MetaEngineTagDataPlaceholder

---@class MetaEnginePointPhysicsTag: MetaEngineTag
---@field data MetaEngineTagDataPointPhysics

---@class MetaEngineProjectileTag: MetaEngineTag
---@field data MetaEngineTagDataProjectile

---@class MetaEngineWeatherParticleSystemTag: MetaEngineTag
---@field data MetaEngineTagDataWeatherParticleSystem

---@class MetaEngineScenarioStructureBspTag: MetaEngineTag
---@field data MetaEngineTagDataScenarioStructureBsp

---@class MetaEngineSceneryTag: MetaEngineTag
---@field data MetaEngineTagDataScenery

---@class MetaEngineShaderTransparentChicagoExtendedTag: MetaEngineTag
---@field data MetaEngineTagDataShaderTransparentChicagoExtended

---@class MetaEngineShaderTransparentChicagoTag: MetaEngineTag
---@field data MetaEngineTagDataShaderTransparentChicago

---@class MetaEngineScenarioTag: MetaEngineTag
---@field data MetaEngineTagDataScenario

---@class MetaEngineShaderEnvironmentTag: MetaEngineTag
---@field data MetaEngineTagDataShaderEnvironment

---@class MetaEngineShaderTransparentGlassTag: MetaEngineTag
---@field data MetaEngineTagDataShaderTransparentGlass

---@class MetaEngineShaderTag: MetaEngineTag
---@field data MetaEngineTagDataShader

---@class MetaEngineSkyTag: MetaEngineTag
---@field data MetaEngineTagDataSky

---@class MetaEngineShaderTransparentMeterTag: MetaEngineTag
---@field data MetaEngineTagDataShaderTransparentMeter

---@class MetaEngineSoundTag: MetaEngineTag
---@field data MetaEngineTagDataSound

---@class MetaEngineSoundEnvironmentTag: MetaEngineTag
---@field data MetaEngineTagDataSoundEnvironment

---@class MetaEngineShaderModelTag: MetaEngineTag
---@field data MetaEngineTagDataShaderModel

---@class MetaEngineShaderTransparentGenericTag: MetaEngineTag
---@field data MetaEngineTagDataShaderTransparentGeneric

---@class MetaEngineTagDataUiWidgetCollection: MetaEngineTagDataTagCollection

---@class MetaEngineUiWidgetCollectionTag: MetaEngineTag
---@field data MetaEngineTagDataUiWidgetCollection

---@class MetaEngineShaderTransparentPlasmaTag: MetaEngineTag
---@field data MetaEngineTagDataShaderTransparentPlasma

---@class MetaEngineSoundSceneryTag: MetaEngineTag
---@field data MetaEngineTagDataSoundScenery

---@class MetaEngineStringListTag: MetaEngineTag
---@field data MetaEngineTagDataStringList

---@class MetaEngineShaderTransparentWaterTag: MetaEngineTag
---@field data MetaEngineTagDataShaderTransparentWater

---@class MetaEngineTagCollectionTag: MetaEngineTag
---@field data MetaEngineTagDataTagCollection

---@class MetaEngineCameraTrackTag: MetaEngineTag
---@field data MetaEngineTagDataCameraTrack

---@class MetaEngineUnitHudInterfaceTag: MetaEngineTag
---@field data MetaEngineTagDataUnitHudInterface

---@class MetaEngineUnitTag: MetaEngineTag
---@field data MetaEngineTagDataUnit

---@class MetaEngineUnicodeStringListTag: MetaEngineTag
---@field data MetaEngineTagDataUnicodeStringList

---@class MetaEngineVirtualKeyboardTag: MetaEngineTag
---@field data MetaEngineTagDataVirtualKeyboard

---@class MetaEngineVehicleTag: MetaEngineTag
---@field data MetaEngineTagDataVehicle

---@class MetaEngineWeaponTag: MetaEngineTag
---@field data MetaEngineTagDataWeapon

---@class MetaEngineWindTag: MetaEngineTag
---@field data MetaEngineTagDataWind

---@class MetaEngineWeaponHudInterfaceTag: MetaEngineTag
---@field data MetaEngineTagDataWeaponHudInterface

---@class EngineTagClassesEnum
---@field tostring fun(enum: EngineTagClassesEnum): string
---@field tointeger fun(enum: EngineTagClassesEnum): integer

---@class EngineTagClassesActor : EngineTagClassesEnum
---@class EngineTagClassesActorVariant : EngineTagClassesEnum
---@class EngineTagClassesAntenna : EngineTagClassesEnum
---@class EngineTagClassesModelAnimations : EngineTagClassesEnum
---@class EngineTagClassesBiped : EngineTagClassesEnum
---@class EngineTagClassesBitmap : EngineTagClassesEnum
---@class EngineTagClassesSpheroid : EngineTagClassesEnum
---@class EngineTagClassesContinuousDamageEffect : EngineTagClassesEnum
---@class EngineTagClassesModelCollisionGeometry : EngineTagClassesEnum
---@class EngineTagClassesColorTable : EngineTagClassesEnum
---@class EngineTagClassesContrail : EngineTagClassesEnum
---@class EngineTagClassesDeviceControl : EngineTagClassesEnum
---@class EngineTagClassesDecal : EngineTagClassesEnum
---@class EngineTagClassesUiWidgetDefinition : EngineTagClassesEnum
---@class EngineTagClassesInputDeviceDefaults : EngineTagClassesEnum
---@class EngineTagClassesDevice : EngineTagClassesEnum
---@class EngineTagClassesDetailObjectCollection : EngineTagClassesEnum
---@class EngineTagClassesEffect : EngineTagClassesEnum
---@class EngineTagClassesEquipment : EngineTagClassesEnum
---@class EngineTagClassesFlag : EngineTagClassesEnum
---@class EngineTagClassesFog : EngineTagClassesEnum
---@class EngineTagClassesFont : EngineTagClassesEnum
---@class EngineTagClassesMaterialEffects : EngineTagClassesEnum
---@class EngineTagClassesGarbage : EngineTagClassesEnum
---@class EngineTagClassesGlow : EngineTagClassesEnum
---@class EngineTagClassesGrenadeHudInterface : EngineTagClassesEnum
---@class EngineTagClassesHudMessageText : EngineTagClassesEnum
---@class EngineTagClassesHudNumber : EngineTagClassesEnum
---@class EngineTagClassesHudGlobals : EngineTagClassesEnum
---@class EngineTagClassesItem : EngineTagClassesEnum
---@class EngineTagClassesItemCollection : EngineTagClassesEnum
---@class EngineTagClassesDamageEffect : EngineTagClassesEnum
---@class EngineTagClassesLensFlare : EngineTagClassesEnum
---@class EngineTagClassesLightning : EngineTagClassesEnum
---@class EngineTagClassesDeviceLightFixture : EngineTagClassesEnum
---@class EngineTagClassesLight : EngineTagClassesEnum
---@class EngineTagClassesSoundLooping : EngineTagClassesEnum
---@class EngineTagClassesDeviceMachine : EngineTagClassesEnum
---@class EngineTagClassesGlobals : EngineTagClassesEnum
---@class EngineTagClassesMeter : EngineTagClassesEnum
---@class EngineTagClassesLightVolume : EngineTagClassesEnum
---@class EngineTagClassesGbxmodel : EngineTagClassesEnum
---@class EngineTagClassesModel : EngineTagClassesEnum
---@class EngineTagClassesMultiplayerScenarioDescription : EngineTagClassesEnum
---@class EngineTagClassesNull : EngineTagClassesEnum
---@class EngineTagClassesPreferencesNetworkGame : EngineTagClassesEnum
---@class EngineTagClassesObject : EngineTagClassesEnum
---@class EngineTagClassesParticle : EngineTagClassesEnum
---@class EngineTagClassesParticleSystem : EngineTagClassesEnum
---@class EngineTagClassesPhysics : EngineTagClassesEnum
---@class EngineTagClassesPlaceholder : EngineTagClassesEnum
---@class EngineTagClassesPointPhysics : EngineTagClassesEnum
---@class EngineTagClassesProjectile : EngineTagClassesEnum
---@class EngineTagClassesWeatherParticleSystem : EngineTagClassesEnum
---@class EngineTagClassesScenarioStructureBsp : EngineTagClassesEnum
---@class EngineTagClassesScenery : EngineTagClassesEnum
---@class EngineTagClassesShaderTransparentChicagoExtended : EngineTagClassesEnum
---@class EngineTagClassesShaderTransparentChicago : EngineTagClassesEnum
---@class EngineTagClassesScenario : EngineTagClassesEnum
---@class EngineTagClassesShaderEnvironment : EngineTagClassesEnum
---@class EngineTagClassesShaderTransparentGlass : EngineTagClassesEnum
---@class EngineTagClassesShader : EngineTagClassesEnum
---@class EngineTagClassesSky : EngineTagClassesEnum
---@class EngineTagClassesShaderTransparentMeter : EngineTagClassesEnum
---@class EngineTagClassesSound : EngineTagClassesEnum
---@class EngineTagClassesSoundEnvironment : EngineTagClassesEnum
---@class EngineTagClassesShaderModel : EngineTagClassesEnum
---@class EngineTagClassesShaderTransparentGeneric : EngineTagClassesEnum
---@class EngineTagClassesUiWidgetCollection : EngineTagClassesEnum
---@class EngineTagClassesShaderTransparentPlasma : EngineTagClassesEnum
---@class EngineTagClassesSoundScenery : EngineTagClassesEnum
---@class EngineTagClassesStringList : EngineTagClassesEnum
---@class EngineTagClassesShaderTransparentWater : EngineTagClassesEnum
---@class EngineTagClassesTagCollection : EngineTagClassesEnum
---@class EngineTagClassesCameraTrack : EngineTagClassesEnum
---@class EngineTagClassesDialogue : EngineTagClassesEnum
---@class EngineTagClassesUnitHudInterface : EngineTagClassesEnum
---@class EngineTagClassesUnit : EngineTagClassesEnum
---@class EngineTagClassesUnicodeStringList : EngineTagClassesEnum
---@class EngineTagClassesVirtualKeyboard : EngineTagClassesEnum
---@class EngineTagClassesVehicle : EngineTagClassesEnum
---@class EngineTagClassesWeapon : EngineTagClassesEnum
---@class EngineTagClassesWind : EngineTagClassesEnum
---@class EngineTagClassesWeaponHudInterface : EngineTagClassesEnum

---@class EngineTagClasses
---@field actor EngineTagClassesActor
---@field actorVariant EngineTagClassesActorVariant
---@field antenna EngineTagClassesAntenna
---@field modelAnimations EngineTagClassesModelAnimations
---@field biped EngineTagClassesBiped
---@field bitmap EngineTagClassesBitmap
---@field spheroid EngineTagClassesSpheroid
---@field continuousDamageEffect EngineTagClassesContinuousDamageEffect
---@field modelCollisionGeometry EngineTagClassesModelCollisionGeometry
---@field colorTable EngineTagClassesColorTable
---@field contrail EngineTagClassesContrail
---@field deviceControl EngineTagClassesDeviceControl
---@field decal EngineTagClassesDecal
---@field uiWidgetDefinition EngineTagClassesUiWidgetDefinition
---@field inputDeviceDefaults EngineTagClassesInputDeviceDefaults
---@field device EngineTagClassesDevice
---@field detailObjectCollection EngineTagClassesDetailObjectCollection
---@field effect EngineTagClassesEffect
---@field equipment EngineTagClassesEquipment
---@field flag EngineTagClassesFlag
---@field fog EngineTagClassesFog
---@field font EngineTagClassesFont
---@field materialEffects EngineTagClassesMaterialEffects
---@field garbage EngineTagClassesGarbage
---@field glow EngineTagClassesGlow
---@field grenadeHudInterface EngineTagClassesGrenadeHudInterface
---@field hudMessageText EngineTagClassesHudMessageText
---@field hudNumber EngineTagClassesHudNumber
---@field hudGlobals EngineTagClassesHudGlobals
---@field item EngineTagClassesItem
---@field itemCollection EngineTagClassesItemCollection
---@field damageEffect EngineTagClassesDamageEffect
---@field lensFlare EngineTagClassesLensFlare
---@field lightning EngineTagClassesLightning
---@field deviceLightFixture EngineTagClassesDeviceLightFixture
---@field light EngineTagClassesLight
---@field soundLooping EngineTagClassesSoundLooping
---@field deviceMachine EngineTagClassesDeviceMachine
---@field globals EngineTagClassesGlobals
---@field meter EngineTagClassesMeter
---@field lightVolume EngineTagClassesLightVolume
---@field gbxmodel EngineTagClassesGbxmodel
---@field model EngineTagClassesModel
---@field multiplayerScenarioDescription EngineTagClassesMultiplayerScenarioDescription
---@field null EngineTagClassesNull
---@field preferencesNetworkGame EngineTagClassesPreferencesNetworkGame
---@field object EngineTagClassesObject
---@field particle EngineTagClassesParticle
---@field particleSystem EngineTagClassesParticleSystem
---@field physics EngineTagClassesPhysics
---@field placeholder EngineTagClassesPlaceholder
---@field pointPhysics EngineTagClassesPointPhysics
---@field projectile EngineTagClassesProjectile
---@field weatherParticleSystem EngineTagClassesWeatherParticleSystem
---@field scenarioStructureBsp EngineTagClassesScenarioStructureBsp
---@field scenery EngineTagClassesScenery
---@field shaderTransparentChicagoExtended EngineTagClassesShaderTransparentChicagoExtended
---@field shaderTransparentChicago EngineTagClassesShaderTransparentChicago
---@field scenario EngineTagClassesScenario
---@field shaderEnvironment EngineTagClassesShaderEnvironment
---@field shaderTransparentGlass EngineTagClassesShaderTransparentGlass
---@field shader EngineTagClassesShader
---@field sky EngineTagClassesSky
---@field shaderTransparentMeter EngineTagClassesShaderTransparentMeter
---@field sound EngineTagClassesSound
---@field soundEnvironment EngineTagClassesSoundEnvironment
---@field shaderModel EngineTagClassesShaderModel
---@field shaderTransparentGeneric EngineTagClassesShaderTransparentGeneric
---@field uiWidgetCollection EngineTagClassesUiWidgetCollection
---@field shaderTransparentPlasma EngineTagClassesShaderTransparentPlasma
---@field soundScenery EngineTagClassesSoundScenery
---@field stringList EngineTagClassesStringList
---@field shaderTransparentWater EngineTagClassesShaderTransparentWater
---@field tagCollection EngineTagClassesTagCollection
---@field cameraTrack EngineTagClassesCameraTrack
---@field dialogue EngineTagClassesDialogue
---@field unitHudInterface EngineTagClassesUnitHudInterface
---@field unit EngineTagClassesUnit
---@field unicodeStringList EngineTagClassesUnicodeStringList
---@field virtualKeyboard EngineTagClassesVirtualKeyboard
---@field vehicle EngineTagClassesVehicle
---@field weapon EngineTagClassesWeapon
---@field wind EngineTagClassesWind
---@field weaponHudInterface EngineTagClassesWeaponHudInterface

---@diagnostic disable-next-line: missing-fields
Engine.tag.classes = {} --[[@as EngineTagClasses]]

-- Get a tag
---@param tagHandleOrPath EngineTagHandle|integer @The handle or path of the tag
---@param tagClass? EngineTagClasses @The class of the tag; If first argument is a handle, this argument can be skipped
---@return MetaEngineTag|nil @The tag; nil if not found
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesActor): MetaEngineActorTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesActorVariant): MetaEngineActorVariantTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesAntenna): MetaEngineAntennaTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesModelAnimations): MetaEngineModelAnimationsTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesBiped): MetaEngineBipedTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesBitmap): MetaEngineBitmapTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesContinuousDamageEffect): MetaEngineContinuousDamageEffectTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesModelCollisionGeometry): MetaEngineModelCollisionGeometryTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesColorTable): MetaEngineColorTableTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesContrail): MetaEngineContrailTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesDeviceControl): MetaEngineDeviceControlTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesDecal): MetaEngineDecalTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesUiWidgetDefinition): MetaEngineUiWidgetDefinitionTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesInputDeviceDefaults): MetaEngineInputDeviceDefaultsTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesDevice): MetaEngineDeviceTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesDetailObjectCollection): MetaEngineDetailObjectCollectionTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesEffect): MetaEngineEffectTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesEquipment): MetaEngineEquipmentTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesFlag): MetaEngineFlagTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesFog): MetaEngineFogTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesFont): MetaEngineFontTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesLightning): MetaEngineLightningTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesMaterialEffects): MetaEngineMaterialEffectsTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesGarbage): MetaEngineGarbageTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesGlow): MetaEngineGlowTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesGrenadeHudInterface): MetaEngineGrenadeHudInterfaceTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesHudMessageText): MetaEngineHudMessageTextTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesHudNumber): MetaEngineHudNumberTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesHudGlobals): MetaEngineHudGlobalsTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesItem): MetaEngineItemTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesItemCollection): MetaEngineItemCollectionTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesDamageEffect): MetaEngineDamageEffectTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesLensFlare): MetaEngineLensFlareTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesDeviceLightFixture): MetaEngineDeviceLightFixtureTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesLight): MetaEngineLightTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesSoundLooping): MetaEngineSoundLoopingTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesDeviceMachine): MetaEngineDeviceMachineTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesGlobals): MetaEngineGlobalsTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesMeter): MetaEngineMeterTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesLightVolume): MetaEngineLightVolumeTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesGbxmodel): MetaEngineGbxmodelTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesModel): MetaEngineModelTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesMultiplayerScenarioDescription): MetaEngineMultiplayerScenarioDescriptionTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesPreferencesNetworkGame): MetaEnginePreferencesNetworkGameTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesObject): MetaEngineObjectTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesParticle): MetaEngineParticleTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesParticleSystem): MetaEngineParticleSystemTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesPhysics): MetaEnginePhysicsTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesPlaceholder): MetaEnginePlaceholderTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesPointPhysics): MetaEnginePointPhysicsTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesProjectile): MetaEngineProjectileTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesWeatherParticleSystem): MetaEngineWeatherParticleSystemTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesScenarioStructureBsp): MetaEngineScenarioStructureBspTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesScenery): MetaEngineSceneryTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesShaderTransparentChicagoExtended): MetaEngineShaderTransparentChicagoExtendedTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesShaderTransparentChicago): MetaEngineShaderTransparentChicagoTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesScenario): MetaEngineScenarioTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesShaderEnvironment): MetaEngineShaderEnvironmentTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesShaderTransparentGlass): MetaEngineShaderTransparentGlassTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesShader): MetaEngineShaderTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesSky): MetaEngineSkyTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesShaderTransparentMeter): MetaEngineShaderTransparentMeterTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesSound): MetaEngineSoundTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesSoundEnvironment): MetaEngineSoundEnvironmentTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesShaderModel): MetaEngineShaderModelTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesShaderTransparentGeneric): MetaEngineShaderTransparentGenericTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesUiWidgetCollection): MetaEngineUiWidgetCollectionTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesShaderTransparentPlasma): MetaEngineShaderTransparentPlasmaTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesSoundScenery): MetaEngineSoundSceneryTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesStringList): MetaEngineStringListTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesShaderTransparentWater): MetaEngineShaderTransparentWaterTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesTagCollection): MetaEngineTagCollectionTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesCameraTrack): MetaEngineCameraTrackTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesUnitHudInterface): MetaEngineUnitHudInterfaceTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesUnit): MetaEngineUnitTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesUnicodeStringList): MetaEngineUnicodeStringListTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesVirtualKeyboard): MetaEngineVirtualKeyboardTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesVehicle): MetaEngineVehicleTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesWeapon): MetaEngineWeaponTag |nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesWind): MetaEngineWindTag|nil
---@overload fun(tagHandleOrPath: string|integer|EngineTagHandle, tagClass: EngineTagClassesWeaponHudInterface): MetaEngineWeaponHudInterfaceTag|nil
function Engine.tag.getTag(tagHandleOrPath, tagClass)
end

---Return a tag entry list given a keyword and/or a tag class
---
---Leave the keyword empty to get all tags, if tag class is nil, it will return all tags
---@param keyword string @Keyword to search for
---@param tagClass? EngineTagClasses Class of the tag
function Engine.tag.findTags(keyword, tagClass)
end
-------------------------------------------------------
-- Engine.userInterface
-------------------------------------------------------

Engine.userInterface = {}

---@class EngineWidget
---@field definitionTagHandle EngineTagHandle
---@field name string
---@field controllerIndex boolean
---@field position EnginePoint2DInt
---@field type EngineTagDataUIWidgetType
---@field visible boolean
---@field renderRegardlessOfControllerIndex boolean
---@field pausesGameTime boolean
---@field deleted boolean
---@field creationProcessStartTime integer
---@field msToClose integer
---@field msToCloseFadeTime integer
---@field opacity number
---@field previousWidget MetaEngineWidget|nil
---@field nextWidget MetaEngineWidget|nil
---@field parentWidget MetaEngineWidget|nil
---@field childWidget MetaEngineWidget|nil
---@field focusedChild MetaEngineWidget|nil
---@field textAddress integer @The address of the text; nil if the widget is not a text widget, be careful!
---@field cursorIndex integer @Index of the last child widget focused by the mouse
---@field extendedDescriptionWidget EngineWidget
---@field bitmapIndex integer

---@class MetaEngineWidget: EngineWidget

---@alias EngineWidgetNavigationSound
---| 'cursor'
---| 'forward'
---| 'back'
---| 'flag_failure'

-- Find a widget from a given widget definition; it only returns the first coincidence
---@param widgetDefinition integer|string|EngineTagHandle @The handle or path of the widget definition
---@param baseWidget? MetaEngineWidget @The base widget to start the search from; If nil, the search will start from the root widget
---@return MetaEngineWidget|nil @Found widget; nil if not found
function Engine.userInterface.findWidget(widgetDefinition, baseWidget)
end

-- Find all widgets from a given widget definition
---@param widgetDefinition integer|string|EngineTagHandle @The handle or path of the widget definition
---@param baseWidget? MetaEngineWidget @The base widget to start the search from; If nil, the search will start from the root widget
---@return table<integer, MetaEngineWidget>|nil @Found widgets; nil if any widget was found
function Engine.userInterface.findWidgets(widgetDefinition, baseWidget)
end

-- Open a widget
---@param widgetDefinition integer|string|EngineTagHandle @The handle or path of the widget definition
---@param pushHistory? boolean @If the widget should be pushed to the history; false by default
---@return MetaEngineWidget|nil @Created widget; nil if failed
function Engine.userInterface.openWidget(widgetDefinition, pushHistory)
end

-- Close current root widget; return to the previous one in history
function Engine.userInterface.closeWidget()
end

-- Replace a widget
---@param targetWidget MetaEngineWidget @The widget to replace
---@param widgetDefinition integer|string|EngineTagHandle @The handle or path of the widget definition
---@return MetaEngineWidget|nil @Created widget; nil if failed
function Engine.userInterface.replaceWidget(targetWidget, widgetDefinition)
end

-- Reload a widget; replaces the widget with a new one with the same definition and state 
---@param targetWidget MetaEngineWidget @The widget to reload
---@return MetaEngineWidget|nil @New widget; nil if failed
function Engine.userInterface.reloadWidget(targetWidget)
end

-- Focus a widget
---@param targetWidget MetaEngineWidget @The widget to focus
function Engine.userInterface.focusWidget(targetWidget)
end

-- Get the current root widget
---@return MetaEngineWidget @The current root widget
function Engine.userInterface.getRootWidget()
end

-- Open the pause menu
function Engine.userInterface.openPauseMenu()
end

-- Gets the HUD globals tag data
---@return MetaEngineTagDataHudGlobals @The HUD globals tag data of the current map
function Engine.userInterface.getHudGlobals()
end

-- Gets the size of a sprite in a bitmap
---@param bitmap MetaEngineTagDataBitmap @Bitmap data
---@param sequenceIndex integer @A valid sequence index of the bitmap
---@param spriteIndex integer @A valid sprite index of the bitmap
---@return EngineResolution|nil @The size of the sprite; nil if failed
function Engine.userInterface.getBitmapSpriteResolution(bitmap, sequenceIndex, spriteIndex)
end

-- Draws a icon bitmap on a HUD message.
---@param bitmap MetaEngineTagDataBitmap @Bitmap data
---@param sequenceIndex integer @A valid sequence index of the bitmap
---@param spriteIndex integer @A valid sprite index of the bitmap
---@param position EnginePoint2D @The position of the sprite
---@param color EngineColorARGBInt @The color of the sprite
function Engine.userInterface.drawHudMessageSprite(bitmap,
                                                   sequenceIndex,
                                                   spriteIndex,
                                                   position,
                                                   color)
end

-- Play a sound from a given tag
---@param soundTagHandleOrPath integer|string|EngineTagHandle @The handle or path of the sound tag
function Engine.userInterface.playSound(soundTagHandleOrPath)
end

-- Get the duration of a sound permutation
---@param soundPermutation MetaEngineTagDataSoundPermutation @The sound permutation
function Engine.userInterface.getSoundPermutationSamplesDuration(soundPermutation)
end

-------------------------------------------------------
-- Engine.renderer
-------------------------------------------------------

---@class EngineResolution
---@field height integer
---@field width integer

-- Get the current resolution of the game
---@return EngineResolution
function Engine.core.getResolution()
end
