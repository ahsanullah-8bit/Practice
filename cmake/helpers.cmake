
# CXX
function(create_cxx_executable target standard)
    add_executable(${target} ${ARGN})

    set_target_properties(${target} PROPERTIES 
        CXX_STANDARD ${standard}
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )
    target_compile_options(${target} PRIVATE
        $<$<COMPILE_LANG_AND_ID:C,GNU,Clang>:-pedantic-errors>

        $<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->
    )
endfunction()

function(create_c_executable target standard)
    add_executable(${target} ${ARGN})

    set_target_properties(${target} PROPERTIES 
        C_STANDARD ${standard}
        C_STANDARD_REQUIRED ON
        C_EXTENSIONS OFF
    )
    target_compile_options(${target} PRIVATE
        $<$<COMPILE_LANG_AND_ID:C,GNU,Clang>:-pedantic-errors>
    )
endfunction()