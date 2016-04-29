#ifndef _GRAPH_LIB_LIST_GRAPH_HPP_
#define _GRAPH_LIB_LIST_GRAPH_HPP_

#include "list_node.hpp"
#include "properties.hpp"
#include "utility.hpp"

#include <list>
#include <map>
#include <ostream>
#include <set>
#include <vector>

#include <cstddef>

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

			/*! \brief Create a graph with a certain number of vertices and some edges.
			 *
			 * \param edges the edges to add in the graph.
			 */
			template <typename... Edges>
			explicit Graph(Edges... edges);

			/*! \brief Create a graph with some edges and their property.
			 *
			 * \param edges the edges to add in the graph.
			 */
			Graph(std::initializer_list<std::tuple<std::string, std::string, EdgeProperty>> edges);

			/*! \brief Create a graph with some edges.
			 *
			 * \param edges the edges to add in the graph.
			 */
			Graph(std::initializer_list<std::pair<std::string, std::string>> edges);

			/*! \brief Check if a node with the given name is in the graph.
			 *
			 * \param nodeName The name of the node.
			 */
			bool hasNode(std::string nodeName) const;

			/*! \brief Add a node to the graph.
			 *
			 * \param node The name of the node to add.
			 */
			void addNode(std::string nodeName, NodeProperty property);

			/*! \brief Add a node to the graph.
			 *
			 * \param node The name of the node to add.
			 */
			void addNode(std::string nodeName);

			/*! \brief Remove a node from the graph.
			 *
			 * \warning This will invalidate every ConstNode or Node object from the graph that have
			 *          an id greater than the node to be removed.
			 *
			 * \exception std::out_of_range If the node is not in the graph.
			 *
			 * \param node The node to remove.
			 */
			void removeNode(ConstNode_t node);

			/*! \brief Check if an edge exists between two nodes.
			 *
			 * \param begin The start of the edge.
			 * \param end The end of the edge.
			 */
			bool hasEdge(ConstNode_t begin, ConstNode_t end) const;

			/*! \brief Add an edge to the graph.
			 *
			 * \param edge A tuple whose first element is the starting vertex, the second element
			 *             is the end vertex, and the third element is the property of the vertex.
			 */
			void addEdges(std::tuple<std::string, std::string, EdgeProperty> edge);

			/*! \brief Add several edges to the graph.
			 *
			 * \param edge An edge with it's property.
			 * \param edges More edges with their properties.
			 * \sa addEdges(std::pair<std::string, std::string>)
			 */
			template <typename... Edges>
			inline void addEdges(std::tuple<std::string, std::string, EdgeProperty> edge,
			                     Edges... edges);

			/*! \brief Add an edge to the graph.
			 *
			 * \param edge A pair whose first element is the starting vertex, and the second
			 *        element is the end vertex.
			 */
			void addEdges(std::pair<std::string, std::string> edge);

			/*! \brief Add several edges to the graph.
			 *
			 * \param edge An edge.
			 * \param edges More edges.
			 * \sa addEdges(std::pair<std::string, std::string>)
			 */
			template <typename... Edges>
			inline void addEdges(std::pair<std::string, std::string> edge, Edges... edges);

			/*! \brief Connect two nodes in the graph
			 *
			 * \param begin The Node at the start of the edge.
			 * \param end The Node at the end of the edge.
			 * \param property The property to assign to the edge.
			 */
			void connect(ConstNode_t begin, ConstNode_t end, EdgeProperty property);

			/*! \brief Connect two nodes in the graph
			 *
			 * \param begin The Node at the start of the edge
			 * \param end The Node at the end of the edge
			 */
			void connect(ConstNode_t begin, ConstNode_t end);

			/*! \brief Remove an edge from the graph.
			 *
			 * \exception std::out_of_range If the edge is not in the graph.
			 *
			 * \param begin The start of the edge.
			 * \param end The end of the arc.
			 */
			void removeEdge(ConstNode_t begin, ConstNode_t end);

			/*! \brief Get the property of a given edge.
			 *
			 * \param begin The node at the start of the edge.
			 * \param end The node at the end of the edge.
			 */
			EdgeProperty getEdgeProperty(ConstNode_t begin, ConstNode_t end) const;

			/*! \brief Set the property of the given edge.
			 *
			 * \param begin The node at the start of the edge.
			 * \param end The node at the end of the edge.
			 * \param property The property to set.
			 */
			void setEdgeProperty(ConstNode_t begin,
			                     ConstNode_t end,
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

			/*! \brief Get the id of from the name of a node.
			 *
			 * If the node is not in the graph, it will be added.
			 *
			 * \param name The name of the node.
			 * \return The id of the node.
			 */
			size_t getId(std::string name);

			/*! \brief Get the id of from the name of a node.
			 *
			 * \param name The name of the node.
			 * \return The id of the node.
			 * \exception std::out_of_range If the node is not in the graph.
			 */
			size_t getId(std::string name) const;

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
			 * \param nodeId The name of the node.
			 * \return The Node representing the given node.
			 */
			Node_t operator[](std::string nodeName);

			/*! \brief Return a Node representing a node from this graph with a given name.
			 *
			 * \param nodeId The name of the node.
			 * \return The Node representing the given node.
			 */
			ConstNode_t operator[](std::string nodeName) const;

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
			std::map<std::pair<std::string, std::string>, EdgeProperty> edgeProperties;

			/*! \brief The name of each nodes.
			 */
			std::map<std::string, size_t> nodeNames;

			/*! \brief A list of each node names
			 *
			 * This is useful for reverse looking up the name of a node from which we know its id.
			 */
			std::vector<std::string> nameList;
		};

		template <typename State>
		using AstarGraph    = Graph<AstarNodeProperty<State>, WeightedProperty>;
		using WeightedGraph = Graph<NoProperty, WeightedProperty>;
	}
}

#include "list_graph.tcc"

#endif
