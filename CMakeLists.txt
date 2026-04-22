cmake_minimum_required(VERSION 3.28)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(VCPKG_BUILD_TYPE "debug" CACHE STRING "")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(VCPKG_BUILD_TYPE "release" CACHE STRING "")
endif()

project(Practice VERSION 1.0)

find_program(PKG_CONFIG_EXECUTABLE
    NAMES pkgconf pkg-config
    HINTS ${PKG_CONFIG_EXECUTABLE} "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/tools/pkgconf"
    NO_DEFAULT_PATH
)

find_program(PKG_CONFIG_EXECUTABLE NAMES pkgconf pkg-config)

find_package(PkgConfig REQUIRED)

# Now, each project can find and link against required dependencies.
add_subdirectory(odb_cpp)       # ODB C++ Library testing
add_subdirectory(ocr_diff_bin)  # OCR Differential Binarization
add_subdirectory(tbb_flow_graph)  # Testing oneTBB's parallel_pipeline