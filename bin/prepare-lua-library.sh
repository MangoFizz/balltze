#!/bin/sh

# Prepare directory
mkdir -p dist/lualib/
mkdir -p dist/lualib/modules/
mkdir -p dist/lualib/docs/

# Copy library annotations
cp lua/plugins/docs/v2/* dist/lualib/docs/

# Copy license
cp LICENSE dist/lualib/

# Copy Lua modules
cp lua/modules/luna.lua dist/lualib/modules/
cp lua/modules/inspect.lua dist/lualib/modules/
cp lua/modules/json.lua dist/lualib/modules/
cp lua/modules/lanes.lua dist/lualib/modules/
