#pragma once

#include <vector>
#include <string>

#include <cstddef>

namespace graph {
	namespace matrix {

		/*! \brief Represents a generic Node
		 *
		 * This is used by the classes Node and ConstNode.
		 */
		template <typename NodePropertyVector, typename Connections>
		class GenericNode {
			using NodeProperty = typename std::decay_t<NodePropertyVector>::value_type;
		public:
			friend class GenericNode<NodePropertyVector, std::vector<bool>&>;
			friend class GenericNode<NodePropertyVector, std::vector<bool> const&>;

			GenericNode() = delete;

			/*! \brief Construct a node from another node.
			 *
			 * \param other the other node.
			 */
			template <typename OtherNodePropertyVector, typename OtherConnections>
			GenericNode(GenericNode<OtherNodePropertyVector, OtherConnections>& other);

			/*! \brief Construct a node from another node.
			 *
			 * \param other the other node.
			 */
			template <typename OtherNodePropertyVector, typename OtherConnections>
			GenericNode(GenericNode<OtherNodePropertyVector, OtherConnections>&& other);

			/*! \brief Node default constructor.
			 *
			 * \param id The name of the node to reference.
			 * \param connections A reference to the boolean matrix of the parent Graph.
			 * \param name the name of the node.
			 * \param property a reference to the properties vector of the parent Graph.
			 */
			template <typename ParentConnections>
			GenericNode(size_t id,
			            ParentConnections& connections,
			            std::string name,
			            NodePropertyVector& property);

			/*! \brief Return true if two nodes are the same node (by name).
			 *
			 * This function will not check if the nodes are from the same graph.
			 *
			 * \param other the other node to compare to.
			 * \return true if the two nodes are the same node.
			 */
			template <typename OtherNodeProperty, typename OtherConnections>
			bool operator==(GenericNode<OtherNodeProperty, OtherConnections> const& other) const;

			/*! \brief Compare two nodes arbitrarily.
			 *
			 * \param other the other node to compare to.
			 * \return true or false.
			 */
			template <typename OtherNodeProperty, typename OtherConnections>
			bool operator<(GenericNode<OtherNodeProperty, OtherConnections> const& other) const;

			/*! \brief Return true if the current node is connected to the given node.
			 *
			 * \param other the other node to check.
			 * \return true if the current node is connected to the given node.
			 */
			bool isConnectedTo(GenericNode<NodePropertyVector, Connections> const& other) const;

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
			std::vector<bool> getConnections() const;

			/*! \brief Return the property of the current Node.
			 *
			 * \return The current node's property.
			 */
			NodeProperty const& getProperty() const;

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
			NodePropertyVector property;
		};

		template <typename NodeProperty>
		class ConstNode;

		/*! \brief Represents a Node
		 *
		 * An object of type Node will store a reference to the boolean matrix of the parent Graph.
		 * Therefore, like an iterator, using this object after the destruction of the parent Graph
		 * will result in a dandling reference.
		 */
		template <typename NodeProperty>
		class Node : public GenericNode<std::vector<NodeProperty>&, std::vector<bool>&> {
			using ParentClass = GenericNode<std::vector<NodeProperty>&, std::vector<bool>&>;
		public:
			/*! \brief Node default constructor.
			 *
			 * \param id The name of the node to reference.
			 * \param connections A reference to the boolean matrix of the parent Graph.
			 * \param name The name of the node.
			 * \param property The property of the node.
			 */
			Node(size_t id,
			     std::vector<bool>& connections,
			     std::string name,
			     std::vector<NodeProperty>& property);

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

		/*! \brief Represents a immutable Node
		 *
		 * An object of type Node will store a constant reference to the boolean matrix of the
		 * parent Graph. Therefore, like an iterator, using this object after the destruction of the
		 * parent Graph will result in a dandling reference.
		 */
		template <typename NodeProperty>
		class ConstNode
		        : public GenericNode<std::vector<NodeProperty> const&, std::vector<bool> const&> {
			using ParentClass =
			        GenericNode<std::vector<NodeProperty> const&, std::vector<bool> const&>;

		public:
			/*! \brief Node default constructor.
			 *
			 * \param id The name of the node to reference.
			 * \param connections A reference to the boolean matrix of the parent Graph.
			 * \param name The name of the node.
			 * \param property a reference to the properties vector of the parent Graph.
			 */
			ConstNode(size_t id,
			          std::vector<bool> const& connections,
			          std::string name,
			          std::vector<NodeProperty> const& property);

			/*! \brief Convert a Node to a ConstNode.
			 *
			 * \param other The Node to convert.
			 */
			ConstNode(Node<NodeProperty> const& other);
		};
	}
}

#include "matrix_node.tcc"
