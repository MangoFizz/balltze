-- SPDX-License-Identifier: GPL-3.0-only

---@meta _

-------------------------------------------------------
-- Engine cache files types
-------------------------------------------------------

---@alias CacheFileType
---| "singlePlayer"
---| "multiplayer"
---| "userInterface"

---@class CacheFileHeader
---@field fileSize integer
---@field name string
---@field build string
---@field gameType CacheFileType
---@field crc32 integer
