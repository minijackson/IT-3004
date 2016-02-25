#include "graph.hpp"

#include <sstream>

std::ostream& operator<<(std::ostream& os, matrix::Graph graph) {
	size_t startVertex = 0;
	for(auto const& subArray : graph.connections) {
		size_t endVertex = 0;
		for(bool connection : subArray) {
			if(connection) {
				os << startVertex << " -> " << endVertex << std::endl;
			}
			++endVertex;
		}
		++startVertex;
	}
	return os;
}

std::string makeDigraph(std::string name, matrix::Graph graph) {
	std::ostringstream result;
	result << "digraph " << name << " {" << std::endl << graph << "}" << std::endl;
	return result.str();
}

std::ostream& operator<<(std::ostream& os, list::Graph graph) {
	size_t startVertex = 0;
	for(auto const& subArray : graph.connections) {
		for(size_t endVertex : subArray) {
			os << startVertex << " -> " << endVertex << std::endl;
		}
		++startVertex;
	}
	return os;
}

std::string makeDigraph(std::string name, list::Graph graph) {
	std::ostringstream result;
	result << "digraph " << name << " {" << std::endl << graph << "}" << std::endl;
	return result.str();
}
