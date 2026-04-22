cmake_minimum_required(VERSION 3.28)

find_package(tbb CONFIG REQUIRED)

add_executable(tbb_flow_graph main.cpp)
target_link_libraries(tbb_flow_graph PRIVATE TBB::tbb)