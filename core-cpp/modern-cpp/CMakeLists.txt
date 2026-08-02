# C++ 11
create_cxx_executable(thread_safe_task_queue 11
    cpp11/thread_safe_task_queue.cpp
)
create_cxx_executable(variadic_template_csv_parser 11
    cpp11/variadic_template_csv_parser.cpp
)
create_cxx_executable(smart_ptr_object_pool 11
    cpp11/smart_ptr_object_pool.cpp
)
create_cxx_executable(lambda_based_signal_slots 11
    cpp11/lambda_based_signal_slots.cpp
)
create_cxx_executable(compile_time_math_library 11
    cpp11/compile_time_math_library.hpp
    cpp11/compile_time_math_library.cpp
)

# C++ 14
create_cxx_executable(generic_memoizer 14
    cpp14/generic_memoizer.cpp
)

create_cxx_executable(compile-time_int_seq_utilities 14
    cpp14/compile-time_int_seq_utilities.cpp
)