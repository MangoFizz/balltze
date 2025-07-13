-- SPDX-License-Identifier: GPL-3.0-only

inspect = require("inspect")

print("Loading demo.lua...")

Logger = Balltze.logger

Balltze.registerCommand("test", "debug", "Test LuaStruct functionality", nil, false, 0, 0, true, true, function()
    print("Running test command...")

    local mainMenuTagHandle = Engine.tag.lookupTag("ui\\shell\\main_menu\\main_menu", "ui_widget_definition")
    if not mainMenuTagHandle then
        Logger:error("Failed to find main menu tag")
        return false
    end

    local mainMenuTagEntry = Engine.tag.getTagEntry(mainMenuTagHandle)
    if not mainMenuTagEntry then
        Logger:error("Failed to get main menu tag entry")
        return false
    end

    local mainMenuTag = Engine.tag.getTagData(mainMenuTagHandle, "ui_widget_definition")
    if not mainMenuTag then
        Logger:error("Failed to get main menu tag")
        return false
    end

    -- Access string literal of the main menu tag path
    local tagPath = mainMenuTagEntry.path
    Logger:debug("Tag Path: {}", tagPath)
    assert(type(tagPath) == "string", "Expected tagPath to be a string")

    -- Access primitive field of the struct
    local millisecondsToAutoClose = mainMenuTag.millisecondsToAutoClose
    Logger:debug("Widget milliseconds to auto close: {}", millisecondsToAutoClose)
    assert(type(millisecondsToAutoClose) == "number", "Expected millisecondsToAutoClose to be a number")

    -- Access a string/char array of the main menu tag
    local widgetName = mainMenuTag.name
    Logger:debug("Widget Name: {}", inspect(widgetName))
    assert(type(widgetName) == "string", "Expected widgetName to be a string")

    -- Access nested struct
    local widgetBounds = mainMenuTag.bounds
    Logger:debug("Widget Bounds: {}", inspect(widgetBounds))
    assert(type(widgetBounds) == "userdata", "Expected widgetBounds to be userdata")

    -- Access fields of the nested struct
    local left = widgetBounds.left
    local top = widgetBounds.top
    local right = widgetBounds.right
    local bottom = widgetBounds.bottom
    Logger:debug("Widget left: {}; top: {}; right: {}; bottom: {}", left, top, right, bottom)
    assert(type(left) == "number", "Expected left to be a number")
    assert(type(top) == "number", "Expected top to be a number")
    assert(type(right) == "number", "Expected right to be a number")
    assert(type(bottom) == "number", "Expected bottom to be a number")

    -- Access enum field
    local widgetType = mainMenuTag.widgetType
    Logger:debug("Widget Type: {}", widgetType)
    assert(type(widgetType) == "userdata", "Expected widgetType to be a number")

    -- Access tag block
    local childWidgets = mainMenuTag.childWidgets
    Logger:debug("Child Widgets: {}", inspect(childWidgets))
    assert(type(childWidgets) == "userdata", "Expected childWidgets to be userdata")

    -- Access tag block count
    local childWidgetCount = #childWidgets
    Logger:debug("Child Widget Count: {}", childWidgetCount)
    assert(type(childWidgetCount) == "number", "Expected childWidgetCount to be a number")

    -- Access child widget by index
    local secondChildWidget = childWidgets[2]
    Logger:debug("Second Child Widget: {}", inspect(secondChildWidget))
    assert(type(secondChildWidget) == "userdata", "Expected secondChildWidget to be userdata")
    
    -- Access fields of the second child widget
    local secondChildWidgetName = secondChildWidget.name
    Logger:debug("Second Child Widget Name: {}", secondChildWidgetName)
    assert(type(secondChildWidgetName) == "string", "Expected secondChildWidgetName to be a string")

    -- Iterate over all child widgets
    Logger:debug("Iterating over all child widgets:")
    for i, childWidget in ipairs(childWidgets) do
        Logger:debug("Child Widget {}: {}", i, childWidget.name)
        assert(type(childWidget) == "userdata", "Expected childWidget to be userdata")
        assert(type(childWidget.name) == "string", "Expected childWidget.name to be a string")
    end

    return true
end)

function PluginOnGameStart()
    Logger:debug("PluginOnGameStart called")
end
