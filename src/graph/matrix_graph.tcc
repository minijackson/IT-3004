#ifndef _GRAPH_LIB_MATRIX_GRAPH_TCC_
#define _GRAPH_LIB_MATRIX_GRAPH_TCC_

#include "matrix_graph.hpp"

#include <algorithm>
#include <functional>
#include <type_traits>

namespace graph {
	namespace matrix {

		template <typename NodeProperty, typename EdgeProperty>
		template <typename... Arcs>
		Graph<NodeProperty, EdgeProperty>::Graph(size_t nbVertices, Arcs... arcs)
		      : connections(nbVertices, std::vector<bool>(nbVertices, false)) {
			addArcs(arcs...);
		}

		template <typename Candidate>
		using is_convertible_to_arc = std::is_convertible<std::pair<size_t, size_t>, Candidate>;

		template <typename NodeProperty, typename EdgeProperty>
		template <typename... Arcs>
		inline void Graph<NodeProperty, EdgeProperty>::addArcs(std::pair<size_t, size_t> const& arc,
		                                                       Arcs... arcs) {
			static_assert(check_all_v<is_convertible_to_arc, Arcs...>,
			              "The arguments of addArcs must be convertible to std::pair<int, int>");
			connections[arc.first][arc.second] = true;
			addArcs(arcs...);
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
		Graph<NodeProperty, EdgeProperty>::Graph(std::initializer_list<std::pair<size_t, size_t>> arcs) {
			size_t max = 0;
			for(const auto& arc : arcs) {
				if(arc.first > max) {
					max = arc.first;
				}

				if(arc.second > max) {
					max = arc.second;
				}
			}

			connections =
			        std::vector<std::vector<bool>>(max + 1, std::vector<bool>(max + 1, false));

			for(const auto& arc : arcs) {
				addArcs(arc);
			}
		}

		template <typename NodeProperty, typename EdgeProperty>
		void Graph<NodeProperty, EdgeProperty>::addArcs(std::pair<size_t, size_t> const& arc) {
			connections[arc.first][arc.second] = true;
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
		size_t Graph<NodeProperty, EdgeProperty>::getArcsCount() const {
			size_t count = 0;
			for(auto const& subArray : connections) {
				count += std::count(subArray.begin(), subArray.end(), true);
			}
			return count;
		}

		template <typename NodeProperty, typename EdgeProperty>
		Graph<NodeProperty, EdgeProperty> Graph<NodeProperty, EdgeProperty>::symmetric() const {
			Graph<NodeProperty, EdgeProperty> symmetricGraph(connections.size());

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

		template <typename NodeProperty, typename EdgeProperty>
		auto Graph<NodeProperty, EdgeProperty>::operator[](size_t nodeId) -> Node_t {
			return Node_t(nodeId, connections, nodeProperties[nodeId]);
		}

		template <typename NodeProperty, typename EdgeProperty>
		auto Graph<NodeProperty, EdgeProperty>::operator[](size_t nodeId) const -> ConstNode_t {
			return ConstNode_t(nodeId, connections, nodeProperties[nodeId]);
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

			return connections != other.connections;
		}
	}
}

#endif
