find_package(OpenCV CONFIG QUIET)
find_package(Clipper2 CONFIG QUIET)

if (OpenCV_FOUND AND Clipper2_FOUND)
    message(STATUS "Enabling ocr_diff_bin")

    add_executable(ocr_diff_bin main.cpp dbpostprocess.hpp)

    target_include_directories(ocr_diff_bin PRIVATE ${OpenCV_INCLUDE_DIRS})
    target_link_directories(ocr_diff_bin PRIVATE ${OpenCV_LIBRARY_DIRS})
    target_link_libraries(ocr_diff_bin PRIVATE 
        ${OpenCV_LIBS}
        Clipper2::Clipper2
    )

    # Copy asset files after build
    add_custom_command(TARGET ocr_diff_bin POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        ${CMAKE_CURRENT_BINARY_DIR}/assets

        VERBATIM
    )
else()
    if (NOT OpenCV_FOUND)
        message(STATUS "OpenCV not found, disabling ocr_diff_bin")
    elseif(NOT Clipper2_FOUND)
        message(STATUS "Clipper2 not found, disabling ocr_diff_bin")
    endif()
endif()