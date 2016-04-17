#ifndef GRAPH_LIB_ALGORITHMS_HPP
#define GRAPH_LIB_ALGORITHMS_HPP

#include <cstddef>

#include <algorithm>
#include <set>

namespace graph {

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

		verticesToCheck = std::set<Node>{vertex};
		Graph sym       = g.symmetric();

		while(!verticesToCheck.empty()) {
			auto currentVertex = *verticesToCheck.begin();
			verticesToCheck.erase(currentVertex);

			sym.eachAdjacents(currentVertex,
			                  [&existPathToVertexSym, &verticesToCheck](Node i) {
				                  if(existPathToVertexSym.find(i) == existPathToVertexSym.end()) {
					                  verticesToCheck.insert(i);
					                  existPathToVertexSym.insert(i);
				                  }
				              });
		}

		std::set<Node> component;

		std::set_intersection(existPathToVertex.begin(),
		                      existPathToVertex.end(),
		                      existPathToVertexSym.begin(),
		                      existPathToVertexSym.end(),
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

		std::set<Node> verticesToCheck{vertex}, component{vertex};

		Graph sym = g.symmetric();

		while(!verticesToCheck.empty()) {
			auto currentVertex = *verticesToCheck.begin();
			verticesToCheck.erase(currentVertex);

			g.eachAdjacents(currentVertex,
			                [&component, &verticesToCheck](Node i) {
				                if(component.find(i) == component.end()) {
					                verticesToCheck.insert(i);
					                component.insert(i);
				                }
				            });

			sym.eachAdjacents(currentVertex,
			                  [&component, &verticesToCheck](Node i) {
				                  if(component.find(i) == component.end()) {
					                  verticesToCheck.insert(i);
					                  component.insert(i);
				                  }
				              });
		}

		return component;
	}
}

#endif
