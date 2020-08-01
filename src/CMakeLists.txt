aux_source_directory(. SRC_FILES)

file(GLOB_RECURSE HEADERS ./*.h)
source_group("Headers/rlib" FILES ${HEADERS})

add_library(
    RLib
    ${SRC_FILES}
    ${HEADERS}
)