clua_version = 2.056

local res = pcall(require, "mods.balltze")
if(res) then
    load_balltze_chimera_table()
    set_callback("map load", "load_balltze_chimera_table")
else 
    console_out("Failed to load Balltze Lua library " .. res)
end
