#include "matrix_node.hpp"

namespace graph {
	namespace matrix {

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
			return connections[otherId];
		}

		template <typename NodeProperty, typename Connections>
		std::vector<size_t> GenericNode<NodeProperty, Connections>::getArcs() const {
			std::vector<size_t> res;
			for(size_t i = 0; i < connections.size(); ++i) {
				if(connections[i]) {
					res.push_back(i);
				}
			}
			return res;
		}

		template <typename NodeProperty, typename Connections>
		size_t GenericNode<NodeProperty, Connections>::getId() const {
			return id;
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
		                         NodeProperty& property)
		      : ParentClass(id, connections, property) {}

		template <typename NodeProperty>
		void Node<NodeProperty>::connectTo(size_t otherId) {
			this->connections[otherId] = true;
		}

		template <typename NodeProperty>
		void Node<NodeProperty>::disconnectFrom(size_t otherId) {
			this->connections[otherId] = false;
		}

		template <typename NodeProperty>
		NodeProperty& Node<NodeProperty>::getProperty() {
			return this->property;
		}

		template <typename NodeProperty>
		ConstNode<NodeProperty>::ConstNode(size_t id,
		                                   std::vector<bool> const& connections,
		                                   NodeProperty const& property)
		      : ParentClass(id, connections, property) {}

		template <typename NodeProperty>
		ConstNode<NodeProperty>::ConstNode(Node<NodeProperty> other)
		      : ParentClass(other.id, other.connections, other.property) {}
	}
}
