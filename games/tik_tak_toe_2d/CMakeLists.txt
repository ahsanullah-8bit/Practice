find_package(SFML COMPONENTS Graphics Window System CONFIG QUIET)

if (SFML_FOUND)
    message(STATUS "Enabling tic_tak_toe_2d")

    add_executable(tic_tak_toe_2d
        "main.cpp"
        "src/GameClass.cpp"
        "src/SceneHandler.cpp"
        "src/Renderer.cpp")

    target_include_directories(tic_tak_toe_2d PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(tic_tak_toe_2d PRIVATE SFML::Graphics SFML::Window SFML::System)

    # Custom commands to copy necessary to the source directory
    add_custom_command(TARGET tic_tak_toe_2d POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/assets"
            "${CMAKE_CURRENT_BINARY_DIR}/assets"
        COMMENT "Copying assets from the source directory"
    )
else()
    message(STATUS "SFML not found, disabling tik_tak_toe_2d")
endif()
