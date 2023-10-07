-- SPDX-License-Identifier: GPL-3.0-only
-- This file is used to document the Lua plugins engine API. It should not be included.

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

---@class EngineColorRGB
---@field red number
---@field green number
---@field blue number

---@class MetaEngineColorRGB: EngineColorRGB

---@class EnginePoint2D
---@field x number
---@field y number

---@class MetaEnginePoint2D: EnginePoint2D

---@class EnginePoint3D
---@field x number
---@field y number
---@field z number

---@class MetaEnginePoint3D: EnginePoint3D

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

---@class EngineRectangle2DF
---@field top number
---@field left number
---@field bottom number
---@field right number

---@class MetaEngineRectangle2DF: EngineRectangle2DF

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

---@class Resolution
---@field height integer
---@field width integer

-- Get the current resolution of the game
---@return Resolution
function Engine.core.getResolution() end

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

---@alias CameraType
---| 'first_person'
---| 'vehicle'
---| 'cinematic'
---| 'debug'
---| 'unknown'

-- Get the camera type
---@return CameraType
function Engine.core.getCameraType() end

---@class CameraData
---@field position EnginePoint3D
---@field orientation table<EnginePoint3D>
---@field fov number

-- Get the camera data
---@return CameraData
function Engine.core.getCameraData() end

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
-- Engine.multiplayer
-------------------------------------------------------

Engine.multiplayer = {}

---@alias EngineServerType
---| 'none'
---| 'dedicated'
---| 'local'
---| 'unknown'

-- Get the server type
---@return EngineServerType
function Engine.multiplayer.getServerType() end

---@alias EngineGameType
---| 'ctf'
---| 'slayer'
---| 'oddball'
---| 'king'
---| 'race'
---| 'none'

-- Get the server gametype
---@return EngineGameType @The server gametype
function Engine.multiplayer.getServerGametype() end

-- Get if the current game is a team game
---@return boolean @If the current game is a team game
function Engine.multiplayer.currentGameIsTeam() end

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

---@class Tag
---@field primaryClass EngineTagClass
---@field secondaryClass integer
---@field tertiaryClass integer
---@field handle EngineTagHandle
---@field path string
---@field dataAddress integer
---@field indexed boolean

-- Get a tag
---@param tagHandle integer @The handle of the tag
---@return Tag @The tag
---@overload fun(tagPath: string, tagClass: EngineTagClass): Tag
function Engine.tag.getTag(tagHandle) end

-- Get a tag
---@param tagHandle integer @The handle of the tag
---@overload fun(tagPath: string, tagClass: EngineTagClass): Tag
function Engine.tag.getTagData(tagHandle) end
