#include "list_node.hpp"

#include <algorithm>

namespace graph {
	namespace list {

		template <typename NodeProperty, typename Connections>
		template <typename OtherNodeProperty, typename OtherConnections>
		GenericNode<NodeProperty, Connections>::GenericNode(
		        GenericNode<OtherNodeProperty, OtherConnections>& other)
		      : id(other.id)
		      , connections(other.connections) {}

		template <typename NodeProperty, typename Connections>
		template <typename OtherNodeProperty, typename OtherConnections>
		GenericNode<NodeProperty, Connections>::GenericNode(
		        GenericNode<OtherNodeProperty, OtherConnections>&& other)
		      : id(other.id)
		      , connections(other.connections) {}

		template <typename NodeProperty, typename Connections>
		template <typename ParentConnections>
		GenericNode<NodeProperty, Connections>::GenericNode(size_t id,
		                                                    ParentConnections& connections,
		                                                    NodeProperty& property)
		      : id(id)
		      , connections(connections)
		      , property(property) {}

		template <typename NodeProperty, typename Connections>
		template <typename OtherNodeProperty, typename OtherConnections>
		bool GenericNode<NodeProperty, Connections>::operator==(
		        GenericNode<OtherNodeProperty, OtherConnections> other) const {
			return id == other.id;
		}

		template <typename NodeProperty, typename Connections>
		template <typename OtherNodeProperty, typename OtherConnections>
		bool GenericNode<NodeProperty, Connections>::operator<(
		        GenericNode<OtherNodeProperty, OtherConnections> other) const {
			return id < other.id;
		}

		template <typename NodeProperty, typename Connections>
		bool GenericNode<NodeProperty, Connections>::isConnectedTo(size_t otherId) const {
			return std::find(connections.begin(), connections.end(), otherId) != connections.end();
		}

		template <typename NodeProperty, typename Connections>
		std::list<size_t> GenericNode<NodeProperty, Connections>::getArcs() const {
			return connections;
		}

		template <typename NodeProperty, typename Connections>
		size_t GenericNode<NodeProperty, Connections>::getId() const {
			return id;
		}

		template <typename NodeProperty, typename Connections>
		std::list<size_t> GenericNode<NodeProperty, Connections>::getConnections() const {
			return connections;
		}

		template <typename NodeProperty, typename Connections>
		NodeProperty GenericNode<NodeProperty, Connections>::getProperty() const {
			return property;
		}

		template <typename NodeProperty>
		Node<NodeProperty>::Node(size_t id, std::list<size_t>& connections, NodeProperty& property)
		      : ParentClass(id, connections, property) {}

		template <typename NodeProperty>
		void Node<NodeProperty>::connectTo(size_t otherId) {
			this->connections.push_back(otherId);
		}

		template <typename NodeProperty>
		void Node<NodeProperty>::disconnectFrom(size_t otherId) {
			this->connections.remove(otherId);
		}

		template <typename NodeProperty>
		ConstNode<NodeProperty>::ConstNode(size_t id,
		                                   std::list<size_t> const& connections,
		                                   NodeProperty const& property)
		      : ParentClass(id, connections, property) {}

		template <typename NodeProperty>
		ConstNode<NodeProperty>::ConstNode(Node<NodeProperty> other)
		      : ParentClass(other.id, other.connections, other.property) {}
	}
}
