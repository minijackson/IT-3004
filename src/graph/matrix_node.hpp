#ifndef _GRAPH_LIB_MATRIX_NODE_HPP_
#define _GRAPH_LIB_MATRIX_NODE_HPP_

#include <vector>

#include <cstddef>

namespace graph {
	namespace matrix {

		/*! \brief Represents a Node
		 *
		 * An object of type Node will store a reference to the boolean matrix of the parent Graph.
		 * Therefore, like an iterator, using this object after the destruction of the parent Graph
		 * will result in a dandling reference.
		 */
		template <typename NodeProperty, typename Connections>
		class GenericNode {
		public:
			friend class GenericNode<NodeProperty, std::vector<bool>&>;
			friend class GenericNode<NodeProperty, std::vector<bool> const&>;

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
			GenericNode(size_t id, ParentConnections& connections, NodeProperty& property);

			/*! \brief Return true if two nodes are the same node.
			 *
			 * This function will not check if the nodes are from the same graph.
			 *
			 * \param other the other node to compare to.
			 * \return true if the two nodes are the same node from the same graph
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
			bool isConnectedTo(size_t otherId) const;

			/*! \brief Get the arcs which have the current node as starting point.
			 *
			 * \return The list of arcs which have the current node as starting point.
			 */
			std::vector<size_t> getArcs() const;

			/*! \brief Get the name of the current node.
			 *
			 * \return The name of the current node.
			 */
			size_t getId() const;

			/*! \brief Return the internal boolean matrix.
			 *
			 * \return A copy of the internale boolean matrix.
			 */
			std::vector<bool> getConnections() const;

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

			/*! \brief The property of the current node.
			 */
			NodeProperty property;
		};

		template <typename NodeProperty>
		class ConstNode;

		template <typename NodeProperty>
		class Node : public GenericNode<NodeProperty&, std::vector<bool>&> {
			using ParentClass = GenericNode<NodeProperty&, std::vector<bool>&>;
		public:
			/*! \brief Node default constructor.
			 *
			 * \param id The name of the node to reference.
			 * \param connections A reference to the boolean matrix of the parent Graph.
			 */
			Node(size_t id, std::vector<bool>& connections, NodeProperty& property);

			/*! \brief Connect the current node to the given node.
			 *
			 * \param otherId The other node to connect to.
			 */
			void connectTo(size_t otherId);

			/*! \brief Disconnect the current node from the given node.
			 *
			 * \param otherId The other node to disconnect from.
			 */
			void disconnectFrom(size_t otherId);

			/*! \brief Return the property of the current Node.
			 *
			 * \return A reference to the current node's property.
			 */
			NodeProperty& getProperty();

			friend ConstNode<NodeProperty>;
		};

		template <typename NodeProperty>
		class ConstNode : public GenericNode<NodeProperty const&, std::vector<bool> const&> {
			using ParentClass = GenericNode<NodeProperty const&, std::vector<bool> const&>;
		public:
			/*! \brief Node default constructor.
			 *
			 * \param id The name of the node to reference.
			 * \param connections A reference to the boolean matrix of the parent Graph.
			 */
			ConstNode(size_t id,
			          std::vector<bool> const& connections,
			          NodeProperty const& property);

			/*! \brief Convert a Node to a ConstNode.
			 *
			 * \param other The Node to convert.
			 */
			ConstNode(Node<NodeProperty> other);
		};
	}
}

#include "matrix_node.tcc"

#endif
