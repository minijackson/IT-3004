#include "list_node.hpp"

#include <algorithm>

namespace list {

	Node::Node(size_t id, std::vector<std::list<size_t>>& connections)
	      : id(id)
	      , connections(connections[id]) {}

	bool Node::isConnectedTo(size_t otherId) const {
		return std::find(connections.begin(), connections.end(), otherId) != connections.end();
	}

	void Node::connectTo(size_t otherId) {
		connections.push_back(otherId);
	}

	void Node::disconnectFrom(size_t otherId) {
		connections.remove(otherId);
	}

	std::list<size_t> Node::getArcs() const {
		return connections;
	}

	size_t Node::getId() const {
		return id;
	}

	std::list<size_t> Node::getConnections() const {
		return connections;
	}

}
