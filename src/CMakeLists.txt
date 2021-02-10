aux_source_directory(. RLIB_SRC)

file(GLOB_RECURSE RLIB_HEADERS ../include/*.h)
source_group("include/rlib" FILES ${RLIB_HEADERS})

add_library(
    rlib
    ${RLIB_SRC}
    ${RLIB_HEADERS}
)