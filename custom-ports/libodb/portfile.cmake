include(CMakePackageConfigHelpers)

set(VCPKG_POLICY_SKIP_COPYRIGHT_CHECK enabled)

# Reason we're not using the prebuilt binaries provided by the 
# ODB maintainers is because, the last I used them, I was facing
# issues with the naming convention used in the debug binaries.
# pkg-config just couldn't find them.
if (VCPKG_TARGET_IS_WINDOWS)
    set(ODB_URL "https://github.com/ahsanullah-8bit/Practice/releases/download/v0.0/libodb-2.5.0-libodb-qt-sqlite-win64.zip")
    set(ODB_SHA512 "abf7ec6ccef2e7b85aad510c221dac50a7ee0a410aedb94ff8158c14f8a07320e9b341659fe9f26264cf6dd75d278a5ecd8adf86b6eaed1004ba47cd835e665c")
elseif(VCPKG_TARGET_IS_LINUX)
    if (EXISTS "/etc/os-release")
        file(READ "/etc/os-release" OS_RELEASE_INFO)
        if (OS_RELEASE_INFO MATCHES "Ubuntu")
            # Ubuntu specific
            message(STATUS "Configuring specifically for Ubuntu")
            set(ODB_URL "https://github.com/ahsanullah-8bit/Practice/releases/download/v0.0/libodb-2.5.0-libodb-qt-sqlite-ubuntu-24.04.zip")
            set(ODB_SHA512 "59df1796e4acf44a899ac0aa220fdc8c992aa5042b362969191f0143429c522c780267561ba94d9fd36f4931047e467a95f410c3310494420b423b93b9f83241") # SEE SECTION 1 (below) FOR MORE DETAILS
        endif()
    endif()
endif()

vcpkg_download_distfile(ARCHIVE
    URLS ${ODB_URL}
    FILENAME "libodb-2.5.0.tar.gz"
    SHA512 ${ODB_SHA512}
)

vcpkg_extract_source_archive(
    SOURCE_PATH
    ARCHIVE "${ARCHIVE}"
)

file(INSTALL "${SOURCE_PATH}/include/" DESTINATION "${CURRENT_PACKAGES_DIR}/include")
file(INSTALL "${SOURCE_PATH}/lib/" DESTINATION "${CURRENT_PACKAGES_DIR}/lib")
file(INSTALL "${SOURCE_PATH}/lib/pkgconfig/" DESTINATION "${CURRENT_PACKAGES_DIR}/lib/pkgconfig")
if(EXISTS "${SOURCE_PATH}/debug/lib")
    file(INSTALL "${SOURCE_PATH}/debug/lib/" DESTINATION "${CURRENT_PACKAGES_DIR}/debug/lib")
    file(INSTALL "${SOURCE_PATH}/debug/lib/pkgconfig/" DESTINATION "${CURRENT_PACKAGES_DIR}/debug/lib/pkgconfig")
endif()

if(EXISTS "${SOURCE_PATH}/bin/")
    file(INSTALL "${SOURCE_PATH}/bin/" DESTINATION "${CURRENT_PACKAGES_DIR}/bin")
endif()
if(EXISTS "${SOURCE_PATH}/debug/bin/")
    file(INSTALL "${SOURCE_PATH}/debug/bin/" DESTINATION "${CURRENT_PACKAGES_DIR}/debug/bin")
endif()

if(EXISTS "${SOURCE_PATH}/share/")
    file(INSTALL "${SOURCE_PATH}/share/" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}")
endif()

# Section 1: Prebuilt binaries for Windows (by the maintainers)
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