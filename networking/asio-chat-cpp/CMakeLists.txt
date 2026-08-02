cmake_minimum_required (VERSION 3.8)

project(Chat)
set(CMAKE_CXX_STANDARD 23)

# Add source to this project's executable.
include_directories(
    "${CMAKE_SOURCE_DIR}/ext/asio/asio/include"
    "${CMAKE_SOURCE_DIR}/includes"
)

add_executable(ClientCLI "customclient.cpp")
add_executable(ServerCLI "customserver.cpp")

if (MINGW)
    # target_link_libraries(ClientCLI ws2_32 wsock32)
    link_libraries(ws2_32 wsock32)
    # link_libraries(ws2_32 wsock32)
endif()
