find_path(TINYALSA_INCLUDE_DIRS tinyalsa/asoundlib.h PATHS "${CMAKE_SOURCE_DIR}/include/tinyalsa")
find_library(TINYALSA_LIBRARIES libtinyalsa.a PATHS "${CMAKE_SOURCE_DIR}/lib")
