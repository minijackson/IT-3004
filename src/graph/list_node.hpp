#ifndef _GRAPH_LIB_LIST_NODE_HPP_
#define _GRAPH_LIB_LIST_NODE_HPP_

#include <list>
#include <string>

#include <cstddef>

namespace graph {
	namespace list {

		/*! \brief Represents a Node
		 *
		 * An object of type Node will store a reference to the adjacency list of the parent Graph.
		 * Therefore, like an iterator, using this object after the destruction of the parent Graph
		 * will result in a dandling reference.
		 */
		template <typename NodeProperty, typename Connections>
		class GenericNode {
		public:
			friend class GenericNode<NodeProperty, std::list<size_t>&>;
			friend class GenericNode<NodeProperty, std::list<size_t> const&>;

			GenericNode() = delete;

			template <typename OtherNodeProperty, typename OtherConnections>
			GenericNode(GenericNode<OtherNodeProperty, OtherConnections>& other);

			template <typename OtherNodeProperty, typename OtherConnections>
			GenericNode(GenericNode<OtherNodeProperty, OtherConnections>&& other);

			/*! \brief Node default constructor.
			 *
			 * \param id The name of the node to reference.
			 * \param connections A reference to the boolean matrix of the parent Graph.
			 */
			template <typename ParentConnections>
			GenericNode(size_t id,
			            ParentConnections& connections,
			            std::string name,
			            NodeProperty& property);

			/*! \brief Return true if two nodes are the same node (by name).
			 *
			 * This function will not check if the nodes are from the same graph.
			 *
			 * \param other the other node to compare to.
			 * \return true if the two nodes are the same node.
			 */
			template <typename OtherNodeProperty, typename OtherConnections>
			bool operator==(GenericNode<OtherNodeProperty, OtherConnections> other) const;

			/*! \brief Compare two nodes arbitrarily.
			 *
			 * \param other the other node to compare to.
			 * \return true or false.
			 */
			template <typename OtherNodeProperty, typename OtherConnections>
			bool operator<(GenericNode<OtherNodeProperty, OtherConnections> other) const;

			/*! \brief Return true if the current node is connected to the given node.
			 *
			 * \param otherId the other node to check.
			 * \return true if the current node is connected to the given node.
			 */
			bool isConnectedTo(GenericNode<NodeProperty, Connections> const& other) const;

			/*! \brief Get the id of the current node.
			 *
			 * \return The id of the current node.
			 */
			size_t getId() const;

			/*! \brief Get the name of the current node.
			 *
			 * \return The name of the current node.
			 */
			std::string getName() const;

			/*! \brief Return the internal boolean matrix.
			 *
			 * \return A copy of the internale boolean matrix.
			 */
			std::list<size_t> getConnections() const;

			/*! \brief Return the property of the current Node.
			 *
			 * \return The current node's property.
			 */
			NodeProperty getProperty() const;

		protected:
			/*! \brief The name of the node to reference.
			 */
			size_t const id;

			/*! \brief The matrix representing the connections in the graph.
			 */
			Connections connections;

			/*! \brief The name of the node to refer.
			 */
			std::string const name;

			/*! \brief The property of the current node.
			 */
			NodeProperty property;
		};

		template <typename NodeProperty>
		class ConstNode;

		template <typename NodeProperty>
		class Node : public GenericNode<NodeProperty&, std::list<size_t>&> {
			using ParentClass = GenericNode<NodeProperty&, std::list<size_t>&>;
		public:
			/*! \brief Node default constructor.
			 *
			 * \param id The name of the node to reference.
			 * \param connections A reference to the boolean matrix of the parent Graph.
			 * \param name The name of the node.
			 * \param property The property of the node.
			 */
			Node(size_t id,
			     std::list<size_t>& connections,
			     std::string name,
			     NodeProperty& property);

			/*! \brief Return the property of the current Node.
			 *
			 * \return A reference to the current node's property.
			 */
			NodeProperty& getProperty();

			/*! \brief Set the property of the current Node.
			 */
			void setProperty(NodeProperty property);

			friend ConstNode<NodeProperty>;
		};

		template <typename NodeProperty>
		class ConstNode : public GenericNode<NodeProperty const&, std::list<size_t> const&> {
			using ParentClass = GenericNode<NodeProperty const&, std::list<size_t> const&>;
		public:
			/*! \brief Node default constructor.
			 *
			 * \param id The name of the node to reference.
			 * \param connections A reference to the boolean matrix of the parent Graph.
			 */
			ConstNode(size_t id,
			          std::list<size_t> const& connections,
			          std::string name,
			          NodeProperty const& property);

			/*! \brief Convert a Node to a ConstNode.
			 *
			 * \param other The Node to convert.
			 */
			ConstNode(Node<NodeProperty> other);
		};
	}
}

#include "list_node.tcc"

#endif
