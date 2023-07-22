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

NOTE: The builds available on Mercury include the experimental features, so if you want to
use the mod without them, download the latest release from the GitHub's releases page instead.

## Building
### Requirements
- MinGW GCC 12.2.0 or later.
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

NOTE: If you want to build the project with the experimental features, you need to
define the `BALLTZE_ENABLE_EXPERIMENTAL` CMake variable to `ON` when generating the 
build files.

## Features
Balltze is still under development, but currently it has two experimental 
features that someone out there may find useful for their evil modding 
purposes, and of course, it brings back a feature from the original xbox version that 
many users had been requesting for years and maybe it's the reason why 
you're here reading this.

### Tag definitions
It is common practice in Halo Custom Edition modding as far as scripting is concerned, 
to access map tag data. However, since there is no way to access every field of a tag 
structure, this has to be through memory addresses and offsets which just makes the 
scripts hard to read and maintain. Plus, it's very easy to make a mistake and cause the 
game to crash and end up with a "The game has encountered a segmentation fault/gathering exception data"
message on your screen.

That's the reason why, from the Invader tag definitions, this project generates a set 
of C++ headers which contains every tag structure of Halo Custom Edition, putting an 
end to all those horrible offsets in your code. At the moment an API that implements 
these structures for using it from Lua scripts is under develoment, but you can already
use the header files in your C++ mod.

### Tag data import
This is probably the most important and monstrous feature of this mod; it allows you 
to **import tags from other maps** to the current loaded map. Needless to say the number 
of possibilities that this feature are endless, from making new and additional resource
maps for your map pack, to build a skins-like system similar to what currently exists in 
the master chief collection for vehicles and weapons.

Currently it just loads all the ui.map tags into each map, since like the previous 
feature, it needs an interface for Lua that allows you to choose which tags to import 
from which map.

### HUD gamepad button icons
Balltze restores this function from the Xbox version, it uses the icons which comes 
with the stock tagset in the HUD globals by mapping them to a button in a configuration 
file. However you can add more icons to the HUD globals and set them to a button in the
configuration file.

### Gamepad pause button
Allows you to pause the game from your gamepad. Like the icons, it can be configured.

## Motivation
The main motivation for this is to improve the process of developing a mod for the 
game, so people can work on their mods without spending hours looking at the tag structures 
in a hex editor just to find out where is the field they want to read on their mods. As 
well as bringing the game as close as possible to what the original Xbox version is.

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
