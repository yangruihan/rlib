cmake_minimum_required(VERSION 3.1)

project(rlib)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

set(PROJ_SOURCE_ROOT ${PROJECT_SOURCE_DIR})

# ---------- OPTIONS ----------
option(DEBUG "Debug Version" off)
option(RLIB_COMMAND_LINE "Command Line Flag" off)
option(RLIB_LOG_USE_COLOR "Log use color" on)
option(RLIB_BUILD_TESTS "Build the RLib test programs" on)
option(RLIB_ENABLE_ASSERT "Enable Assert" on)

message(STATUS "* DEBUG FLAG: ${DEBUG}")
message(STATUS "* COMMAND LINE FLAG: ${RLIB_COMMAND_LINE}")
message(STATUS "* LOG USE COLOR: ${RLIB_LOG_USE_COLOR}")
message(STATUS "* RLIB BUILD TESTS: ${RLIB_BUILD_TESTS}")
message(STATUS "* RLIB ENABLE ASSERT: ${RLIB_ENABLE_ASSERT}")

if(RLIB_COMMAND_LINE)
    add_definitions(-DCOMMAND_LINE)
endif(RLIB_COMMAND_LINE)

if(RLIB_LOG_USE_COLOR)
    add_definitions(-DLOG_USE_COLOR)
endif(RLIB_LOG_USE_COLOR)

if(WIN32 OR MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG /MTd")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG /MTd")
elseif(APPLE)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
else()
    if(DEBUG)
        add_definitions(-DDEBUG)
    endif(DEBUG)
endif()

# ---------- FUNCTIONS ----------
function(get_static_lib_path SUB_FOLDER LIB_NAME DEBUG_OUT RELEASE_OUT)
    if (WIN32 OR MSVC)
        set(${DEBUG_OUT} "${PROJ_SOURCE_ROOT}/libs/${SUB_FOLDER}/win/debug/${LIB_NAME}.lib" PARENT_SCOPE)
        set(${RELEASE_OUT} "${PROJ_SOURCE_ROOT}/libs/${SUB_FOLDER}/win/release/${LIB_NAME}.lib" PARENT_SCOPE)
        message(STATUS "+ Get static lib debug: ${DEBUG_OUT}")
        message(STATUS "+ Get static lib release: ${RELEASE_OUT}")
    elseif (APPLE)
        set(${DEBUG_OUT} "${PROJ_SOURCE_ROOT}/libs/${SUB_FOLDER}/macos/debug/${LIB_NAME}.a" PARENT_SCOPE)
        set(${RELEASE_OUT} "${PROJ_SOURCE_ROOT}/libs/${SUB_FOLDER}/macos/release/${LIB_NAME}.a" PARENT_SCOPE)
        message(STATUS "+ Get static lib debug: ${DEBUG_OUT}")
        message(STATUS "+ Get static lib release: ${RELEASE_OUT}")
    elseif (UNIX)
        set(${DEBUG_OUT} "${PROJ_SOURCE_ROOT}/libs/${SUB_FOLDER}/unix/debug/${LIB_NAME}.a" PARENT_SCOPE)
        set(${RELEASE_OUT} "${PROJ_SOURCE_ROOT}/libs/${SUB_FOLDER}/unix/release/${LIB_NAME}.a" PARENT_SCOPE)
        message(STATUS "+ Get static lib debug: ${DEBUG_OUT}")
        message(STATUS "+ Get static lib release: ${RELEASE_OUT}")
    endif()
endfunction()

include_directories("${PROJ_SOURCE_ROOT}/include")

add_subdirectory(src)

if (RLIB_BUILD_TESTS)
    add_subdirectory(test)
endif()