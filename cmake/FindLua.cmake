find_path(LUA_INCLUDE_DIRS lua.h PATHS "${CMAKE_SOURCE_DIR}/include/lua")
find_library(LUA_LIBRARIES liblua.a PATHS "${CMAKE_SOURCE_DIR}/lib")
