#ifndef LUAEFFECT_H_YVU5RZIG
#define LUAEFFECT_H_YVU5RZIG

#include <string>

#include "lua.hpp"
#include "processor.h"

class LuaEffect : public Processor<float, float>
{
    public:
        LuaEffect(const std::string& filename);
        ~LuaEffect();
        virtual std::shared_ptr<std::vector<float>> process(const std::vector<std::shared_ptr<std::vector<float>>> &data);

    private:
        lua_State *state;
};

#endif /* end of include guard: LUAEFFECT_H_YVU5RZIG */
