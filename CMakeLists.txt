cmake_minimum_required(VERSION 3.28)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(VCPKG_BUILD_TYPE "debug" CACHE STRING "")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(VCPKG_BUILD_TYPE "release" CACHE STRING "")
endif()

project(labs VERSION 1.0 LANGUAGES CXX C)

find_program(PKG_CONFIG_EXECUTABLE
    NAMES pkgconf pkg-config
    HINTS ${PKG_CONFIG_EXECUTABLE} "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/tools/pkgconf"
    NO_DEFAULT_PATH
)
find_program(PKG_CONFIG_EXECUTABLE NAMES pkgconf pkg-config)

# Now, each project can find and link against required dependencies.
include(cmake/helpers.cmake)
add_subdirectory(core-c/modern-c)
add_subdirectory(core-cpp/modern-cpp)
add_subdirectory(computer-vision/ocr_diff_bin)          # OCR Differential Binarization
add_subdirectory(databases/odb_cpp)                     # ODB C++ Library testing
add_subdirectory(games/tic_tak_toe_2d)
add_subdirectory(multithreading/tbb_flow_graph)         # Testing oneTBB's graph
add_subdirectory(networking/asio_chat_cpp)

# Copy the compile_commands.json for tools like clangd
if (CMAKE_EXPORT_COMPILE_COMMANDS)
    message(STATUS "Copying compile_commands.json")
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_BINARY_DIR}/compile_commands.json"
        "${CMAKE_SOURCE_DIR}/compile_commands.json"
    )
endif()