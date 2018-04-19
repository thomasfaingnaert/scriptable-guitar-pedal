#ifndef LUAEFFECT_H_YVU5RZIG
#define LUAEFFECT_H_YVU5RZIG

#include <string>

#include "source.h"
#include "sink.h"
#include "lua.hpp"

class LuaEffect : public Source<float>, public Sink<float>
{
    public:
        LuaEffect(const std::string& filename);
        ~LuaEffect();
        virtual void push(const std::array<float, Constants::BLOCK_SIZE>& data);

    private:
        lua_State *state;
};

#endif /* end of include guard: LUAEFFECT_H_YVU5RZIG */
