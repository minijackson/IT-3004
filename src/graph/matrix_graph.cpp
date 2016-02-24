#include "matrix_graph.hpp"

#include <algorithm>
#include <iterator>

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

	Graph Graph::symmetric() const {
		Graph symmetricGraph(connections.size());

		size_t i = 0;
		for(auto const& subArray : connections) {
			size_t j = 0;
			for(bool connection : subArray) {
				if(connection) {
					symmetricGraph.connections[j][i] = true;
				}
				++j;
			}
			++i;
		}

		return symmetricGraph;
	}

	std::set<size_t> Graph::stronglyConnectedComponent(size_t vertex) const {
		std::set<size_t> verticesToCheck{vertex}, existPathToVertex{vertex},
		  existPathToVertexSym{vertex};

		while(!verticesToCheck.empty()) {
			size_t currentVertex = *verticesToCheck.begin();
			verticesToCheck.erase(currentVertex);

			for(size_t i = 0; i < connections.size(); ++i) {
				if(connections[currentVertex][i] &&
				   (existPathToVertex.find(i) == existPathToVertex.end())) {
					verticesToCheck.insert(i);
					existPathToVertex.insert(i);
				}
			}
		}

		verticesToCheck            = std::set<size_t>{vertex};
		Graph sym                  = this->symmetric();
		auto const& symConnections = sym.connections;

		while(!verticesToCheck.empty()) {
			size_t currentVertex = *verticesToCheck.begin();
			verticesToCheck.erase(currentVertex);

			for(size_t i = 0; i < symConnections.size(); ++i) {
				if(symConnections[currentVertex][i] &&
				   (existPathToVertexSym.find(i) == existPathToVertexSym.end())) {
					verticesToCheck.insert(i);
					existPathToVertexSym.insert(i);
				}
			}
		}

		std::set<size_t> component;

		std::set_intersection(existPathToVertex.begin(),
		                      existPathToVertex.end(),
		                      existPathToVertexSym.begin(),
		                      existPathToVertexSym.end(),
		                      std::inserter(component, component.begin()));
		return component;
	}

	std::set<size_t> Graph::connectedComponent(size_t vertex) const {
		std::set<size_t> verticesToCheck{vertex}, component{vertex};

		Graph sym                  = this->symmetric();
		auto const& symConnections = sym.connections;

		while(!verticesToCheck.empty()) {
			size_t currentVertex = *verticesToCheck.begin();
			verticesToCheck.erase(currentVertex);

			for(size_t i = 0; i < connections.size(); ++i) {
				if(connections[currentVertex][i] && (component.find(i) == component.end())) {
					verticesToCheck.insert(i);
					component.insert(i);
				}
			}

			for(size_t i = 0; i < symConnections.size(); ++i) {
				if(symConnections[currentVertex][i] && (component.find(i) == component.end())) {
					verticesToCheck.insert(i);
					component.insert(i);
				}
			}
		}

		return component;
	}

	Node Graph::operator[](size_t nodeId) {
		return Node(nodeId, connections);
	}

	bool Graph::operator==(Graph const& other) const {
		return connections == other.connections;
	}

	bool Graph::operator!=(Graph const& other) const {
		return connections != other.connections;
	}

}
