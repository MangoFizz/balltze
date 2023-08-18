# Balltze - Lua API

Balltze provides a Lua API for the game engine. This API is used to create and modify the game world
and to interact with the game engine. The API is split into several functions each of which is
described below.

Version: 0.1.2

**NOTE:** All the functions listed below are available in the global `balltze` table. For example,
the `create` function can be accessed as `balltze.create_object`.


- [Balltze - Lua API](#balltze---lua-api)
  - [Functions](#functions)
    - [create\_dynamic\_object](#create_object)
    - [unit\_enter\_vehicle](#unit_enter_vehicle)


## Functions

### create_object

Creates a new object from the specified tag.
The tag can be specified as a tag handle. The tag handle must be a handle to a tag that is
currently loaded in the game.

Usage:
```typescript
create_object(tag_handle: TagHandle, parent_handle: ObjectHandle | nil, x: number, y: number, z: number)
create_object(tag_handle: TagHandle, parent_handle: ObjectHandle | nil, position: Point3D)
```

### unit_enter_vehicle

Enters the specified unit into the specified vehicle, at the specified seat.

The unit and vehicle must be a handle to units that are currently in the game world. The seat index
exist in the seats list of the tag vehicle, representing the seat to enter. The seat index is
optional, and defaults to 0.

Usage:
```typescript
unit_enter_vehicle(unit_handle: ObjectHandle, vehicle_handle: ObjectHandle, seat_index?: number)
unit_enter_vehicle(unit_handle: ObjectHandle, vehicle_handle: ObjectHandle, seat_name?: string)
```
