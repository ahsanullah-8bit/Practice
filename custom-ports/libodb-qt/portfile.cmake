include(CMakePackageConfigHelpers)

if (VCPKG_TARGET_IS_WINDOWS)
    set(ODB_QT_URL "https://github.com/ahsanullah-8bit/APSS/releases/download/v0.1/libodb-2.5.0-release.zip")
    set(ODB_QT_URL_DEBUG "https://github.com/ahsanullah-8bit/APSS/releases/download/v0.1/libodb-2.5.0-debug.zip")
elseif(VCPKG_TARGET_IS_LINUX)
    set(ODB_QT_URL "")
    set(ODB_QT_URL_DEBUG "") # SEE SECTION 1 (below) FOR MORE DETAILS
endif()

vcpkg_download_distfile(ARCHIVE
    URLS ${ODB_QT_URL}
    FILENAME "libodb-qt-2.5.0.tar.gz"
    SHA512 0
)
vcpkg_download_distfile(ARCHIVE_DEBUG
    URLS ${ODB_QT_URL_DEBUG}
    FILENAME "libodb-qt-2.5.0-debug.tar.gz"
    SHA512 0
)

vcpkg_extract_source_archive(
    SOURCE_PATH
    ARCHIVE "${ARCHIVE}"
)
vcpkg_extract_source_archive(
    SOURCE_PATH_DEBUG
    ARCHIVE "${ARCHIVE_DEBUG}"
)

file(INSTALL "${SOURCE_PATH}/bin" DESTINATION "${CURRENT_PACKAGES_DIR}")
file(INSTALL "${SOURCE_PATH}/include" DESTINATION "${CURRENT_PACKAGES_DIR}")
file(INSTALL "${SOURCE_PATH}/lib" DESTINATION "${CURRENT_PACKAGES_DIR}")
file(INSTALL "${SOURCE_PATH}/share" DESTINATION "${CURRENT_PACKAGES_DIR}")
file(INSTALL "${SOURCE_PATH_DEBUG}/bin" DESTINATION "${CURRENT_PACKAGES_DIR}/debug")
file(INSTALL "${SOURCE_PATH_DEBUG}/lib" DESTINATION "${CURRENT_PACKAGES_DIR}/debug")

file(GLOB PC_FILES "${CURRENT_PACKAGES_DIR}/lib/pkgconfig/*.pc")
file(GLOB PC_FILES_DEBUG "${CURRENT_PACKAGES_DIR}/debug/lib/pkgconfig/*.pc")

list(APPEND PC_FILES ${PC_FILES_DEBUG})
foreach(PC_FILE IN LISTS PC_FILES)
    vcpkg_replace_string("${PC_FILE}" "\\\\" "/")
endforeach(PC_FILE IN LISTS PC_FILES)


# Section 1: Prebuilt binaries for Windows
# https://codesynthesis.com/download/odb/2.5.0/windows/windows10/x86_64/libodb-2.5.0-x86_64-windows10-msvc17.10-debug.zip
# https://codesynthesis.com/download/odb/2.5.0/windows/windows10/x86_64/libodb-2.5.0-x86_64-windows10-msvc17.10.zip
# https://codesynthesis.com/download/odb/2.5.0/windows/windows10/x86_64/libodb-sqlite-2.5.0-x86_64-windows10-msvc17.10-debug.zip
# https://codesynthesis.com/download/odb/2.5.0/windows/windows10/x86_64/libodb-qt-2.5.0-x86_64-windows10-msvc17.10-debug.zip

# odb 2.5.0
# https://codesynthesis.com/download/odb/2.5.0/windows/windows10/x86_64/odb-2.5.0-x86_64-windows10.zip

# Set the ODB_QT_URL for other platforms if needed.
# VCPKG_TARGET_IS_WINDOWS (also true for UWP and MinGW)
# VCPKG_TARGET_IS_UWP
# VCPKG_TARGET_IS_MINGW
# VCPKG_TARGET_IS_LINUX
# VCPKG_TARGET_IS_OSX
# VCPKG_TARGET_IS_IOS
# VCPKG_TARGET_IS_ANDROID
# VCPKG_TARGET_IS_FREEBSD
# VCPKG_TARGET_IS_OPENBSD
# VCPKG_TARGET_IS_EMSCRIPTEN