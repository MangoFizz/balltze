// SPDX-License-Identifier: GPL-3.0-only

#include <lua.hpp>
#include <balltze/math.hpp>
#include "helpers.hpp"

namespace Balltze::Plugins {
    static int lua_get_bezier_curve_point(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 4 || args == 5) {
            auto initial_value = luaL_checkinteger(state, 2);
            auto final_value = luaL_checkinteger(state, 3);
            float t = luaL_checknumber(state, 4);

            // Check if the first argument is a table
            if(!lua_istable(state, 1)) {
                return luaL_error(state, "invalid curve in Balltze.math.getBezierCurvePoint function");
            }
            
            // Get curve from userdata
            lua_getfield(state, 1, "_curve");
            auto *curve = reinterpret_cast<Math::QuadraticBezier *>(lua_touserdata(state, -1));
            
            bool reverse = false;
            if(args == 5) {
                reverse = lua_toboolean(state, 5);
            }

            // fool proof
            if(t > 1.0f) {
                t = 1;
            }
            else if(t < 0.0f) {
                t = 0;
            }

            auto point = curve->get_point(t, reverse);
            float transform = point.y * (final_value - initial_value);
            float result = transform + initial_value;
            lua_pushnumber(state, result);

            return 1;
        }
        else {
            return luaL_error(state, "invalid number of arguments in Balltze.math.getBezierCurvePoint function");
        }
    }

    static int lua_create_bezier_curve(lua_State *state) noexcept {
        int args = lua_gettop(state);
        if(args == 1 || args == 4) {
            Math::QuadraticBezier new_curve;
            if(args == 1) {
                std::string preset = luaL_checkstring(state, 1);
                if(preset == "ease in") {
                    new_curve = Math::QuadraticBezier::ease_in();
                }
                else if(preset == "ease out") {
                    new_curve = Math::QuadraticBezier::ease_out();
                }
                else if(preset == "ease in out") {
                    new_curve = Math::QuadraticBezier::ease_in_out();
                }
                else if(preset == "linear") {
                    new_curve = Math::QuadraticBezier::linear();
                }
                else {
                    return luaL_error(state, "invalid curve name in Balltze.math.createBezierCurve function");
                }
            }
            else {
                float x1 = luaL_checknumber(state, 1);
                float y1 = luaL_checknumber(state, 2);
                float x2 = luaL_checknumber(state, 3);
                float y2 = luaL_checknumber(state, 4);
                new_curve = Math::QuadraticBezier({x1, y1}, {x2, y2});
            }

            lua_newtable(state);
            
            auto *curve = reinterpret_cast<Math::QuadraticBezier *>(lua_newuserdata(state, sizeof(Math::QuadraticBezier)));
            *curve = new_curve;
            lua_setfield(state, -2, "_curve");
            
            lua_pushcfunction(state, lua_get_bezier_curve_point);
            lua_setfield(state, -2, "getPoint");

            return 1;
        }
        else {
            return luaL_error(state, "invalid number of arguments in Balltze.math.createBezierCurve function");
        }
    }

    static const luaL_Reg math_functions[] = {
        {"createBezierCurve", lua_create_bezier_curve},
        {"getBezierCurvePoint", lua_get_bezier_curve_point},
        {nullptr, nullptr}
    };

    void lua_set_math_table(lua_State *state) noexcept {
        lua_create_functions_table(state, "math", math_functions);
    }
}
