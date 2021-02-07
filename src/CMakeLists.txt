aux_source_directory(. RLIB_SRC)

include_directories(${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE RLIB_HEADERS ./*.h)
source_group("include/rlib" FILES ${RLIB_HEADERS})

add_library(
    rlib
    ${RLIB_SRC}
    ${RLIB_HEADERS}
)