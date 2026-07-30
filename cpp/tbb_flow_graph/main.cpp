#include <cstddef>
#include <iostream>
#include <oneapi/tbb/flow_graph.h>
#include <ostream>
#include <string>
#include <tbb/tbb.h>
#include <tuple>


size_t square_cube(int N, size_t concurrency) {
    size_t result = 0;

    tbb::flow::graph g;
    tbb::flow::broadcast_node<size_t> bn(g);

    tbb::flow::function_node<size_t, size_t> squarer(g, concurrency, [] (size_t v) -> size_t { return v * v; });
    tbb::flow::function_node<size_t, size_t> cuber(g, concurrency, [] (size_t v) -> size_t { return v * v * v; });

    tbb::flow::join_node<std::tuple<size_t, size_t>, tbb::flow::queueing> sc_join(g);

    tbb::flow::function_node<std::tuple<size_t,size_t>> summer(g, tbb::flow::serial, [&result] (std::tuple<size_t, size_t> t) { result += std::get<0>(t) + std::get<1>(t); });

    tbb::flow::make_edge(bn, squarer);
    tbb::flow::make_edge(bn, cuber);

    tbb::flow::make_edge(squarer, std::get<0>(sc_join.input_ports()));
    tbb::flow::make_edge(cuber, std::get<1>(sc_join.input_ports()));

    tbb::flow::make_edge(sc_join, summer);

    for (int i = 0; i < N; ++i) {
        bn.try_put(i);
    }

    g.wait_for_all();
    return result;
}

int main(int argc, char **argv) 
{
    int N = std::stoi(argv[1]); // try 10000000+, your RAM will love it.
    std::string type = argv[2]; // s or u

    if (type == "s")
        std::cout << square_cube(N, tbb::flow::serial) << std::endl;
    else
        std::cout << square_cube(N, tbb::flow::unlimited) << std::endl;
    
    return 0;
}