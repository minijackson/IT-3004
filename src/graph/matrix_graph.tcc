#ifndef _GRAPH_LIB_MATRIX_GRAPH_TCC_
#define _GRAPH_LIB_MATRIX_GRAPH_TCC_

#include "matrix_graph.hpp"

#include <algorithm>
#include <functional>
#include <type_traits>

namespace graph {
	namespace matrix {

		template <typename NodeProperty, typename EdgeProperty>
		template <typename... Edges>
		Graph<NodeProperty, EdgeProperty>::Graph(Edges... edges)
		      : connections() {
			addEdges(edges...);
		}

		template <typename Candidate>
		using is_convertible_to_edge =
		        std::is_convertible<std::pair<std::string, std::string>, Candidate>;

		template <typename NodeProperty, typename EdgeProperty>
		template <typename... Edges>
		inline void Graph<NodeProperty, EdgeProperty>::addEdges(
		        std::pair<std::string, std::string> const& edge,
		        Edges... edges) {
			static_assert(check_all_v<is_convertible_to_edge, Edges...>,
			              "The arguments of addEdges must be convertible to "
			              "std::pair<std::string, std::string>");
			addEdges(edge);
			addEdges(edges...);
		}

		template <typename NodeProperty, typename EdgeProperty>
		Graph<NodeProperty, EdgeProperty>::Graph(
		        std::initializer_list<std::pair<std::string, std::string>> edgeNames)
		      : connections() {
			for(const auto& edge : edgeNames) {
				addEdges(edge);
			}
		}

		template <typename NodeProperty, typename EdgeProperty>
		void Graph<NodeProperty, EdgeProperty>::addNode(std::string nodeName,
		                                                NodeProperty property) {
			try {
				nodeNames.at(nodeName);
			} catch(std::out_of_range) {
				size_t nodeId       = connections.size();
				nodeNames[nodeName] = nodeId;
				nodeProperties.push_back(property);
				for(auto& eachConnections : connections) {
					eachConnections.push_back(false);
				}
				connections.push_back(std::vector<bool>(nodeId + 1, false));
			}
		}

		template <typename NodeProperty, typename EdgeProperty>
		void Graph<NodeProperty, EdgeProperty>::addNode(std::string nodeName) {
			addNode(nodeName, NodeProperty());
		}

		template <typename NodeProperty, typename EdgeProperty>
		void Graph<NodeProperty, EdgeProperty>::addEdges(
		        std::pair<std::string, std::string> const& edge) {
			size_t beginId = getId(edge.first), endId = getId(edge.second);
			connections[beginId][endId] = true;
			edgeProperties[{edge.first, edge.second}] = EdgeProperty();
		}

		template <typename NodeProperty, typename EdgeProperty>
		void Graph<NodeProperty, EdgeProperty>::connect(ConstNode_t const& begin,
		                                                ConstNode_t const& end,
		                                                EdgeProperty property) {
			size_t beginId = begin.getId(), endId = end.getId();
			connections[beginId][endId] = true;
			edgeProperties[{begin.getName(), end.getName()}] = std::move(property);
		}

		template <typename NodeProperty, typename EdgeProperty>
		void Graph<NodeProperty, EdgeProperty>::connect(ConstNode_t const& begin,
		                                                ConstNode_t const& end) {
			connect(begin, end, EdgeProperty());
		}

		template <typename NodeProperty, typename EdgeProperty>
		EdgeProperty Graph<NodeProperty, EdgeProperty>::getEdgeProperty(
		        ConstNode_t const& begin,
		        ConstNode_t const& end) const {
			// Use .at() instead of operator[] because the method .at() throw if the key does not
			// exists, instead of inserting a new key, allowing "getEdgeProperty" to be const.
			return edgeProperties.at({begin.getName(), end.getName()});
		}

		template <typename NodeProperty, typename EdgeProperty>
		void Graph<NodeProperty, EdgeProperty>::setEdgeProperty(ConstNode_t const& begin,
		                                                        ConstNode_t const& end,
		                                                        EdgeProperty property) {
			edgeProperties.at({begin.getName(), end.getName()}) = std::move(property);
		}

		template <typename NodeProperty, typename EdgeProperty>
		std::vector<std::vector<bool>> Graph<NodeProperty, EdgeProperty>::getConnections() const {
			return connections;
		}

		template <typename NodeProperty, typename EdgeProperty>
		size_t Graph<NodeProperty, EdgeProperty>::getVerticesCount() const {
			return connections.size();
		}

		template <typename NodeProperty, typename EdgeProperty>
		size_t Graph<NodeProperty, EdgeProperty>::getEdgesCount() const {
			size_t count = 0;
			for(auto const& subArray : connections) {
				count += std::count(subArray.begin(), subArray.end(), true);
			}
			return count;
		}

		template <typename NodeProperty, typename EdgeProperty>
		template <typename Functor>
		void Graph<NodeProperty, EdgeProperty>::eachVertices(Functor&& functor) const {
			static_assert(std::is_convertible<Functor, std::function<void(ConstNode_t)>>::value,
			              "The function must be convertible to a function of type void(ConstNode)");

			for(auto const& node : nodeNames) {
				functor((*this)[node.first]);
			}
		}

		template <typename NodeProperty, typename EdgeProperty>
		template <typename Functor>
		void Graph<NodeProperty, EdgeProperty>::eachEdges(Functor&& functor) const {
			static_assert(std::is_convertible<Functor,
			                                  std::function<void(ConstNode_t, ConstNode_t)>>::value,
			              "The function must be convertible to a function of type void(ConstNode, "
			              "ConstNode)");

			for(auto const& begin: nodeNames) {
				for(auto const& end: nodeNames) {
					if(connections[begin.second][end.second]) {
						functor((*this)[begin.first], (*this)[end.first]);
					}
				}
			}
		}

		template <typename NodeProperty, typename EdgeProperty>
		template <typename Functor>
		void Graph<NodeProperty, EdgeProperty>::eachAdjacents(ConstNode_t vertex,
		                                                      Functor&& functor) const {
			static_assert(std::is_convertible<Functor, std::function<void(ConstNode_t)>>::value,
			              "The function must be convertible to a function of type void(ConstNode)");
			size_t vertexId = vertex.getId();

			for(auto const& vertex : nodeNames) {
				if(connections[vertexId][vertex.second]) {
					functor((*this)[vertex.first]);
				}
			}
		}

		template <typename NodeProperty, typename EdgeProperty>
		size_t Graph<NodeProperty, EdgeProperty>::getId(std::string name) {
			try {
				return nodeNames.at(name);
			} catch(std::out_of_range) {
				addNode(name);
				return nodeNames.at(name);
			}
		}

		template <typename NodeProperty, typename EdgeProperty>
		size_t Graph<NodeProperty, EdgeProperty>::getId(std::string name) const {
			return nodeNames.at(name);
		}

		template <typename NodeProperty, typename EdgeProperty>
		auto Graph<NodeProperty, EdgeProperty>::operator[](std::string nodeName) -> Node_t {
			size_t nodeId = getId(nodeName);
			return Node_t(nodeId, connections[nodeId], nodeName, nodeProperties[nodeId]);
		}

		template <typename NodeProperty, typename EdgeProperty>
		auto Graph<NodeProperty, EdgeProperty>::operator[](std::string nodeName) const
		        -> ConstNode_t {
			size_t nodeId = getId(nodeName);
			return ConstNode_t(nodeId, connections[nodeId], nodeName, nodeProperties[nodeId]);
		}

		template <typename NodeProperty, typename EdgeProperty>
		template <typename OtherNodeProperty, typename OtherEdgeProperty>
		bool Graph<NodeProperty, EdgeProperty>::operator==(
		        Graph<OtherNodeProperty, OtherEdgeProperty> const& other) const {

			if(nodeNames.size() != other.nodeNames.size()) {
				return false;
			}

			try {
				bool failure = false;
				for(const auto& begin : nodeNames) {
					for(const auto& end : nodeNames) {
						size_t otherBeginId = other.nodeNames.at(begin.first),
						       otherEndId = other.nodeNames.at(end.first);
						if(connections[begin.second][end.second] !=
						   other.connections[otherBeginId][otherEndId]) {
							failure = true;
							goto loop_end;
						}
					}
				}
			loop_end:
				return !failure;
				return connections == other.connections && nodeProperties == other.nodeProperties &&
				       edgeProperties == other.edgeProperties;
			} catch(std::out_of_range) {
				return false;
			}
		}

		template <typename NodeProperty, typename EdgeProperty>
		template <typename OtherNodeProperty, typename OtherEdgeProperty>
		bool Graph<NodeProperty, EdgeProperty>::operator!=(
		        Graph<OtherNodeProperty, OtherEdgeProperty> const& other) const {
			return !(*this == other);
		}
	}
}

#endif
