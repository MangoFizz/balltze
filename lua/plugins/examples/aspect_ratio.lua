-- SPDX-License-Identifier: GPL-3.0-only

function PluginMetadata()
    return {
        name = "Widescreen UI aspect ratio",
        author = "MangoFizz",
        version = "1.0.0",
        targetApi = "1.0.0-rc.1",
        reloadable = false
    }
end

function PluginInit() 
    Balltze.event.mapLoad.subscribe(function (event)
        if event.time == "before" and event.context:mapName() == "levels\\ui\\ui" then
            Balltze.features.setUIAspectRatio(16, 9)
        end
    end)
    return true
end

function PluginLoad() 
end
