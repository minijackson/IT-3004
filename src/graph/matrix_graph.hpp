#ifndef _GRAPH_LIB_MATRIX_GRAPH_HPP_
#define _GRAPH_LIB_MATRIX_GRAPH_HPP_

#include "utility.hpp"
#include "properties.hpp"
#include "matrix_node.hpp"

#include <map>
#include <ostream>
#include <set>
#include <vector>

#include <cstddef>

namespace graph {
	namespace matrix {
		template <typename NodeProperty, typename EdgeProperty>
		class Graph;
	}
}

template <typename NodeProperty, typename EdgeProperty>
std::ostream& operator<<(std::ostream& os,
                         graph::matrix::Graph<NodeProperty, EdgeProperty> const& graph);

template <typename NodeProperty, typename EdgeProperty>
std::string makeDigraph(std::string name,
                        graph::matrix::Graph<NodeProperty, EdgeProperty> const& graph);

namespace graph {
	/*! \brief Namespace used for the classes and types using a graph with an adjacency matrix as
	 *         internal representation.
	 */
	namespace matrix {

		/*! \brief Represents a graph with an adjacency matrix as internal representation.
		 */
		template <typename NodeProperty, typename EdgeProperty>
		class Graph {
		public:
			using Node_t      = Node<NodeProperty>;
			using ConstNode_t = ConstNode<NodeProperty>;

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

			/*! Call a given function for each vertices.
			 *
			 * The functor must be convertible to a function of type void(Node)
			 *
			 * \param functor the function to call
			 */
			template <typename Functor>
			void eachVertices(Functor&& functor) const;

			/*! Call a given function for each edges.
			 *
			 * The functor must be convertible to a function of type void(Node, Node)
			 *
			 * \param functor the function to call
			 */
			template <typename Functor>
			void eachEdges(Functor&& functor) const;

			/*! Call a given function for each vertices adjacent to the given edge.
			 *
			 * The functor must be convertible to a function of type void(Node)
			 *
			 * \param functor the function to call
			 */
			template <typename Functor>
			void eachAdjacents(ConstNode_t vertex, Functor&& functor) const;

			/*! \brief Return a Node representing a node from this graph with a given name.
			 *
			 * \param nodeId The name of the node.
			 * \return The Node representing the given node.
			 */
			Node_t operator[](size_t nodeId);

			/*! \brief Return a Node representing a node from this graph with a given name.
			 *
			 * \param nodeId The name of the node.
			 * \return The Node representing the given node.
			 */
			ConstNode_t operator[](size_t nodeId) const;

			/*! \brief Check if two graphs are equal.
			 *
			 * \param other The other graph to check for equality.
			 * \return true if the two graphs are equal.
			 */
			template <typename OtherNodeProperty, typename OtherEdgeProperty>
			bool operator==(Graph<OtherNodeProperty, OtherEdgeProperty> const& other) const;

			/*! \brief Check if two graphs are not equal.
			 *
			 * \param other The other graph to check for equality.
			 * \return true if the two graphs are not equal.
			 */
			template <typename OtherNodeProperty, typename OtherEdgeProperty>
			bool operator!=(Graph<OtherNodeProperty, OtherEdgeProperty> const& other) const;

		protected:
			/*! \brief The matrix representing the connections in the graph.
			 */
			std::vector<std::vector<bool>> connections;

			/*! \brief The properties of each nodes.
			 */
			std::vector<NodeProperty> nodeProperties;

			/*! \brief The properties of each edges.
			 */
			std::map<std::pair<size_t, size_t>, EdgeProperty> edgeProperties;
		};

		using AstarGraph    = Graph<AstarNodeProperty, WeightedArcProperty>;
		using WeightedGraph = Graph<void, WeightedArcProperty>;
	}
}

#include "matrix_graph.tcc"

#endif
