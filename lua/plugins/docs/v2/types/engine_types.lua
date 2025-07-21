-- SPDX-License-Identifier: GPL-3.0-only

---@meta _

-------------------------------------------------------
-- Engine common types
-------------------------------------------------------

---@class ColorARGB
---@field a number
---@field r number
---@field g number
---@field b number

---@alias ColorARGBInt integer

---@class ColorRGB
---@field r number
---@field g number
---@field b number

---@class VectorXY
---@field x number
---@field y number

---@class VectorXYInt
---@field x integer
---@field y integer

---@class VectorXYZ
---@field x number
---@field y number
---@field z number

---@class VectorIJ
---@field i number
---@field j number

---@class VectorIJK
---@field i number
---@field j number
---@field k number

---@class TableResourceHandle
---@field value integer
---@field index integer
---@field id integer
---@field isNull fun(): boolean

---@class TagHandle: EngineResourceHandle

---@class ObjectHandle: EngineResourceHandle

---@class PlayerHandle: EngineResourceHandle

---@class VectorPYR
---@field pitch number
---@field yaw number
---@field roll number

---@class VectorPY
---@field pitch number
---@field yaw number

---@class Rectangle2D
---@field top integer
---@field left integer
---@field bottom integer
---@field right integer

---@class Bounds2D
---@field top number
---@field left number
---@field bottom number
---@field right number

---@class Plane2D
---@field i number
---@field j number
---@field w number

---@class Plane3D
---@field i number
---@field j number
---@field k number
---@field w number

---@class Angle: number

---@class Quaternion
---@field i number
---@field j number
---@field k number
---@field w number

---@class Matrix: table<integer, table<integer, number>>
