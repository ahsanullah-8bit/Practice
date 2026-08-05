cmake_minimum_required (VERSION 3.15)

project (Tic_Tak_Toe_2D VERSION 1.0)

# set(SFML_DIR "C:\\packages\\SFML-2.6.0")

find_package(SFML COMPONENTS  graphics window system QUIET)

include(FetchContent)
if (NOT SFML_FOUND)
    message(STATUS "SFML not found, fetching SFML from GitHub...")

    FetchContent_Declare(
        sfml
        GIT_REPOSITORY "https://github.com/SFML/SFML.git"
        GIT_TAG 2.6.0
        SOURCE_DIR "${CMAKE_SOURCE_DIR}/_deps/sfml-src"
        UPDATE_DISCONNECTED ON
        QUIET OFF
    )

    # FetchContent_Populate
    FetchContent_MakeAvailable(sfml)
endif()

# Add source to this project's executable.
add_executable (Tic_Tak_Toe_2D
    "main.cpp"
    "src/GameClass.cpp"
    "src/SceneHandler.cpp"
    "src/Renderer.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Tic_Tak_Toe_2D PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

# Link SFML
target_link_libraries(Tic_Tak_Toe_2D PRIVATE sfml-graphics sfml-window sfml-system)
target_include_directories(Tic_Tak_Toe_2D PRIVATE ${SFML_INCLUDE_PATH} includes)

# Custom commands to copy necessary to the source directory
add_custom_command(TARGET Tic_Tak_Toe_2D POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/resources"
        "${CMAKE_BINARY_DIR}/resources"
    COMMENT "Copying resources from the source directory"
)

# Add a custom target to depend on the custom command
add_custom_target(CopyFiles ALL DEPENDS Tic_Tak_Toe_2D)
# target_link_libraries(Tic_Tak_Toe_2D PRIVATE ${SFML_LIBRARIES})
