#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <string>

#include "NE10.h"
#include "adder.h"
#include "blockbuffer.h"
#include "civetweb.h"
#include "codec.h"
#include "convolver.h"
#include "delayeffect.h"
#include "distortioneffect.h"
#include "filesink.h"
#include "filesource.h"
#include "processor.h"
#include "sinesource.h"
#include "sink.h"
#include "source.h"
#include "streamsink.h"
#include "tremoloeffect.h"
#include "webserver.h"

extern "C"
{
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

int lua_f(lua_State *state)
{
    int n = lua_tonumber(state, -1);
    lua_pushnumber(state, 2 * n);
    return 1;
}

int main(int argc, char *argv[])
{
    // NE10 Initialisation
    if (ne10_init() != NE10_OK)
    {
        std::cerr << "Could not initialise Ne10." << std::endl;
        return EXIT_FAILURE;
    }

    // Initialise Lua
    lua_State *state = luaL_newstate();

    if (!state)
    {
        std::cerr << "Could not initialise Lua state." << std::endl;
        return EXIT_FAILURE;
    }

    // Open all Lua standard libraries
    luaL_openlibs(state);

    // Load test script
    if (luaL_dofile(state, "examples/test.lua") != LUA_OK)
    {
        std::cerr << "Could not load file" << std::endl;
        return EXIT_FAILURE;
    }

    // Register C function
    lua_pushcfunction(state, lua_f);
    lua_setglobal(state, "f");

    // Push global "main" function on the stack
    lua_getglobal(state, "main");

    // Push argument on the stack
    lua_pushnumber(state, 20);

    // Call
    lua_call(state, 1, 1);

    // Convert to integer
    int result = lua_tonumber(state, -1);

    std::cout << result << std::endl;

    // Clean up
    lua_close(state);

    return EXIT_SUCCESS;
}
