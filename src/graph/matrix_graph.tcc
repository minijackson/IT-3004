#pragma once

#include "matrix_graph.hpp"

#include <algorithm>
#include <functional>
#include <sstream>
#include <type_traits>

namespace graph {
	namespace matrix {

		template <typename NodeProperty, typename EdgeProperty>
		template <typename... Edges>
		Graph<NodeProperty, EdgeProperty>::Graph(Edges... edges)
		      : connections() {
			addEdges(std::move(edges)...);
		}

		template <typename NodeProperty, typename EdgeProperty>
		Graph<NodeProperty, EdgeProperty>::Graph(
		        std::initializer_list<Edge_t> edgeNames)
		      : connections() {
			for(const auto& edge : edgeNames) {
				addEdges(std::move(edge));
			}
		}

		template <typename NodeProperty, typename EdgeProperty>
		bool Graph<NodeProperty, EdgeProperty>::hasNode(std::string const& nodeName) const {
			return static_cast<bool>(nodeNames.count(nodeName));
		}

		template <typename NodeProperty, typename EdgeProperty>
		void Graph<NodeProperty, EdgeProperty>::addNode(std::string const& nodeName,
		                                                NodeProperty property) {
			if(!hasNode(nodeName)) {
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
		void Graph<NodeProperty, EdgeProperty>::addNode(std::string const& nodeName) {
			addNode(nodeName, NodeProperty());
		}

		template <typename NodeProperty, typename EdgeProperty>
		void Graph<NodeProperty, EdgeProperty>::removeNode(ConstNode_t const& node) {

			std::string nodeName = node.getName();

			if(hasNode(nodeName)) {
				size_t nodeId = nodeNames[nodeName];

				nodeNames.erase(node.getName());
				for(auto& node : nodeNames) {
					if(node.second > nodeId) {
						--node.second;
					}
				}

				connections.erase(connections.begin() + nodeId);
				this->eachEdges([this, nodeId](ConstNode_t const& begin, ConstNode_t const& end) {
					if(end.getId() == nodeId) {
						removeEdge((*this)[begin.getName()], (*this)[end.getName()]);
					}
				});

				for(auto& nodeConnections : connections) {
					nodeConnections.erase(nodeConnections.begin() + nodeId);
				}
			}
		}

		template <typename NodeProperty, typename EdgeProperty>
		bool Graph<NodeProperty, EdgeProperty>::hasEdge(ConstNode_t const& begin,
		                                                ConstNode_t const& end) const {
			return static_cast<bool>(edgeProperties.count({begin.getName(), end.getName()}));
		}

		template <typename NodeProperty, typename EdgeProperty>
		void Graph<NodeProperty, EdgeProperty>::addEdges(Edge_t const& edge) {
			std::string start = std::move(std::get<0>(edge)), end = std::move(std::get<1>(edge));
			size_t beginId = getId(start), endId = getId(end);
			connections[beginId][endId]  = true;
			edgeProperties[{start, end}] = std::get<EdgeProperty>(edge);
		}

		template <typename NodeProperty, typename EdgeProperty>
		template <typename... Edges>
		inline void Graph<NodeProperty, EdgeProperty>::addEdges(Edge_t const& edge,
		                                                        Edges const&... edges) {
			addEdges(edge);
			addEdges(edges...);
		}

		template <typename NodeProperty, typename EdgeProperty>
		inline void Graph<NodeProperty, EdgeProperty>::addEdges(
		        std::initializer_list<Edge_t> edges) {
			for(auto& edge : edges) {
				addEdges(std::move(edge));
			}
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
		void Graph<NodeProperty, EdgeProperty>::removeEdge(ConstNode_t const& begin,
		                                                   ConstNode_t const& end) {
			size_t beginId = begin.getId(), endId = end.getId();

			if(!connections[beginId][endId]) {
				std::ostringstream errMsg;
				errMsg << "No such edge if the graph: (" << begin.getName() << ", " << end.getName()
				       << "), with id: (" << begin.getId() << ", " << end.getId() << ").";
				throw std::out_of_range(errMsg.str());
			}

			connections[beginId][endId] = false;

			edgeProperties.erase({begin.getName(), end.getName()});
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
				functor(std::move((*this)[node.first]));
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
						functor(std::move((*this)[begin.first]), std::move((*this)[end.first]));
					}
				}
			}
		}

		template <typename NodeProperty, typename EdgeProperty>
		template <typename Functor>
		void Graph<NodeProperty, EdgeProperty>::eachAdjacents(ConstNode_t const& vertex,
		                                                      Functor&& functor) const {
			static_assert(std::is_convertible<Functor, std::function<void(ConstNode_t)>>::value,
			              "The function must be convertible to a function of type void(ConstNode)");
			size_t vertexId = vertex.getId();

			for(auto const& vertex : nodeNames) {
				if(connections[vertexId][vertex.second]) {
					functor(std::move((*this)[vertex.first]));
				}
			}
		}

		template <typename NodeProperty, typename EdgeProperty>
		size_t Graph<NodeProperty, EdgeProperty>::getId(std::string const& name) {
			if(!hasNode(name)) {
				addNode(name);
			}
			return nodeNames[name];
		}

		template <typename NodeProperty, typename EdgeProperty>
		size_t Graph<NodeProperty, EdgeProperty>::getId(std::string const& name) const {
			return nodeNames.at(name);
		}

		template <typename NodeProperty, typename EdgeProperty>
		auto Graph<NodeProperty, EdgeProperty>::begin() -> Node_t {
			return (*this)[nodeNames.begin()->first];
		}

		template <typename NodeProperty, typename EdgeProperty>
		auto Graph<NodeProperty, EdgeProperty>::begin() const -> ConstNode_t {
			return (*this)[nodeNames.begin()->first];
		}

		template <typename NodeProperty, typename EdgeProperty>
		auto Graph<NodeProperty, EdgeProperty>::operator[](std::string const& nodeName) -> Node_t {
			size_t nodeId = getId(nodeName);
			return Node_t(nodeId, connections[nodeId], nodeName, nodeProperties);
		}

		template <typename NodeProperty, typename EdgeProperty>
		auto Graph<NodeProperty, EdgeProperty>::operator[](std::string const& nodeName) const
		        -> ConstNode_t {
			size_t nodeId = getId(nodeName);
			return ConstNode_t(nodeId, connections[nodeId], nodeName, nodeProperties);
		}

		template <typename NodeProperty, typename EdgeProperty>
		template <typename OtherNodeProperty, typename OtherEdgeProperty>
		bool Graph<NodeProperty, EdgeProperty>::operator==(
		        Graph<OtherNodeProperty, OtherEdgeProperty> const& other) const {

			// Compare node names and number of nodes
			if(!std::equal(nodeNames.begin(),
			               nodeNames.end(),
			               other.nodeNames.begin(),
			               other.nodeNames.end(),
			               [](std::pair<std::string, size_t> const& a,
			                  std::pair<std::string, size_t> const& b) {
				               return a.first == b.first;
				           })) {
				return false;
			}

			return edgeProperties == other.edgeProperties;
		}

		template <typename NodeProperty, typename EdgeProperty>
		template <typename OtherNodeProperty, typename OtherEdgeProperty>
		bool Graph<NodeProperty, EdgeProperty>::operator!=(
		        Graph<OtherNodeProperty, OtherEdgeProperty> const& other) const {
			return !(*this == other);
		}
	}
}
