#include "matrix_node.hpp"

namespace matrix {

	class Node;
	class ConstNode;

	ConstNode::ConstNode(size_t id, std::vector<std::vector<bool>> const& connections)
	      : GenericNode(id, connections) {}

	Node::Node(size_t id, std::vector<std::vector<bool>>& connections)
	      : GenericNode(id, connections) {}

	void Node::connectTo(size_t otherId) {
		connections[otherId] = true;
	}

	void Node::disconnectFrom(size_t otherId) {
		connections[otherId] = false;
	}

}
