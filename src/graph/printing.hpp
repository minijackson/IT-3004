#ifndef GRAPH_LIB_PRINTING_HPP
#define GRAPH_LIB_PRINTING_HPP

#include "graph.hpp"

#include <sstream>

template <typename NodeProperty, typename EdgeProperty>
std::ostream& operator<<(std::ostream& os,
                         graph::matrix::Graph<NodeProperty, EdgeProperty> const& graph) {

	using ConstNode = typename graph::matrix::Graph<NodeProperty, EdgeProperty>::ConstNode_t;

	graph.eachEdges([&os](ConstNode start, ConstNode end) {
		os << start.getName() << " -> " << end.getName() << std::endl;
	});

	return os;
}

template <typename NodeProperty, typename EdgeProperty>
std::string makeDigraph(std::string name,
                        graph::matrix::Graph<NodeProperty, EdgeProperty> const& graph) {
	std::ostringstream result;
	result << "digraph " << name << " {" << std::endl << graph << "}" << std::endl;
	return result.str();
}


template <typename NodeProperty, typename EdgeProperty>
std::ostream& operator<<(std::ostream& os,
                         graph::list::Graph<NodeProperty, EdgeProperty> const& graph) {

	using ConstNode = typename graph::list::Graph<NodeProperty, EdgeProperty>::ConstNode_t;

	graph.eachEdges([&os](ConstNode start, ConstNode end) {
		os << start.getName() << " -> " << end.getName() << std::endl;
	});

	return os;
}

template <typename NodeProperty, typename EdgeProperty>
std::string makeDigraph(std::string name,
                        graph::list::Graph<NodeProperty, EdgeProperty> const& graph) {
	std::ostringstream result;
	result << "digraph " << name << " {" << std::endl << graph << "}" << std::endl;
	return result.str();
}

#endif
