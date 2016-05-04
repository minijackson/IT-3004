#pragma once

#include "edge.hpp"
#include "utility.hpp"
#include "properties.hpp"
#include "matrix_node.hpp"

#include <map>
#include <ostream>
#include <set>
#include <vector>

#include <cstddef>

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
			/*! \brief The edge type used by this graph.
			 */
			using Edge_t      = Edge<EdgeProperty>;

			/*! \brief The node type used by this graph.
			 */
			using Node_t      = Node<NodeProperty>;

			/*! \brief The immutable node type used by this graph.
			 */
			using ConstNode_t = ConstNode<NodeProperty>;

			/*! \brief Create an empty graph
			 */
			Graph() = default;

			/*! \brief Create a graph with a certain number of vertices and some edges.
			 *
			 * \param edges the edges to add in the graph.
			 */
			template <typename... Edges>
			explicit Graph(Edges... edges);

			/*! \brief Create a graph with some edges.
			 *
			 * \param edges the edges to add in the graph.
			 */
			Graph(std::initializer_list<Edge_t> edges);

			/*! \brief Check if a node with the given name is in the graph.
			 *
			 * \param nodeName The name of the node.
			 */
			bool hasNode(std::string const& nodeName) const;

			/*! \brief Add a node to the graph.
			 *
			 * \param nodeName The name of the node to add.
			 * \param property The property of the node to add.
			 */
			void addNode(std::string const& nodeName, NodeProperty property);

			/*! \brief Add a node to the graph.
			 *
			 * \param nodeName The name of the node to add.
			 */
			void addNode(std::string const& nodeName);

			/*! \brief Remove a node from the graph.
			 *
			 * \warning This will invalidate every ConstNode or Node object from the graph that have
			 *          an id greater than the node to be removed.
			 *
			 * \exception std::out_of_range If the node is not in the graph.
			 *
			 * \param node The node to remove.
			 */
			void removeNode(ConstNode_t const& node);

			/*! \brief Check if an edge exists between two nodes.
			 *
			 * \param begin The start of the edge.
			 * \param end The end of the edge.
			 */
			bool hasEdge(ConstNode_t const& begin, ConstNode_t const& end) const;

			/*! \brief Add an edge to the graph.
			 *
			 * \param edge An edge.
			 */
			void addEdges(Edge_t const& edge);

			/*! \brief Add several edges to the graph.
			 *
			 * \param edge An edge.
			 * \param edges More edges.
			 * \sa addEdges(Edge_t)
			 */
			template <typename... Edges>
			inline void addEdges(Edge_t const& edge, Edges const&... edges);

			/*! \brief Add several edges to the graph.
			 *
			 * \param edges Many edges.
			 * \sa addEdges(Edge_t)
			 */
			inline void addEdges(std::initializer_list<Edge_t> edges);

			/*! \brief Connect two nodes in the graph
			 *
			 * \param begin The Node at the start of the edge.
			 * \param end The Node at the end of the edge.
			 * \param property The property to assign to the edge.
			 */
			void connect(ConstNode_t const& begin, ConstNode_t const& end, EdgeProperty property);

			/*! \brief Connect two nodes in the graph
			 *
			 * \param begin The Node at the start of the edge
			 * \param end The Node at the end of the edge
			 */
			void connect(ConstNode_t const& begin, ConstNode_t const& end);

			/*! \brief Remove an edge from the graph.
			 *
			 * \exception std::out_of_range If the edge is not in the graph.
			 *
			 * \param begin The start of the edge.
			 * \param end The end of the edge.
			 */
			void removeEdge(ConstNode_t const& begin, ConstNode_t const& end);

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
			std::vector<std::vector<bool>> getConnections() const;

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
			 * \param vertex the vertex from which to process the adjacents.
			 * \param functor the function to call
			 */
			template <typename Functor>
			void eachAdjacents(ConstNode_t const& vertex, Functor&& functor) const;

			/*! \brief Get the id of from the name of a node.
			 *
			 * If the node is not in the graph, it will be added.
			 *
			 * \param name The name of the node.
			 * \return The id of the node.
			 */
			size_t getId(std::string const& name);

			/*! \brief Get the id of from the name of a node.
			 *
			 * \param name The name of the node.
			 * \return The id of the node.
			 * \exception std::out_of_range If the node is not in the graph.
			 */
			size_t getId(std::string const& name) const;

			/*! \brief Get the first node of the graph.
			 *
			 * \return The first node in the graph.
			 */
			Node_t begin();

			/*! \brief Get the first node of the graph.
			 *
			 * \return The first node in the graph.
			 */
			ConstNode_t begin() const;

			/*! \brief Return a Node representing a node from this graph with a given name.
			 *
			 * \param nodeName The name of the node.
			 * \return The Node representing the given node.
			 */
			Node_t operator[](std::string const& nodeName);

			/*! \brief Return a Node representing a node from this graph with a given name.
			 *
			 * \param nodeName The name of the node.
			 * \return The Node representing the given node.
			 */
			ConstNode_t operator[](std::string const& nodeName) const;

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
			std::map<std::pair<std::string, std::string>, EdgeProperty> edgeProperties;

			/*! \brief The name of each nodes.
			 */
			std::map<std::string, size_t> nodeNames;
		};

		/*! \brief A graph to be used by an A* algorithm.
		 */
		template <typename State>
		using AstarGraph    = Graph<AstarNodeProperty<State>, WeightedProperty>;

		/*! \brief A graph with weighted edges
		 */
		using WeightedGraph = Graph<NoProperty, WeightedProperty>;
	}
}

#include "matrix_graph.tcc"
