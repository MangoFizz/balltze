## Getting started

Balltze is not just capable of adding new features to the game, it also extends and provides
features that are already present in the game exposing them through a powerful API that can be
used by plugins.

Plugins can (at the moment) be written in Lua or C++ and can be loaded and unloaded at runtime, 
allowing for quick testing and development of new features. Plugins were designed in mind for 
modders to create their own features and extend the game in ways that are not possible through 
the base game engine, such as adding new commands, new user interface elements or even creating 
new game modes giving them new rules and mechanics.

## Supported languages

### :waxing_gibbous_moon: Lua

Lua is a powerful, efficient, lightweight, embeddable scripting language. It supports procedural
programming, object-oriented programming, functional programming, data-driven programming, and
data description.

It is a great choice for creating plugins that require fast prototyping and development, as it
is a high-level language that is easy to learn and use.

It requires a minimal setup, as it is interpreted at runtime by the game engine and just a few
set of tools are required to develop and test plugins. We also provide a strong IDE integration
trough the use of EmmyLua annotations that can be used to provide code completion and some sort
of type checking, but keep in mind it does not have the same level of control over the game as 
C++.

In summary, Lua offers fast prototyping and development, making it easy to learn and use with 
minimal setup required. However, it may not be ideal for more complex plugins that demand 
low-level access to the game engine.

To develop Lua plugins, all you need is a code editor and a Lua language server. 

### :tools: C++

C++ is a powerful, high-performance, and versatile programming language. It is widely used in
game development, system programming, and other performance-critical applications.

While C++ offers high performance and versatility, making it ideal for developing complex 
plugins that require low-level access to the game engine, it comes with a more complex setup 
and build process compared to Lua, as it needs to be compiled and developed with a 
well-configured IDE. Additionally, it requires a deep understanding of programming concepts 
and best practices, which can be a drawback for fast prototyping and development.

To develop C++ plugins, you will need the following tools:

- [MinGW GCC 12.2.0](https://sourceforge.net/projects/mingw-w64/) - A Windows port of the 
GNU Compiler Collection (GCC) that provides a set of tools for building C and C++ programs.

- [CMake 3.16 or later](https://cmake.org/download/) - A cross-platform build system that 
generates native build files.

!!! note
    You can also use Docker to build the project without having to install the tools on your 
    system. Note that you may also need the Mingw GCC compiler installed on your system to 
    set up your IDE's IntelliSense.

## Plugins folder

Plugins are organized in a specific structure that allows Balltze to load and manage them. 
The plugins folder is located in the Balltze folder at ``My Games\Halo CE\balltze\plugins``. 
Every plugin placed on this folder will be loaded by Balltze, no matter if it is written in Lua 
or C++. 

Each plugin can have its own folder where all the files related to the plugin are placed, 
like modules, assets or configuration files. A plugin folder structure can look like this:

```
My Games\Halo CE\balltze\plugins
├── my_plugin
│   ├── assets
│   │   └── my_asset.png
│   ├── config
│   │   └── my_config.json
│   ├── modules
│   │   └── my_module.lua
│   └── my_plugin.lua
└── my_plugin.lua
```

## Plugins lifecycle

Plugins must provide some specific functions that Balltze will call at different points of the
plugin lifecycle, like when the plugin is loaded or unloaded. The functions that must be provided
are:

- **Plugin metadata:** A function that returns an structure with the plugin metadata, like the name, 
author, version, and description.

- **Plugin load:** A function that is called when the plugin is loaded by Balltze. This function is
responsible for initializing the plugin and setting up any required resources or event listeners.
It returns a boolean value indicating whether the plugin was loaded successfully.

- **Plugin unload:** A function that is called when the plugin is unloaded by Balltze. This function
is responsible for cleaning up any resources or event listeners that were set up during the plugin
load. This may be called when the game is closed or when the plugin is reloaded manually. 

!!! info
    The unload function can be omitted if the plugin does not require any cleanup or if is not meant 
    to be reloaded.

