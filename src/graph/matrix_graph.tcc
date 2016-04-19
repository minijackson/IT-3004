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
		Graph<NodeProperty, EdgeProperty>::Graph(size_t nbVertices, Edges... edges)
		      : connections(nbVertices, std::vector<bool>(nbVertices, false)) {
			addEdges(edges...);
		}

		template <typename Candidate>
		using is_convertible_to_edge = std::is_convertible<std::pair<size_t, size_t>, Candidate>;

		template <typename NodeProperty, typename EdgeProperty>
		template <typename... Edges>
		inline void Graph<NodeProperty, EdgeProperty>::addEdges(
		        std::pair<size_t, size_t> const& edge,
		        Edges... edges) {
			static_assert(check_all_v<is_convertible_to_edge, Edges...>,
			              "The arguments of addEdges must be convertible to std::pair<int, int>");
			addEdges(edge);
			addEdges(edges...);
		}

		template <typename NodeProperty, typename EdgeProperty>
		void Graph<NodeProperty, EdgeProperty>::addEdges(std::pair<size_t, size_t> const& edge) {
			connections[edge.first][edge.second] = true;
			edgeProperties[{edge.first, edge.second}] = EdgeProperty();
		}

		template <typename NodeProperty, typename EdgeProperty>
		void Graph<NodeProperty, EdgeProperty>::addEdge(ConstNode_t const& begin,
		                                                ConstNode_t const& end,
		                                                EdgeProperty property) {
			size_t beginId = begin.getId(), endId = end.getId();
			connections[beginId][endId] = true;
			edgeProperties[{beginId, endId}] = std::move(property);
		}

		template <typename NodeProperty, typename EdgeProperty>
		void Graph<NodeProperty, EdgeProperty>::addEdge(ConstNode_t const& begin,
		                                                ConstNode_t const& end) {
			addEdge(begin, end, EdgeProperty());
		}

		template <typename NodeProperty, typename EdgeProperty>
		std::decay_t<EdgeProperty> Graph<NodeProperty, EdgeProperty>::getEdgeProperty(
		        ConstNode_t const& begin,
		        ConstNode_t const& end) const {
			// Use .at() instead of operator[] because the method .at() throw if the key does not
			// exists, instead of inserting a new key, allowing "getEdgeProperty" to be const.
			return edgeProperties.at({begin.getId(), end.getId()});
		}

		template <typename NodeProperty, typename EdgeProperty>
		void Graph<NodeProperty, EdgeProperty>::setEdgeProperty(ConstNode_t const& begin,
		                                                        ConstNode_t const& end,
		                                                        EdgeProperty property) {
			edgeProperties[{begin.getId(), end.getId()}] = std::move(property);
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

			for(size_t i = 0; i < connections.size(); ++i) {
				functor((*this)[i]);
			}
		}

		template <typename NodeProperty, typename EdgeProperty>
		template <typename Functor>
		void Graph<NodeProperty, EdgeProperty>::eachEdges(Functor&& functor) const {
			static_assert(std::is_convertible<Functor,
			                                  std::function<void(ConstNode_t, ConstNode_t)>>::value,
			              "The function must be convertible to a function of type void(ConstNode, "
			              "ConstNode)");

			for(size_t i = 0; i < connections.size(); ++i) {
				for(size_t j = 0; j < connections.size(); ++j) {
					if(connections[i][j]) {
						functor((*this)[i], (*this)[j]);
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

			for(size_t i = 0; i < connections.size(); ++i) {
				if(connections[vertexId][i]) {
					functor((*this)[i]);
				}
			}
		}

		template <typename NodeProperty, typename EdgeProperty>
		Graph<NodeProperty, EdgeProperty>::Graph(size_t nbVertices)
		      : connections(nbVertices, std::vector<bool>(nbVertices, false)) {}

		template <typename NodeProperty, typename EdgeProperty>
		Graph<NodeProperty, EdgeProperty>::Graph(
		        std::initializer_list<std::pair<size_t, size_t>> edges) {
			size_t max = 0;
			for(const auto& edge : edges) {
				if(edge.first > max) {
					max = edge.first;
				}

				if(edge.second > max) {
					max = edge.second;
				}
			}

			connections =
			        std::vector<std::vector<bool>>(max + 1, std::vector<bool>(max + 1, false));

			for(const auto& edge : edges) {
				addEdges(edge);
			}
		}

		template <typename NodeProperty, typename EdgeProperty>
		auto Graph<NodeProperty, EdgeProperty>::operator[](size_t nodeId) -> Node_t {
			return Node_t(nodeId, connections[nodeId], nodeProperties[nodeId]);
		}

		template <typename NodeProperty, typename EdgeProperty>
		auto Graph<NodeProperty, EdgeProperty>::operator[](size_t nodeId) const -> ConstNode_t {
			return ConstNode_t(nodeId, connections[nodeId], nodeProperties[nodeId]);
		}

		template <typename NodeProperty, typename EdgeProperty>
		template <typename OtherNodeProperty, typename OtherEdgeProperty>
		bool Graph<NodeProperty, EdgeProperty>::operator==(
		        Graph<OtherNodeProperty, OtherEdgeProperty> const& other) const {

			return connections == other.connections && nodeProperties == other.nodeProperties &&
			       edgeProperties == other.edgeProperties;
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
