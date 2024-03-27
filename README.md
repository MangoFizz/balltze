# Balltze
Balltze is a mod for Halo Custom Edition that aims to be what 
[LiteLoaderBDS](https://github.com/LiteLDev/LiteLoaderBDS) is for the Minecraft
Bedrock dedicated server.

This mod currently has two main goals:
1. Restore most of the functionality from the original Xbox version that is missing on the PC versions.
2. Create an API that allows you to manipulate every aspect of the game from your DLL or Lua script. This could (eventually) help in the 
process of creating a reimplementation of the game's netcode.

## Installation
### Manual
1. [Install Chimera](https://github.com/SnowyMouse/chimera#installation).
2. Create a folder called "mods" (e.g. "D:\Halo Custom Edition\mods").
3. Copy balltze.dll file to your mods folder.
4. Copy the contents of the "data" folder on your "My Games\Halo CE" folder.

### Mercury
Get Balltze on [Mercury](https://github.com/Sledmine/Mercury) by using the
following command:
```
mercury install balltze
```

## Building
### Requirements
- MinGW GCC 12.2.0.
- CMake 3.16 or later.
- Lua 5.1 or later.

### Compiling
1. Clone the repository.
```
git clone https://github.com/MangoFizz/balltze && cd balltze
```
2. Create a build folder.
```
mkdir build && cd build
```
3. Generate the build files.
```
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../toolchain-i686-w64-mingw32.cmake
```
4. Build the project.
```
cmake --build .
```
Depending on your system, you may need to modify the toolchain file to match your 
environment.

### Compiling using Docker
There is a docker image available for building the project without having to install libraries and
tools on your system. To build the image, run:
```
docker build . -t bonk
```
Now use docker compose to build the project:
```
docker compose run compile
```

NOTE: If you want to build the project with the experimental features, you need to
define the `BALLTZE_ENABLE_EXPERIMENTAL` CMake variable to `ON` when generating the 
build files.

## Contributing
If you have any suggestions or want to contribute in any other way to the project, feel 
free to do so and submit a pull request. The project has big goals in mind, so any kind
of help is more than welcome.

## Special mentions 
- [Chimera](https://github.com/SnowyMouse/chimera) - The biggest mod for Halo 1 on PC and 
a great source of inspiration for the development of this mod.

- [Lua-blam](https://github.com/Sledmine/lua-blam) - The first library that dealt with the 
manipulation of tag structures.

- [Invader](https://github.com/SnowyMouse/invader) - Free and open source toolkit for Halo: 
Combat Evolved for creating maps and assets. 

- [LiteLoaderBDS](https://github.com/LiteLDev/LiteLoaderBDS) - A cross-language plugin loader
for Minecraft Bedrock Dedicated Server.

- [json](https://github.com/nlohmann/json) - Nlohmann's JSON library for C++. Used in this
project.

- [Lazy-Grunt](https://github.com/Modzybear/Lazy-Grunt) - A dead project that like Balltze, 
had the goal of generating headers for tag structures.

## Credits 
- Elaya Deerson - Testing and supply of assets.
- EzDeath - Testing and ideas.
- MrChromed - Testing and moral support.
- [Sledmine](https://github.com/Sledmine) - Development and testing.
- [SnowyMouse](https://github.com/SnowyMouse) - Development of Chimera and Invader.
