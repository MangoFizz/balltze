# Getting Started with Balltze
Balltze is a mod for Halo Custom Edition that adds new features to the game. This document will
guide you through the process of installing and using Balltze.

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

## Introduction to Balltze
Balltze is not just capable of adding new features to the game, it also extends and provides
existing features already present in the game exposing them trough a powerful API that can be
used by plugins.

Plugins can be written in Lua or C++ and can be loaded and unloaded at runtime, allowing for
quick testing and development of new features.

Plugins were designed in mind for modders to create their own features and extend the game in
ways that are not possible trough the base game engine, such as creating a new game mode, adding new
commands, or even creating a reimplementation of the game's netcode.

## Choosing a Language for Plugins

### Lua
Lua is a powerful, efficient, lightweight, embeddable scripting language. It supports procedural
programming, object-oriented programming, functional programming, data-driven programming, and
data description.

It is a great choice for creating plugins that require fast prototyping and development, as it
is a high-level language that is easy to learn and use.

It requires a minimal setup, as it is interpreted at runtime by the game engine and just a few
set of tools are required to develop and test plugins. We also provide a strong IDE integration
trough the use of EmmyLua annotations that can be used to provide code completion and some sort
of type checking, but keep in mind it does not have the same level of game control as C++.

In summary:
- Fast prototyping and development.
- Easy to learn and use.
- Minimal setup required.
- Not ideal for more complex plugins that require low-level access to the game engine.

### C++
C++ is a powerful, high-performance, and versatile programming language. It is widely used in
game development, system programming, and other performance-critical applications.

However, C++ is a complex language that requires a deep understanding of programming concepts
and best practices. It is a great choice for creating plugins that require low-level access to
the game engine.

It requires a more complex setup and build process compared to Lua because it needs to be compiled
and developed with a well configured IDE.

In summary:
- High-performance and versatile.
- Ideal for more complex plugins that require low-level access to the game engine.
- More complex setup and build process.
- Requires a deep understanding of programming concepts and best practices.
- Not ideal for fast prototyping and development.

## Gathering tools
### Lua 🌘
To develop Lua plugins, you will need the following tools:
- [Visual Studio Code](https://code.visualstudio.com/)
- [Lua Language Server for VS Code](https://marketplace.visualstudio.com/items?itemName=sumneko.lua)
- [Git](https://git-scm.com/)
- [Mercury](https://mercury.shadowmods.net/)

### C++ 💀
To develop C++ plugins, you will need the following tools:
- [Visual Studio Code](https://code.visualstudio.com/)
- [MinGW GCC 12.2.0](https://sourceforge.net/projects/mingw-w64/)
- [CMake 3.16 or later](https://cmake.org/download/)
- [Git](https://git-scm.com/)

**NOTE**: You can also use Docker to build the project without having to install libraries and tools
on your system.

## Creating a Plugin
To create a plugin, you can use the template repository provided by Balltze. This repository
contains a basic plugin structure that you can use as a starting point for your language of choice.

### Lua
To create a Lua plugin, you can use the following steps:
1. Clone the [Lua plugin template repository](https://github.com/MangoFizz/balltze-plugin-lua)
2. Open the repository in Visual Studio Code.
3. Run Mercury command to link the plugin to your Balltze installation:
```
mercury link
```
This will create a symbolic link to your Balltze installation, allowing you to test your plugin
without having to copy it to the plugins folder.

4. Start developing your plugin!

### C++
TODO
