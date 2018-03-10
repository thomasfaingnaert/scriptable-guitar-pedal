find_path(LIBI2C_INCLUDE_DIRS smbus.h PATHS "${CMAKE_SOURCE_DIR}/include/i2c")
find_library(LIBI2CFILE_LIBRARIES libi2c.a PATHS "${CMAKE_SOURCE_DIR}/lib")
