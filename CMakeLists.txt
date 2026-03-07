cmake_minimum_required(VERSION 3.28)

project(Practice VERSION 1.0)

if (PKG_CONFIG_EXECUTABLE STREQUAL "")
    set(PKG_CONFIG_EXECUTABLE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/tools/pkgconf/pkgconf.exe")
endif()

find_program(PKG_CONFIG_EXECUTABLE
    NAMES pkgconf pkg-config
    HINTS ${PKG_CONFIG_EXECUTABLE} "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/tools/pkgconf"
    NO_DEFAULT_PATH
)

find_program(PKG_CONFIG_EXECUTABLE NAMES pkgconf pkg-config)

find_package(PkgConfig REQUIRED)

# init_prebuilt_deps()

# Now, each project can find and link against required dependencies.
add_subdirectory(odb_cpp)