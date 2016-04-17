#include "list_graph.hpp"

#include <algorithm>
#include <iterator>

namespace graph {
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

		Node Graph::operator[](size_t nodeId) {
			return Node(nodeId, connections);
		}

		ConstNode Graph::operator[](size_t nodeId) const {
			return ConstNode(nodeId, connections);
		}

		bool Graph::operator==(Graph const& other) const {
			return connections == other.connections;
		}

		bool Graph::operator!=(Graph const& other) const {
			return connections != other.connections;
		}
	}
}
