find_path(LIBSNDFILE_INCLUDE_DIRS sndfile.hh PATHS "${CMAKE_SOURCE_DIR}/include/libsndfile")
find_library(LIBSNDFILE_LIBRARIES libsndfile.a PATHS "${CMAKE_SOURCE_DIR}/lib")
