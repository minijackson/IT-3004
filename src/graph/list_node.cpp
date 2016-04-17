#include "list_node.hpp"

namespace graph {
	namespace list {

		class Node;
		class ConstNode;

		ConstNode::ConstNode(size_t id, std::vector<std::list<size_t>> const& connections)
		      : GenericNode(id, connections) {}

		Node::Node(size_t id, std::vector<std::list<size_t>>& connections)
		      : GenericNode(id, connections) {}

		void Node::connectTo(size_t otherId) {
			connections.push_back(otherId);
		}

		void Node::disconnectFrom(size_t otherId) {
			connections.remove(otherId);
		}
	}
}
