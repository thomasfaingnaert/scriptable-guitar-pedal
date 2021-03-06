find_path(XENOMAI_ROOT_INCLUDE_DIR xeno_config.h PATHS "${CMAKE_SOURCE_DIR}/include/xenomai")
set(XENOMAI_INCLUDE_DIRS ${XENOMAI_ROOT_INCLUDE_DIR})
set(XENOMAI_DEFINITIONS "-D_GNU_SOURCE -D_REENTRANT -D__COBALT__ -D__COBALT_WRAP__")
set(XENOMAI_CFLAGS "-fasynchronous-unwind-tables")

find_file(XENOMAI_COBALT_WRAPPERS cobalt.wrappers PATHS "${CMAKE_SOURCE_DIR}/lib/xenomai")
find_file(XENOMAI_MODECHK_WRAPPERS modechk.wrappers PATHS "${CMAKE_SOURCE_DIR}/lib/xenomai")
find_file(XENOMAI_BOOTSTRAP_OBJ_FILE bootstrap.o PATHS "${CMAKE_SOURCE_DIR}/lib/xenomai")
find_file(XENOMAI_DYNLIST dynlist.ld PATHS "${CMAKE_SOURCE_DIR}/lib/xenomai")
find_library(XENOMAI_COBALT_LIBRARY libcobalt.a PATHS "${CMAKE_SOURCE_DIR}/lib/xenomai")
find_library(XENOMAI_MODECHK_LIBRARY libmodechk.a PATHS "${CMAKE_SOURCE_DIR}/lib/xenomai")
set(XENOMAI_LIBRARIES "-Wl,--no-as-needed -Wl,@${XENOMAI_COBALT_WRAPPERS} -Wl,@${XENOMAI_MODECHK_WRAPPERS} ${XENOMAI_BOOTSTRAP_OBJ_FILE} -Wl,--wrap=main -Wl,--dynamic-list=${XENOMAI_DYNLIST} ${XENOMAI_COBALT_LIBRARY} ${XENOMAI_MODECHK_LIBRARY} -lpthread -lrt")
