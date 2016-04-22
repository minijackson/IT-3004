#ifndef GRAPH_LIB_ALGORITHMS_HPP
#define GRAPH_LIB_ALGORITHMS_HPP

#include <algorithm>
#include <set>

#include <cstddef>

namespace graph {

	/*! \brief Return the symmetric graph of the current graph.
	 *
	 * \param graph The Graph from which to create the symmetric graph.
	 * \return the symmetric graph of the current graph.
	 */
	template <typename Graph>
	std::decay_t<Graph> symmetric(Graph const& g) {
		std::decay_t<Graph> symmetricGraph;
		using ConstNode = typename Graph::ConstNode_t;

		g.eachEdges([&symmetricGraph, &g](ConstNode begin, ConstNode end) {
			std::string beginName = begin.getName(), endName = end.getName();
			symmetricGraph.addEdges({end.getName(), begin.getName()});
			symmetricGraph.setEdgeProperty(symmetricGraph[endName],
			                               symmetricGraph[beginName],
			                               g.getEdgeProperty(begin, end));
		});

		return symmetricGraph;
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
	 * \return the set of vertices which compose the connected component.
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
}

#endif
