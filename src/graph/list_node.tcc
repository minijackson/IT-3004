#include "list_node.hpp"

#include <algorithm>

namespace list {

	template <typename Connections>
	template <typename OtherConnections>
	GenericNode<Connections>::GenericNode(GenericNode<OtherConnections>& other)
	      : id(other.id)
	      , connections(other.connections) {}

	template <typename Connections>
	template <typename OtherConnections>
	GenericNode<Connections>::GenericNode(GenericNode<OtherConnections>&& other)
	      : id(other.id)
	      , connections(other.connections) {}

	template <typename Connections>
	template <typename ParentConnections>
	GenericNode<Connections>::GenericNode(size_t id, ParentConnections& connections)
	      : id(id)
	      , connections(connections[id]) {}

	template <typename Connections>
	bool GenericNode<Connections>::isConnectedTo(size_t otherId) const {
		return std::find(connections.begin(), connections.end(), otherId) != connections.end();
	}

	template <typename Connections>
	std::list<size_t> GenericNode<Connections>::getArcs() const {
		return connections;
	}

	template <typename Connections>
	size_t GenericNode<Connections>::getId() const {
		return id;
	}

	template <typename Connections>
	std::list<size_t> GenericNode<Connections>::getConnections() const {
		return connections;
	}
}
