#include "luaeffect.h"

#include <iostream>
#include <stdexcept>

LuaEffect::LuaEffect(const std::string& filename)
    : Processor(1)
{
    // Initialise Lua state
    state = luaL_newstate();

    if (!state)
    {
        throw std::runtime_error("Could not create a new Lua state");
    }

    // Load standard Lua libraries
    luaL_openlibs(state);

    // Load the input script
    if (luaL_dofile(state, filename.c_str()) != LUA_OK)
    {
        throw std::runtime_error("Error loading effect script");
    }
}

LuaEffect::~LuaEffect()
{
    // Clean up Lua state
    lua_close(state);
}

std::shared_ptr<std::vector<float>> LuaEffect::process(const std::vector<std::shared_ptr<std::vector<float>>> &data)
{
    // Push the global 'process' function to the top of the stack
    lua_getglobal(state, "process");

    // Create a table on the top of the stack to contain the input
    lua_createtable(state, data[0]->size(), 0);

    // Add all samples to table; remember Lua indices start at 1!
    for (unsigned int i = 1; i <= data[0]->size(); ++i)
    {
        // Push key
        lua_pushinteger(state, i);

        // Push value
        lua_pushnumber(state, data[0]->at(i-1));

        // Add key-value pair to table
        lua_settable(state, -3);
    }

    // Call the process function
    lua_call(state, 1, 1);

    // Variable to store result
    auto result = std::make_shared<std::vector<float>>(data[0]->size());

    // Get the return value from calling 'process' and convert to std::vector
    for (unsigned int i = 1; i <= BLOCK_SIZE; ++i)
    {
        // Push the desired index
        lua_pushinteger(state, i);

        // Push the item at that index to top of stack
        lua_gettable(state, -2);

        // Convert that to an integer
        float n = lua_tonumber(state, -1);

        // Add it to the result vector
        result->at(i-1) = n;

        // Pop the value from the stack
        lua_pop(state, 1);
    }

    return result;
}
