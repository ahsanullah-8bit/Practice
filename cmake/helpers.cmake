
# CXX
function(create_cxx_executable target standard)
    add_executable(${target} ${ARGN})

    set_target_properties(${target} PROPERTIES 
        CMAKE_CXX_STANDARD ${standard}
        CMAKE_CXX_STANDARD_REQUIRED ON
        CMAKE_CXX_EXTENSIONS OFF
    )
endfunction()

function(create_c_executable target standard)
    add_executable(${target} ${ARGN})

    set_target_properties(${target} PROPERTIES 
        CMAKE_C_STANDARD ${standard}
        CMAKE_C_STANDARD_REQUIRED ON
        CMAKE_C_EXTENSIONS OFF
    )
endfunction()