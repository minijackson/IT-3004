#ifndef _GRAPH_LIB_LIST_GRAPH_HPP_
#define _GRAPH_LIB_LIST_GRAPH_HPP_

#include "utility.hpp"
#include "properties.hpp"
#include "list_node.hpp"

#include <list>
#include <map>
#include <ostream>
#include <set>
#include <vector>

#include <cstddef>

namespace graph {
	namespace list {
		template <typename NodeProperty, typename EdgeProperty>
		class Graph;
	}
}

template <typename NodeProperty, typename EdgeProperty>
std::ostream& operator<<(std::ostream& os,
                         graph::list::Graph<NodeProperty, EdgeProperty> const& graph);

template <typename NodeProperty, typename EdgeProperty>
std::string makeDigraph(std::string name,
                        graph::list::Graph<NodeProperty, EdgeProperty> const& graph);

namespace graph {
	/*! \brief Namespace used for the classes and types using a graph with an adjacency list as
	 *         internal representation.
	 */
	namespace list {

		/*! \brief Represents a graph with an adjacency list as internal representation.
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

			/*! \brief Create a graph with a certain number of vertices and some edges.
			 *
			 * \param nbVertices the number of vertices in the graph.
			 * \param edges the edges to add in the graph.
			 */
			template <typename... Edges>
			explicit Graph(size_t nbVertices, Edges... edges);

			/*! \brief Create a graph with some edges.
			 *
			 * \param edges the edges to add in the graph.
			 */
			Graph(std::initializer_list<std::pair<size_t, size_t>> edges);

			/*! \brief Add an edge to the graph.
			 *
			 * \param edge A pair whose first element is the starting vertex, and the second
			 *        element is the end vertex.
			 */
			void addEdges(std::pair<size_t, size_t> const& edge);

			/*! \brief Add several edges to the graph.
			 *
			 * \param edge An edge.
			 * \param edges More edges.
			 * \sa addEdges(std::pair<size_t, size_t>)
			 */
			template <typename... Edges>
			inline void addEdges(std::pair<size_t, size_t> const& edge, Edges... edges);

			/*! \brief Add an edge to the graph.
			 *
			 * \param begin The Node at the start of the edge.
			 * \param end The Node at the end of the edge.
			 * \param property The property to assign to the edge.
			 */
			void addEdge(ConstNode_t const& begin, ConstNode_t const& end, EdgeProperty property);

			/*! \brief Add an edge to the graph.
			 *
			 * \param begin The Node at the start of the edge
			 * \param end The Node at the end of the edge
			 */
			void addEdge(ConstNode_t const& begin, ConstNode_t const& end);

			/*! \brief Get the property of a given edge.
			 *
			 * \param begin The node at the start of the edge.
			 * \param end The node at the end of the edge.
			 */
			EdgeProperty getEdgeProperty(ConstNode_t const& begin, ConstNode_t const& end) const;

			/*! \brief Set the property of the given edge.
			 *
			 * \param begin The node at the start of the edge.
			 * \param end The node at the end of the edge.
			 * \param property The property to set.
			 */
			void setEdgeProperty(ConstNode_t const& begin,
			                     ConstNode_t const& end,
			                     EdgeProperty property);

			/*! \brief Get the matrix of connections for this graph.
			 */
			std::vector<std::list<size_t>> getConnections() const;

			/*! \brief Get the number of vertices in the graph.
			 *
			 * \return the number of vertices in the graph.
			 */
			size_t getVerticesCount() const;

			/*! \brief Get the number of edges in the graph.
			 *
			 * \return the number of edges in the graph.
			 */
			size_t getEdgesCount() const;

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
			std::vector<std::list<size_t>> connections;


			/*! \brief The properties of each nodes.
			 */
			std::vector<NodeProperty> nodeProperties;

			/*! \brief The properties of each edges.
			 */
			std::map<std::pair<size_t, size_t>, EdgeProperty> edgeProperties;
		};

		using AstarGraph    = Graph<AstarNodeProperty, WeightedProperty>;
		using WeightedGraph = Graph<void, WeightedProperty>;
	}
}

#include "list_graph.tcc"

#endif
