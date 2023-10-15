-- SPDX-License-Identifier: GPL-3.0-only

---@type Logger
local logger = {}

function PluginMetadata()
    return {
        name = "Hello World",
        author = "MangoFizz",
        version = "1.0.0",
        targetApi = "1.0.0-rc.1",
        reloadable = true
    }
end

function PluginInit() 
    logger = Balltze.logger.createLogger("Hello World")
    
    Balltze.command.registerCommand("hello", "misc", "Prints 'Hello World' to the console", nil, false, 0, 0, true, true, function()
        Engine.core.consolePrint("Hello {} from a command!", "World")
        logger:info("Hello World command was executed!")
        return true
    end)
    
    return true
end

function PluginLoad() 
    logger:info("Hello World!")
end

function PluginUnload()
    logger:info("Bye bye!")
end
