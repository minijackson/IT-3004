#pragma once

#include "list_graph.hpp"

#include <functional>
#include <sstream>
#include <type_traits>

namespace graph {
	namespace list {

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
		bool Graph<NodeProperty, EdgeProperty>::hasNode(std::string nodeName) const {
			try {
				nodeNames.at(nodeName);
				return true;
			} catch(std::out_of_range) {
				return false;
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
				nameList.push_back(nodeName);
				connections.push_back(std::list<size_t>());
			}
		}

		template <typename NodeProperty, typename EdgeProperty>
		void Graph<NodeProperty, EdgeProperty>::addNode(std::string nodeName) {
			addNode(nodeName, NodeProperty());
		}

		template <typename NodeProperty, typename EdgeProperty>
		void Graph<NodeProperty, EdgeProperty>::removeNode(ConstNode_t const& node) {

			std::string nodeName = node.getName();
			size_t nodeId        = nodeNames.at(nodeName);

			connections.erase(connections.begin() + nodeId);

			for(size_t beginId = 0; beginId < connections.size(); ++beginId) {

				auto edgeEndIt = connections[beginId].begin();
				while(edgeEndIt != connections[beginId].end()) {
					if(*edgeEndIt == nodeId) {
						edgeProperties.erase({nameList[beginId], nameList[*edgeEndIt]});
						edgeEndIt = connections[beginId].erase(edgeEndIt);
						continue;
					} else if(*edgeEndIt > nodeId) {
						--*edgeEndIt;
					}

					++edgeEndIt;
				}
			}

			nodeNames.erase(node.getName());
			for(auto& node : nodeNames) {
				if(node.second > nodeId) {
					--node.second;
				}
			}

			nameList.erase(nameList.begin() + nodeId);
		}

		template <typename NodeProperty, typename EdgeProperty>
		bool Graph<NodeProperty, EdgeProperty>::hasEdge(ConstNode_t const& begin,
		                                                ConstNode_t const& end) const {
			try {
				edgeProperties.at({begin.getName(), end.getName()});
				return true;
			} catch(std::out_of_range) {
				return false;
			}
		}

		template <typename NodeProperty, typename EdgeProperty>
		void Graph<NodeProperty, EdgeProperty>::addEdges(Edge_t const& edge) {
			std::string start = std::get<0>(edge), end = std::get<1>(edge);
			size_t beginId = getId(start), endId = getId(end);
			connections[beginId].push_back(endId);
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
			connections[beginId].push_back(endId);
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
			auto& nodeConnections = connections[beginId];
			bool found = false;
			for(auto adjacentIt = nodeConnections.begin(); adjacentIt != nodeConnections.end();
			    ++adjacentIt) {
				if(*adjacentIt == endId) {
					found = true;
					nodeConnections.erase(adjacentIt);
					break;
				}
			}

			if(!found) {
				std::ostringstream errMsg;
				errMsg << "No such edge if the graph: (" << begin.getName() << ", " << end.getName()
				       << "), with id: (" << begin.getId() << ", " << end.getId() << ").";
				throw std::out_of_range(errMsg.str());
			}

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
		std::vector<std::list<size_t>> Graph<NodeProperty, EdgeProperty>::getConnections() const {
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
				count += subArray.size();
			}
			return count;
		}

		template <typename NodeProperty, typename EdgeProperty>
		template <typename Functor>
		void Graph<NodeProperty, EdgeProperty>::eachVertices(Functor&& functor) const {
			static_assert(std::is_convertible<Functor, std::function<void(ConstNode_t)>>::value,
			              "The function must be convertible to a function of type void(ConstNode)");

			for(size_t i = 0; i < connections.size(); ++i) {
				functor(std::move((*this)[nameList[i]]));
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
				for(auto j : connections[i]) {
					functor(std::move((*this)[nameList[i]]), std::move((*this)[nameList[j]]));
				}
			}
		}

		template <typename NodeProperty, typename EdgeProperty>
		template <typename Functor>
		void Graph<NodeProperty, EdgeProperty>::eachAdjacents(ConstNode_t const& vertex,
		                                                      Functor&& functor) const {
			static_assert(std::is_convertible<Functor, std::function<void(ConstNode_t)>>::value,
			              "The function must be convertible to a function of type void(ConstNode)");
			for(auto i : connections[vertex.getId()]) {
				functor(std::move((*this)[nameList[i]]));
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
		auto Graph<NodeProperty, EdgeProperty>::begin() -> Node_t {
			return (*this)[nameList[0]];
		}

		template <typename NodeProperty, typename EdgeProperty>
		auto Graph<NodeProperty, EdgeProperty>::begin() const -> ConstNode_t {
			return (*this)[nameList[0]];
		}

		template <typename NodeProperty, typename EdgeProperty>
		auto Graph<NodeProperty, EdgeProperty>::operator[](std::string nodeName) -> Node_t {
			size_t nodeId = getId(nodeName);
			return Node_t(nodeId, connections[nodeId], nodeName, nodeProperties);
		}

		template <typename NodeProperty, typename EdgeProperty>
		auto Graph<NodeProperty, EdgeProperty>::operator[](std::string nodeName) const
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
