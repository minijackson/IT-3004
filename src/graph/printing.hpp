#pragma once

#include "graph.hpp"

#include <iomanip>
#include <sstream>

namespace graph {

	/*! \brief Output a graph in the form of graphviz.
	 *
	 * \param os the stream to output to.
	 * \param graph the graph to output.
	 */
	template <template <class, class> class Graph, typename NodeProperty>
	std::ostream& operator<<(std::ostream& os, Graph<NodeProperty, NoProperty> const& graph) {
		using ConstNode = typename Graph<NodeProperty, NoProperty>::ConstNode_t;

		graph.eachEdges([&os](ConstNode start, ConstNode end) {
			os << std::quoted(start.getName()) << " -> " << std::quoted(end.getName()) << std::endl;
		});

		return os;
	}

	/*! \brief Output a graph with weighted arcs in the form of graphviz.
	 *
	 * \param os the stream to output to.
	 * \param graph the graph to output.
	 */
	template <template <class, class> class Graph, typename NodeProperty>
	std::ostream& operator<<(std::ostream& os, Graph<NodeProperty, WeightedProperty> const& graph) {
		using ConstNode = typename Graph<NodeProperty, NoProperty>::ConstNode_t;

		graph.eachEdges([&os, &graph](ConstNode start, ConstNode end) {
			os << std::quoted(start.getName()) << " -> " << std::quoted(end.getName())
			   << " [label=\"" << graph.getEdgeProperty(start, end).weight << "\"]" << std::endl;
		});

		return os;
	}

	/*! \brief Print a graph directly usable by graphviz.
	 *
	 * \param name the name of the graph.
	 * \param graph the graph to output.
	 */
	template <class Graph>
	std::string makeDigraph(std::string name,
	                        Graph const& graph) {
		std::ostringstream result;
		result << "digraph " << name << " {\n" << graph << "}" << std::endl;
		return result.str();
	}

}
