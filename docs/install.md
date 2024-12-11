# Installation

## Using Mercury :fontawesome-solid-droplet:
Get Balltze with a single command:
```
mercury install balltze
```

You can install Mercury by following the instructions on the [official repository](https://github.com/Sledmine/Mercury).

## Manual
1. [Install Chimera](https://github.com/SnowyMouse/chimera#installation). This mod needs Chimera in order to work.
2. Create a folder called "mods" (e.g. `D:\Halo Custom Edition\mods`).
3. Grab the latest release from the [releases page](https://github.com/MangoFizz/balltze/releases/latest).
4. Copy `balltze.dll` file to your mods folder.
5. Copy the contents of the `data` folder on your `My Games\Halo CE` folder.
6. Launch the game for the first time after installing Balltze.

## Building

Balltze is intended to be easy to build, it uses CMake to generate all the necessary files to compile the project. Even though it is a simple process, there are some requirements that need to be met before building the project.

As the result of the build process, you will get the DLL file and its link library that can be used to create
plugins by developers.

One of the points that need to be highlighted is that Balltze should be linked statically with the libraries it uses. This is to avoid needing to distribute the DLLs of the libraries with the mod, making it easier to install and use.

### Requirements

!!! tip
    If you are using :simple-archlinux: Arch Linux, you can get the required libraries from AUR repositories.

- MinGW GCC 12.2.0 (lastest version may not work correctly)
- CMake 3.22 or later
- Lua 5.3
- libflac 1.4.3
- libogg 1.3.5 
- libvorbis 1.3.7 

### Procedure

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

!!! info
    Depending on your system, you may need to modify the toolchain file to match your 
    environment.

## Building using Docker
There is a Dockerfile available for building the project without having to install libraries and
tools on your system. You only need to have Docker installed on your system.

To build the image, run:
```
docker build . -t balltze-dev-env
```
Now use docker compose to build the project:
```
docker compose run build
```
The compiled files will be available on the `build` folder inside of the project directory.
