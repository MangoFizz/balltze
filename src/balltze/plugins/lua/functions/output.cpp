// SPDX-License-Identifier: GPL-3.0-only

#include <string>
#include <lua.hpp>
#include <balltze/output.hpp>
#include "../helpers/function_table.hpp"
#include "../types.hpp"
#include "../../loader.hpp"
#include "../../../logger.hpp"

namespace Balltze::Plugins::Lua {
    static int lua_play_bik_video(lua_State *state) {
        int args = lua_gettop(state);
        if(args == 1) {
            auto video = luaL_checkstring(state, 1);
            play_bik_video(video);
            return 0;
        }
        else {
            return luaL_error(state, "Invalid number of arguments in function Balltze.output.playBikVideo.");
        }
        return 0;
    }

    static const luaL_Reg output_functions[] = {
        {"playBikVideo", lua_play_bik_video},
        {nullptr, nullptr}
    };

    void set_output_table(lua_State *state) noexcept {
        create_functions_table(state, "output", output_functions);
    }
}
