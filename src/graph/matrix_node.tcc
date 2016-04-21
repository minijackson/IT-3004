#ifndef GRAPH_LIB_MATRIX_NODE_TCC
#define GRAPH_LIB_MATRIX_NODE_TCC

#include "matrix_node.hpp"

namespace graph {
	namespace matrix {

		template <typename NodeProperty, typename Connections>
		template <typename OtherNodeProperty, typename OtherConnections>
		GenericNode<NodeProperty, Connections>::GenericNode(
		        GenericNode<OtherNodeProperty, OtherConnections>& other)
		      : id(other.id)
		      , connections(other.connections)
		      , name(other.name)
		      , property(other.property) {}

		template <typename NodeProperty, typename Connections>
		template <typename OtherNodeProperty, typename OtherConnections>
		GenericNode<NodeProperty, Connections>::GenericNode(
		        GenericNode<OtherNodeProperty, OtherConnections>&& other)
		      : id(other.id)
		      , connections(other.connections)
		      , name(std::move(other.name))
		      , property(std::move(other.property)) {}

		template <typename NodeProperty, typename Connections>
		template <typename ParentConnections>
		GenericNode<NodeProperty, Connections>::GenericNode(size_t id,
		                                                    ParentConnections& connections,
		                                                    std::string name,
		                                                    NodeProperty& property)
		      : id(id)
		      , connections(connections)
		      , name(name)
		      , property(property) {}

		template <typename NodeProperty, typename Connections>
		template <typename OtherNodeProperty, typename OtherConnections>
		bool GenericNode<NodeProperty, Connections>::operator==(
		        GenericNode<OtherNodeProperty, OtherConnections> other) const {
			return name == other.name;
		}

		template <typename NodeProperty, typename Connections>
		template <typename OtherNodeProperty, typename OtherConnections>
		bool GenericNode<NodeProperty, Connections>::operator<(
		        GenericNode<OtherNodeProperty, OtherConnections> other) const {
			return id < other.id;
		}

		template <typename NodeProperty, typename Connections>
		bool GenericNode<NodeProperty, Connections>::isConnectedTo(
		        GenericNode<NodeProperty, Connections> const& other) const {
			return connections[other.getId()];
		}

		template <typename NodeProperty, typename Connections>
		size_t GenericNode<NodeProperty, Connections>::getId() const {
			return id;
		}

		template <typename NodeProperty, typename Connections>
		std::string GenericNode<NodeProperty, Connections>::getName() const {
			return name;
		}

		template <typename NodeProperty, typename Connections>
		std::vector<bool> GenericNode<NodeProperty, Connections>::getConnections() const {
			return connections;
		}

		template <typename NodeProperty, typename Connections>
		NodeProperty GenericNode<NodeProperty, Connections>::getProperty() const {
			return property;
		}

		template <typename NodeProperty>
		Node<NodeProperty>::Node(size_t id,
		                         std::vector<bool>& connections,
		                         std::string name,
		                         NodeProperty& property)
		      : ParentClass(id, connections, name, property) {}

		template <typename NodeProperty>
		NodeProperty& Node<NodeProperty>::getProperty() {
			return this->property;
		}

		template <typename NodeProperty>
		ConstNode<NodeProperty>::ConstNode(size_t id,
		                                   std::vector<bool> const& connections,
		                                   std::string name,
		                                   NodeProperty const& property)
		      : ParentClass(id, connections, name, property) {}

		template <typename NodeProperty>
		ConstNode<NodeProperty>::ConstNode(Node<NodeProperty> other)
		      : ParentClass(other.id, other.connections, other.name, other.property) {}
	}
}

#endif
