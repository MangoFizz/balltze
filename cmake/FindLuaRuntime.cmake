# Locate Lua runtime
# This module defines
#  LUA_EXECUTABLE, if found
#  LUA_FOUND, if false, do not try to link to Lua 
#
#=============================================================================
# from https://github.com/aerospike/lua/blob/master/cmake/FindLua.cmake
#=============================================================================

# Always search for non-versioned lua first (recommended)
SET(_POSSIBLE_LUA_EXECUTABLE lua)
SET(_POSSIBLE_SUFFIXES "52" "5.2" "-5.2" "51" "5.1" "-5.1" "53" "5.3" "-5.3")

# Set up possible search names and locations
FOREACH(_SUFFIX ${_POSSIBLE_SUFFIXES})
    LIST(APPEND _POSSIBLE_LUA_EXECUTABLE "lua${_SUFFIX}")
ENDFOREACH(_SUFFIX)

# Find the lua executable
FIND_PROGRAM(LUA_EXECUTABLE
    NAMES ${_POSSIBLE_LUA_EXECUTABLE}
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LuaRuntime REQUIRED_VARS LUA_EXECUTABLE)

MARK_AS_ADVANCED(LUA_EXECUTABLE)
