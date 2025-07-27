#!/bin/sh

# Copy license files for used libraries
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
for library in "${!license_files[@]}"; do
    mkdir -p "dist/release/licenses/$library"
    cp "${license_files[$library]}" "dist/release/licenses/$library/"
done

# Copy the license and readme file for the project
cp LICENSE dist/release/
cp README.md dist/release/

# Copy binary file to dist
cp build/balltze.dll dist/release/

# Copy config files
cp -r data/config/ dist/release/ 
