#include "matrix_node.hpp"

namespace matrix {

	Node::Node(size_t id, std::vector<std::vector<bool>>& connections)
	      : id(id)
	      , connections(connections[id]) {}

	std::vector<bool>::reference Node::operator[](size_t otherId) {
		return connections[otherId];
	}

	bool Node::isConnectedTo(size_t otherId) const {
		return connections[otherId];
	}

	void Node::connectTo(size_t otherId) {
		connections[otherId] = true;
	}

	void Node::disconnectFrom(size_t otherId) {
		connections[otherId] = false;
	}

	std::vector<size_t> Node::getArcs() const {
		std::vector<size_t> res;
		for(size_t i = 0; i < connections.size() ; ++i) {
			if(connections[i]) {
				res.push_back(i);
			}
		}
		return res;
	}

	size_t Node::getId() const {
		return id;
	}

	std::vector<bool> Node::getConnections() const {
		return connections;
	}

}
