#include "list_graph.hpp"

#include <algorithm>
#include <iterator>

namespace list {

	Graph::Graph(size_t nbVertices)
	      : connections(nbVertices) {}

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

		connections = std::vector<std::list<size_t>>(max + 1);

		for(const auto& arc : arcs) {
			addArcs(arc);
		}
	}

	void Graph::addArcs(std::pair<size_t, size_t> const& arc) {
		connections[arc.first].push_back(arc.second);
	}

	std::vector<std::list<size_t>> Graph::getConnections() const {
		return connections;
	}

	size_t Graph::getVerticesCount() const {
		return connections.size();
	}

	size_t Graph::getArcsCount() const {
		size_t count = 0;
		for(auto const& subArray : connections) {
			count += subArray.size();
		}
		return count;
	}

	Graph Graph::symmetric() const {
		Graph symmetricGraph(connections.size());

		size_t i = 0;
		for(auto const& subArray : connections) {
			for(size_t endVertex : subArray) {
				symmetricGraph.connections[endVertex].push_back(i);
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

			for(size_t endVertex : connections[currentVertex]) {
				if(existPathToVertex.find(endVertex) == existPathToVertex.end()) {
					verticesToCheck.insert(endVertex);
					existPathToVertex.insert(endVertex);
				}
			}
		}

		verticesToCheck            = std::set<size_t>{vertex};
		Graph sym                  = this->symmetric();
		auto const& symConnections = sym.connections;

		while(!verticesToCheck.empty()) {
			size_t currentVertex = *verticesToCheck.begin();
			verticesToCheck.erase(currentVertex);

			for(size_t endVertex : symConnections[currentVertex]) {
				if(existPathToVertexSym.find(endVertex) == existPathToVertexSym.end()) {
					verticesToCheck.insert(endVertex);
					existPathToVertexSym.insert(endVertex);
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

			for(size_t endVertex : connections[currentVertex]) {
				if(component.find(endVertex) == component.end()) {
					verticesToCheck.insert(endVertex);
					component.insert(endVertex);
				}
			}

			for(size_t endVertex : symConnections[currentVertex]) {
				if(component.find(endVertex) == component.end()) {
					verticesToCheck.insert(endVertex);
					component.insert(endVertex);
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
