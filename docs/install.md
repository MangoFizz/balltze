# Installation
## Installation
### Manual
1. [Install Chimera](https://github.com/SnowyMouse/chimera#installation).
2. Create a folder called "mods" (e.g. "D:\Halo Custom Edition\mods").
3. Grab the latest release from the [releases page](https://github.com/MangoFizz/balltze/releases/latest).
4. Copy balltze.dll file to your mods folder.
5. Copy the contents of the "data" folder on your "My Games\Halo CE" folder.
6. Launch the game for the first time after installing Balltze.

### Mercury
Get Balltze on [Mercury](https://github.com/Sledmine/Mercury) by using the
following command:
```
mercury install balltze
```

!!! note
    The Mercury package may not be up to date with the latest release.

## Building
### Requirements
- MinGW GCC 12.2.0 (lastest version may not work).
- CMake 3.16 or later.
- Lua 5.1 or later.
- libflac 1.4.3
- libogg 1.3.5
- libvorbis 1.3.7

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
The compiled files will be available on the `build` folder.
