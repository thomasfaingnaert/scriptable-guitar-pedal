find_path(LUA_INCLUDE_DIRS lua.h PATHS "${CMAKE_SOURCE_DIR}/include/lua")
find_library(LUA_LIBRARY liblua.a PATHS "${CMAKE_SOURCE_DIR}/lib")
set(LUA_LIBRARIES ${LUA_LIBRARY} dl)
