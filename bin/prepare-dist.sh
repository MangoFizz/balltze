#!/bin/bash

# License files for used libraries
declare -A license_files
license_files=(
    ["fmt"]="include/fmt/LICENSE.rst"
    ["lua"]="include/lua/COPYRIGHT"
    ["nlohmann-json"]="include/nlohmann/LICENSE.txt"
    ["semver"]="include/semver/COPYRIGHT"
    ["clipboardxx"]="lib/clipboardxx/LICENSE"
    ["invader"]="lib/invader/LICENSE"
    ["lanes"]="lib/lanes/COPYRIGHT"
    ["lfmt"]="lib/lua_fmt/LICENSE"
    ["lua_memory_snapshot"]="lib/lua_memory_snapshot/LICENSE"
    ["luacstruct"]="lib/luacstruct/COPYRIGHT"
    ["luastruct"]="lib/luastruct/LICENSE"
)

# Create dist directory
mkdir -p dist/

# Prepare release files
mkdir -p dist/release/
for library in "${!license_files[@]}"; do
    mkdir -p "dist/release/licenses/$library"
    cp "${license_files[$library]}" "dist/release/licenses/$library/"
done
cp LICENSE dist/release/
cp README.md dist/release/
cp build/balltze.dll dist/release/
cp build/balltze.dll.debug dist/release/
cp -r data/config/ dist/release/ 

# Prepare C++ library dist
mkdir -p dist/cpplib/
mkdir -p dist/cpplib/lib/
for library in "${!license_files[@]}"; do
    mkdir -p "dist/cpplib/licenses/$library"
    cp "${license_files[$library]}" "dist/cpplib/licenses/$library/"
done
cp -r include/ dist/cpplib/
cp LICENSE dist/cpplib/
cp build/libballtze.dll.a dist/cpplib/lib/
cp build/libfmt.a dist/cpplib/lib/
cp cmake/balltze-library.cmake dist/cpplib/balltze.cmake

# Prepare Lua library dist
mkdir -p dist/lualib/
mkdir -p dist/lualib/modules/
mkdir -p dist/lualib/docs/
cp lua/plugins/docs/v2/* dist/lualib/docs/
cp LICENSE dist/lualib/
cp lua/modules/luna.lua dist/lualib/modules/
cp lua/modules/inspect.lua dist/lualib/modules/
cp lua/modules/json.lua dist/lualib/modules/
cp lua/modules/lanes.lua dist/lualib/modules/
