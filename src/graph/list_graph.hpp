#ifndef _GRAPH_LIB_LIST_GRAPH_HPP_
#define _GRAPH_LIB_LIST_GRAPH_HPP_

#include "utility.hpp"
#include "list_node.hpp"

#include <list>
#include <ostream>
#include <set>
#include <vector>

#include <cstddef>

namespace list {
	class Graph;
}

std::ostream& operator<<(std::ostream& os, list::Graph graph);
std::string makeDigraph(std::string name, list::Graph graph);

/*! \brief Namespace used for the classes and types using a graph with an adjacency list as internal
 *         representation.
 */
namespace list {

	/*! \brief Represents a graph with an adjacency list as internal representation.
	 */
	class Graph {
	public:
		using Node_t      = Node;
		using ConstNode_t = ConstNode;

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
		std::vector<std::list<size_t>> getConnections() const;

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

		/*! Call a given function for each vertices.
		 *
		 * The functor must be convertible to a function of type void(Node)
		 *
		 * \param functor the function to call
		 */
		template<typename Functor>
		void eachVertices(Functor&& functor) const;

		/*! Call a given function for each edges.
		 *
		 * The functor must be convertible to a function of type void(Node, Node)
		 *
		 * \param functor the function to call
		 */
		template<typename Functor>
		void eachEdges(Functor&& functor) const;

		/*! Call a given function for each vertices adjacent to the given edge.
		 *
		 * The functor must be convertible to a function of type void(Node)
		 *
		 * \param functor the function to call
		 */
		template<typename Functor>
		void eachAdjacents(ConstNode vertex, Functor&& functor) const;

		/*! \brief Return a Node representing a node from this graph with a given name.
		 *
		 * \param nodeId The name of the node.
		 * \return The Node representing the given node.
		 */
		Node operator[](size_t nodeId);

		/*! \brief Return a Node representing a node from this graph with a given name.
		 *
		 * \param nodeId The name of the node.
		 * \return The Node representing the given node.
		 */
		ConstNode operator[](size_t nodeId) const;

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
		std::vector<std::list<size_t>> connections;
	};

}

#include "list_graph.tcc"

#endif
