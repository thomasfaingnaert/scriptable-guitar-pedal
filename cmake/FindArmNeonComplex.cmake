find_path(ARM_NEON_COMPLEX_INCLUDE_DIRS arm_neon_complex.h PATHS "${CMAKE_SOURCE_DIR}/include/arm-neon-complex")
find_library(ARM_NEON_COMPLEX_LIBRARIES libarm-neon-complex.a PATHS "${CMAKE_SOURCE_DIR}/lib")
