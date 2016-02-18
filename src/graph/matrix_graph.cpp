#include "matrix_graph.hpp"

#include <algorithm>

namespace matrix {

	Graph::Graph(size_t nbVertices)
	      : connections(nbVertices, std::vector<bool>(nbVertices, false)) {}

	Graph::Graph(std::initializer_list<std::pair<size_t, size_t>> arcs) {
		size_t max = 0;
		for(const auto& arc : arcs) {
			if(arc.first > max) {
				max = arc.first;
			}

			if(arc.second > max) {
				max = arc.second;
			}
		}

		connections = std::vector<std::vector<bool>>(max + 1, std::vector<bool>(max + 1, false));

		for(const auto& arc : arcs) {
			addArcs(arc);
		}
	}

	void Graph::addArcs(std::pair<size_t, size_t> const& arc) {
		connections[arc.first][arc.second] = true;
	}

	std::vector<std::vector<bool>> Graph::getConnections() const {
		return connections;
	}

	size_t Graph::getVerticesCount() const {
		return connections.size();
	}

	size_t Graph::getArcsCount() const {
		size_t count = 0;
		for(auto const& subArray : connections) {
			count += std::count(subArray.begin(), subArray.end(), true);
		}
		return count;
	}

	std::set<size_t> Graph::stronglyConnectedComponent(size_t vertex) const {
		std::set<size_t> component{vertex};

		return component;
	}

	Node Graph::operator[](size_t nodeId) {
		return Node(nodeId, connections);
	}

}
