cmake_minimum_required(VERSION 3.28)

find_package(PkgConfig QUIET)

if (PkgConfig_FOUND)
    pkg_check_modules(ODB QUIET libodb libodb-sqlite)

    if (ODB_FOUND)
        message(STATUS "Enabling odb_cpp")

        add_executable(odb_cpp main.cpp models-odb.cxx)

        # message(STATUS "ODB Include Directories: ${ODB_INCLUDE_DIRS}")
        # message(STATUS "ODB Library Directories: ${ODB_LIBRARY_DIRS}")
        # message(STATUS "ODB Libraries: ${ODB_LIBRARIES}")

        target_include_directories(odb_cpp PRIVATE ${ODB_INCLUDE_DIRS})
        target_link_directories(odb_cpp PRIVATE ${ODB_LIBRARY_DIRS})
        target_link_libraries(odb_cpp PRIVATE ${ODB_LIBRARIES})
    else()
        message(STATUS "ODB not found, disabling odb_cpp")
    endif()
else()
    message(STATUS "PkgConfig not found, disabling odb_cpp")
endif()