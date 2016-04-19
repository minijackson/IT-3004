#include "printing.hpp"
#include "graph.hpp"

#include <sstream>

//template <typename NodeProperty, typename EdgeProperty>
//std::ostream& operator<<(std::ostream& os, graph::list::Graph<NodeProperty, EdgeProperty> graph) {
std::ostream& operator<<(std::ostream& os, graph::list::Graph graph) {
	size_t startVertex = 0;
	for(auto const& subArray : graph.connections) {
		for(size_t endVertex : subArray) {
			os << startVertex << " -> " << endVertex << std::endl;
		}
		++startVertex;
	}
	return os;
}

//template <typename NodeProperty, typename EdgeProperty>
//std::string makeDigraph(std::string name, graph::list::Graph<NodeProperty, EdgeProperty> graph) {
std::string makeDigraph(std::string name, graph::list::Graph graph) {
	std::ostringstream result;
	result << "digraph " << name << " {" << std::endl << graph << "}" << std::endl;
	return result.str();
}
