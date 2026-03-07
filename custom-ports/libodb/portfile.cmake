include(CMakePackageConfigHelpers)

set(VCPKG_POLICY_SKIP_COPYRIGHT_CHECK enabled)

# TODO: Reason we're not using the prebuilt binaries 
if (VCPKG_TARGET_IS_WINDOWS)
    set(ODB_URL "https://github.com/ahsanullah-8bit/APSS/releases/download/v0.1/libodb-2.5.0-release.zip")
    set(ODB_URL_DEBUG "https://github.com/ahsanullah-8bit/APSS/releases/download/v0.1/libodb-2.5.0-debug.zip")
elseif(VCPKG_TARGET_IS_LINUX)
    set(ODB_URL "")
    set(ODB_URL_DEBUG "") # SEE SECTION 1 (below) FOR MORE DETAILS
endif()

vcpkg_download_distfile(ARCHIVE
    URLS ${ODB_URL}
    FILENAME "libodb-2.5.0.tar.gz"
    SHA512 e4acf052f0ee4ce825a5775464c8f89da1426934691e47f7c1a7a7e73959731a073ce4a3aaad99b1b44baf5cd93c3a5d542a6df229ee138c2d2ab635a5966d7e
)
vcpkg_download_distfile(ARCHIVE_DEBUG
    URLS ${ODB_URL_DEBUG}
    FILENAME "libodb-2.5.0-debug.tar.gz"
    SHA512 e291436b14fbc92cfd19b746ae72388bd8741bb275db893912b01f1706d430d6d29de3f78bea6227e6e0afc0c99a9e3e6eed81e3ece95a983cb19cc63a09f2ac
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
    vcpkg_replace_string("${PC_FILE}" "\\\\" "/" IGNORE_UNCHANGED)
endforeach(PC_FILE IN LISTS PC_FILES)
  
# vcpkg handles debug differently. It creates a debug folder
#   inside the CURRENT_PACKAGES_DIR and moves the bin and lib there
#   while sharing the same include and share folders. So, we
#   need to update the paths to one folder above.
foreach(PC_FILE IN LISTS PC_FILES_DEBUG)
    vcpkg_replace_string("${PC_FILE}" "/../../include" "/../../../include" REGEX IGNORE_UNCHANGED)
endforeach(PC_FILE IN LISTS PC_FILES_DEBUG)

# Section 1: Prebuilt binaries for Windows
# https://codesynthesis.com/download/odb/2.5.0/windows/windows10/x86_64/libodb-2.5.0-x86_64-windows10-msvc17.10-debug.zip
# https://codesynthesis.com/download/odb/2.5.0/windows/windows10/x86_64/libodb-2.5.0-x86_64-windows10-msvc17.10.zip
# https://codesynthesis.com/download/odb/2.5.0/windows/windows10/x86_64/libodb-sqlite-2.5.0-x86_64-windows10-msvc17.10-debug.zip
# https://codesynthesis.com/download/odb/2.5.0/windows/windows10/x86_64/libodb-qt-2.5.0-x86_64-windows10-msvc17.10-debug.zip

# odb 2.5.0
# https://codesynthesis.com/download/odb/2.5.0/windows/windows10/x86_64/odb-2.5.0-x86_64-windows10.zip

# Set the ODB_URL for other platforms if needed.
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