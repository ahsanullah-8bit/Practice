set(CMAKE_CXX_STANDARD 23)

find_package(asio CONFIG QUIET)

if (asio_FOUND)
    message(STATUS "Enabling asio_chat_cpp")

    add_executable(asio_chat_cpp_client "customclient.cpp")
    add_executable(asio_chat_cpp_server "customserver.cpp")

    target_include_directories(asio_chat_cpp_client PRIVATE include)
    target_include_directories(asio_chat_cpp_server PRIVATE include)
    target_link_libraries(asio_chat_cpp_client PRIVATE asio::asio)
    target_link_libraries(asio_chat_cpp_server PRIVATE asio::asio)
else()
    message(STATUS "asio not found, disabling asio_chat_cpp")
endif()
