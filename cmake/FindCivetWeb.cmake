find_path(CIVETWEB_INCLUDE_DIRS civetweb.h PATHS "${CMAKE_SOURCE_DIR}/include/civetweb")
find_library(CIVETWEB_LIBRARY libcivetweb.a PATHS "${CMAKE_SOURCE_DIR}/lib")
set(CIVETWEB_LIBRARIES ${CIVETWEB_LIBRARY} pthread dl)
