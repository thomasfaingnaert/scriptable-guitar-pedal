find_path(I2C_INCLUDE_DIRS smbus.h PATHS "${CMAKE_SOURCE_DIR}/include/i2c")
find_library(I2C_LIBRARIES libi2c.a PATHS "${CMAKE_SOURCE_DIR}/lib")
