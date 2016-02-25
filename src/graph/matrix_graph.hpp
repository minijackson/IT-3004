#ifndef _GRAPH_LIB_MATRIX_GRAPH_HPP_
#define _GRAPH_LIB_MATRIX_GRAPH_HPP_

#include "utility.hpp"
#include "matrix_node.hpp"

#include <ostream>
#include <set>
#include <vector>

#include <cstddef>

namespace matrix {
	class Graph;
}

std::ostream& operator<<(std::ostream& os, matrix::Graph graph);
std::string makeDigraph(std::string name, matrix::Graph graph);

/*! \brief Namespace used for the classes and types using a graph with an adjacency matrix as
 *         internal representation.
 */
namespace matrix {

	/*! \brief Represents a graph with an adjacency matrix as internal representation.
	 */
	class Graph {
	public:
		/*! \brief Create an empty graph
		 */
		Graph() = default;

		/*! \brief Create a graph with a certain number of vertices.
		 *
		 * \param nbVertices the number of vertices in the graph
		 */
		explicit Graph(size_t nbVertices);

		/*! \brief Create a graph with a certain number of vertices and some arcs.
		 *
		 * \param nbVertices the number of vertices in the graph.
		 * \param arcs the arcs to add in the graph.
		 */
		template <typename... Arcs>
		explicit Graph(size_t nbVertices, Arcs... arcs);

		/*! \brief Create a graph with some arcs.
		 *
		 * \param arcs the arcs to add in the graph.
		 */
		Graph(std::initializer_list<std::pair<size_t, size_t>> arcs);

		/*! \brief Add an arc to the graph.
		 *
		 * \param arc A pair whose first element is the starting vertex, and the second
		 *        element is the end vertex.
		 */
		void addArcs(std::pair<size_t, size_t> const& arc);

		/*! \brief Add several arcs to the graph.
		 *
		 * \param arc An arc.
		 * \param arcs More arcs.
		 * \sa addArcs(std::pair<size_t, size_t>)
		 */
		template <typename... Arcs>
		inline void addArcs(std::pair<size_t, size_t> const& arc, Arcs... arcs);

		/*! \brief Get the matrix of connections for this graph.
		 */
		std::vector<std::vector<bool>> getConnections() const;

		/*! \brief Get the number of vertices in the graph.
		 *
		 * \return the number of vertices in the graph.
		 */
		size_t getVerticesCount() const;

		/*! \brief Get the number of arcs in the graph.
		 *
		 * \return the number of arcs in the graph.
		 */
		size_t getArcsCount() const;

		/*! \brief Return the symmetric graph of the current graph.
		 *
		 * \return the symmetric graph of the current graph.
		 */
		Graph symmetric() const;

		/*! \brief Get the strongly connected component of a given vertex.
		 *
		 * \param vertex The vertex from which to compute the strongly connected component.
		 * \return the set of vertices which compose the strongly connected component.
		 */
		std::set<size_t> stronglyConnectedComponent(size_t vertex) const;

		/*! \brief Get the connected component of a given vertex.
		 *
		 * \param vertex The vertex from which to compute the connected component.
		 * \return the set of vertices which compose the connected component.
		 */
		std::set<size_t> connectedComponent(size_t vertex) const;

		/*! \brief Return a Node representing a node from this graph with a given name.
		 *
		 * \param nodeId The name of the node.
		 * \return The Node representing the given node.
		 */
		Node operator[](size_t nodeId);

		/*! \brief Check if two graphs are equal.
		 *
		 * \param other The other graph to check for equality.
		 * \return true if the two graphs are equal.
		 */
		bool operator==(Graph const& other) const;

		/*! \brief Check if two graphs are not equal.
		 *
		 * \param other The other graph to check for equality.
		 * \return true if the two graphs are not equal.
		 */
		bool operator!=(Graph const& other) const;

		friend std::ostream& (::operator<<)(std::ostream& os, Graph graph);

	protected:
		/*! \brief The matrix representing the connections in the graph.
		 */
		std::vector<std::vector<bool>> connections;
	};

}

#include "matrix_graph.tcc"

#endif
