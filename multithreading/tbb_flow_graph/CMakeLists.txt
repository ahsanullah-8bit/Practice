find_package(TBB CONFIG QUIET)

if (TBB_FOUND)
    add_executable(tbb_flow_graph main.cpp)
    target_link_libraries(tbb_flow_graph PRIVATE TBB::tbb)
else()
    message(STATUS "TBB not found, disabling tbb_flow_graph")
endif()