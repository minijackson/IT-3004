#pragma once

#include <algorithm>
#include <set>

#include <cstddef>

namespace graph {

	/*! \brief Return the symmetric graph of the current graph.
	 *
	 * \param g The Graph from which to create the symmetric graph.
	 * \return The symmetric graph of the current graph.
	 */
	template <typename Graph>
	Graph symmetric(Graph const& g) {
		std::decay_t<Graph> symmetricGraph;
		using ConstNode = typename Graph::ConstNode_t;

		g.eachEdges([&symmetricGraph, &g](ConstNode begin, ConstNode end) {
			std::string beginName = begin.getName(), endName = end.getName();
			symmetricGraph.addEdges(
			        {end.getName(), begin.getName(), g.getEdgeProperty(begin, end)});
		});

		return symmetricGraph;
	}

	/*! \brief Compute the undirected graph equivalent of a given directed graph.
	 *
	 * \param g The Graph from which we want the undirected graph equivalent.
	 * \return The undirected graph equivalent.
	 */
	template <typename Graph>
	Graph undirected(Graph g) {
		using ConstNode = typename Graph::ConstNode_t;

		Graph symm = symmetric(g);

		symm.eachEdges([&g, &symm](ConstNode begin, ConstNode end) {
			g.connect(g[begin.getName()], g[end.getName()], symm.getEdgeProperty(begin, end));
		});

		return g;
	}

	/*! \brief Get the strongly connected component of a given vertex.
	 *
	 * \param g The graph from which we want the strongly connected component.
	 * \param vertex The vertex from which to compute the strongly connected component.
	 * \return the set of vertices which compose the strongly connected component.
	 */
	template <typename Graph>
	std::set<typename Graph::ConstNode_t> stronglyConnectedComponent(
	        Graph const& g,
	        typename Graph::ConstNode_t vertex) {
		using Node = typename Graph::ConstNode_t;

		std::set<Node> verticesToCheck{vertex}, existPathToVertex{vertex},
		        existPathToVertexSym{vertex};

		while(!verticesToCheck.empty()) {
			auto currentVertex = *verticesToCheck.begin();
			verticesToCheck.erase(currentVertex);

			g.eachAdjacents(currentVertex,
			                [&existPathToVertex, &verticesToCheck](Node i) {
				                if(existPathToVertex.find(i) == existPathToVertex.end()) {
					                verticesToCheck.insert(i);
					                existPathToVertex.insert(i);
				                }
				            });
		}

		Graph sym       = symmetric(g);
		verticesToCheck = std::set<Node>{sym[vertex.getName()]};

		while(!verticesToCheck.empty()) {
			auto currentVertex = *verticesToCheck.begin();
			verticesToCheck.erase(currentVertex);

			sym.eachAdjacents(currentVertex,
			                  [&existPathToVertexSym, &verticesToCheck, &g](Node i) {
				                  if(existPathToVertexSym.find(i) == existPathToVertexSym.end()) {
					                  verticesToCheck.insert(i);
					                  existPathToVertexSym.insert(i);
				                  }
				              });
		}

		std::set<Node> existPathToVertexSymTemp, component;

		for(auto const& node : existPathToVertexSym) {
			existPathToVertexSymTemp.insert(g[node.getName()]);
		}

		std::set_intersection(existPathToVertex.begin(),
		                      existPathToVertex.end(),
		                      existPathToVertexSymTemp.begin(),
		                      existPathToVertexSymTemp.end(),
		                      std::inserter(component, component.begin()));

		return component;
	}

	/*! \brief Get the connected component of a given vertex.
	 *
	 * \param g The graph from which we want the connected component.
	 * \param vertex The vertex from which to compute the connected component.
	 * \return The set of vertices which compose the connected component.
	 */
	template <typename Graph>
	std::set<typename Graph::ConstNode_t> connectedComponent(Graph const& g,
	                                                         typename Graph::ConstNode_t vertex) {
		using Node = typename Graph::ConstNode_t;

		std::set<std::string> verticesToCheck{vertex.getName()}, component{vertex.getName()};

		Graph sym = symmetric(g);

		while(!verticesToCheck.empty()) {
			auto currentVertex = *verticesToCheck.begin();
			verticesToCheck.erase(currentVertex);

			g.eachAdjacents(g[currentVertex],
			                [&component, &verticesToCheck](Node i) {
				                std::string nodeName = i.getName();
				                if(component.find(nodeName) == component.end()) {
					                verticesToCheck.insert(nodeName);
					                component.insert(nodeName);
				                }
				            });

			sym.eachAdjacents(sym[currentVertex],
			                  [&component, &verticesToCheck, &g](Node i) {
				                  std::string nodeName = i.getName();
				                  if(component.find(nodeName) == component.end()) {
					                  verticesToCheck.insert(nodeName);
					                  component.insert(nodeName);
				                  }
				              });
		}

		std::set<Node> nodeComponent;

		for(auto const& nodeName : component) {
			nodeComponent.insert(g[nodeName]);
		}

		return nodeComponent;
	}

	/*! \brief Get the minimum spanning tree of a given graph.
	 *
	 * This implementation uses the Prim algorithm.
	 *
	 * \param g The graph from which we want the minimum spanning tree. The graph must be an
	 *          undirected graph or the result will be garbage.
	 * \param vertex The vertex from which to compute the minimum spanning tree.
	 * \return The minimum spanning tree of the graph.
	 */
	template <typename Graph>
	Graph minimumSpanningTree(Graph const& g, typename Graph::ConstNode_t vertex) {
		using ConstNode   = typename Graph::ConstNode_t;

		auto compareEdges([&g](std::pair<ConstNode, ConstNode> left,
		                       std::pair<ConstNode, ConstNode> right) mutable {
			return g.getEdgeProperty(left.first, left.second).weight <
			       g.getEdgeProperty(right.first, right.second).weight;
		});

		std::set<std::pair<ConstNode, ConstNode>, decltype(compareEdges)> sortedEdgesToCheck(
		        compareEdges);
		std::set<ConstNode> checkedNodes{vertex};
		std::set<std::pair<ConstNode, ConstNode>> mstEdgeSet;

		g.eachAdjacents(vertex, [&g, &sortedEdgesToCheck, &vertex](ConstNode end) {
			sortedEdgesToCheck.insert({vertex, end});
		});

		while(sortedEdgesToCheck.size() != 0) {
			auto const& currentEdge = *sortedEdgesToCheck.begin();
			if(checkedNodes.count(currentEdge.second) == 0) {
				mstEdgeSet.insert(currentEdge);

				g.eachAdjacents(currentEdge.second,
				                  [&sortedEdgesToCheck, begin = currentEdge.second ](ConstNode end){
								  	sortedEdgesToCheck.insert({begin, end});
				                  });
				checkedNodes.insert(currentEdge.second);
			}

			sortedEdgesToCheck.erase(sortedEdgesToCheck.begin());
		}

		Graph mst;
		for(auto const& edge : mstEdgeSet) {
			mst.addEdges({edge.first.getName(), edge.second.getName()});
			mst.setEdgeProperty(
			        edge.first, edge.second, g.getEdgeProperty(edge.first, edge.second));
		}
		return undirected(mst);
	}
}
