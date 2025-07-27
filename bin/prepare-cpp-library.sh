#!/bin/bash

# Prepare directory
mkdir -p dist/cpplib/
mkdir -p dist/cpplib/lib/

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
    mkdir -p "dist/cpplib/licenses/$library"
    cp "${license_files[$library]}" "dist/cpplib/licenses/$library/"
done

# Copy library includes
cp -r include/ dist/cpplib/

# Copy license
cp LICENSE dist/cpplib/

# Copy binary files to dist
cp build/libballtze.dll.a dist/cpplib/lib/
cp build/libfmt.a dist/cpplib/lib/

# Copy CMake include file
cp cmake/balltze-library.cmake dist/cpplib/balltze.cmake
