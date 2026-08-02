cmake_minimum_required(VERSION 3.28)

pkg_check_modules(ODB REQUIRED libodb libodb-qt libodb-sqlite)

add_executable(odb_cpp main.cpp models-odb.cxx)

message(STATUS "ODB Include Directories: ${ODB_INCLUDE_DIRS}")
message(STATUS "ODB Library Directories: ${ODB_LIBRARY_DIRS}")
message(STATUS "ODB Libraries: ${ODB_LIBRARIES}")

target_include_directories(odb_cpp PRIVATE ${ODB_INCLUDE_DIRS})
target_link_directories(odb_cpp PRIVATE ${ODB_LIBRARY_DIRS})
target_link_libraries(odb_cpp PRIVATE ${ODB_LIBRARIES})