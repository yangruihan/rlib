project(rlib_test)

aux_source_directory(. RLIB_TEST_SRC)

file(GLOB_RECURSE RLIB_TEST_HEADERS ./*.h)
file(GLOB_RECURSE GMOCK_HEADERS "${PROJ_SOURCE_ROOT}/libs/gtest/include/gmock/*.h")
file(GLOB_RECURSE GTEST_HEADERS "${PROJ_SOURCE_ROOT}/libs/gtest/include/gtest/*.h")

source_group("Headers" FILES ${RLIB_TEST_HEADERS})
source_group("Headers/gmock" FILES ${GMOCK_HEADERS})
source_group("Headers/gtest" FILES ${GTEST_HEADERS})

include_directories("${PROJ_SOURCE_ROOT}/libs/gtest/include")
include_directories("${PROJ_SOURCE_ROOT}/test")

set(EXECUTABLE_OUTPUT_PATH "${PROJ_SOURCE_ROOT}/bin/Test")

add_executable(
    rlib_test
    ${RLIB_TEST_SRC}
    ${RLIB_TEST_HEADERS}
    ${GTEST_HEADERS}
    ${GMOCK_HEADERS}
)

get_static_lib_path(gtest gmock GMOCKD_LIB GMOCK_LIB)
get_static_lib_path(gtest gtest GTESTD_LIB GTEST_LIB)

message(STATUS "-+ GMOCKD_LIB: ${GMOCKD_LIB}")
message(STATUS "-+ GMOCK_LIB: ${GMOCK_LIB}")
message(STATUS "-+ GTESTD_LIB: ${GTESTD_LIB}")
message(STATUS "-+ GTEST_LIB: ${GTEST_LIB}")

if (WIN32 OR MSVC)
    target_link_libraries(
        rlib_test
        rlib
        debug ${GMOCKD_LIB} optimized ${GMOCK_LIB}
        debug ${GTESTD_LIB} optimized ${GTEST_LIB}
    )
else()
    target_link_libraries(
        rlib_test
        rlib
        debug ${GMOCKD_LIB} optimized ${GMOCK_LIB}
        debug ${GTESTD_LIB} optimized ${GTEST_LIB}
        pthread
    )
endif()
