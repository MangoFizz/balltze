# Creating your first Lua plugin

Once you have Balltze installed and all the required tools set up you can start creating your own 
plugins. In this guide we will create a simple Lua plugin that will print a message to the game 
console.

## Let's get started

To create a new plugin, there is a template repository that you can use as a starting point. It 
contains the Lua annotations that you can use to get code completion and documentation in your IDE. 
You can create a new repository from the template by clicking on the "Use this template" button on 
the [repository page](https://github.com/MangoFizz/balltze-plugin-lua).

### Step 1: Clone the repository

Once you have created a new repository from the template, you can clone it to your local machine 
using Git. Open a terminal and run the following command:

```bash
git clone https://github.com/yourusername/my-lua-plugin.git
```

Replace `yourusername` with your GitHub username and `my-lua-plugin` with the name of the 
repository you created from the template (or just copy the link from the repository page).

### Step 2: Open the project in your IDE

Open the project in your favorite code editor. We recommend using Visual Studio Code with the Lua 
Language Server extension from sumneko. This extension provides code completion, type checking, and 
other features that will help you develop your plugin. 

You can install the Lua Language Server extension from the Visual Studio Code marketplace:

[Download Lua Language Server for VS Code](https://marketplace.visualstudio.com/items?itemName=sumneko.lua)

### Step 3: Creating the plugin

Create a new folder called `lua` in the root of the project. Inside this folder, create a new Lua 
file called `hello.lua`. 

As you could see in the Introduction, Lua plugins are organized in a specific structure that allows 
Balltze to manage them. We need to create three functions in the `hello.lua` file: `PluginLoad`, 
`PluginUnload`, and `PluginMetadata`. These functions will be called by Balltze when the plugin is 
loaded, unloaded, and to get the plugin metadata, respectively.

The PluginMetadata function should return a table with the plugin metadata, with the following 
fields: `name`, `author`, `version`, and `description`. The PluginLoad must return a boolean value 
indicating if the plugin was loaded successfully. The PluginUnload function is optional and can be 
used to clean up resources when the plugin is unloaded.

Here is an example of the `hello.lua` file:

```lua
local function PluginMetadata()
    return {
        name = "Hello Plugin",
        author = "Your Name",
        version = "1.0.0",
        description = "A simple Lua plugin that prints a message to the console."
    }
end

local function PluginLoad()
    Engine.core.consolePrint("Hello from the Hello Plugin!")
    return true
end
```

### Step 4: Loading the plugin

In order to load the plugin in Balltze, you need to copy you main Lua file to the `plugins` folder 
in the Balltze directory. The `plugins` folder is located in the Balltze folder at `My Games\Halo 
CE\balltze\plugins`. Every plugin placed in this folder will be loaded by Balltze, no matter if it 
is written in Lua or C++. So you can copy the `hello.lua` file to the `plugins` folder and we 
should be ready to go.

### Step 5: Testing the plugin

Now that the plugin is loaded, you can start Halo CE and open the console by pressing the tilde key 
(`~`). You should see the message printed by the plugin in the console.

Congratulations! You have created your first Lua plugin for Balltze. You can now start experimenting 
with more complex plugins and features. Continue reading the documentation to learn more about the 
Balltze API and how to create more advanced plugins.
