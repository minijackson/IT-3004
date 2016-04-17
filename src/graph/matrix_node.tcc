#include "matrix_node.hpp"

namespace graph {
	namespace matrix {

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
		template <typename OtherConnections>
		bool GenericNode<Connections>::operator==(GenericNode<OtherConnections> other) const {
			return id == other.id;
		}

		template <typename Connections>
		template <typename OtherConnections>
		bool GenericNode<Connections>::operator<(GenericNode<OtherConnections> other) const {
			return (id < other.id);
		}

		template <typename Connections>
		bool GenericNode<Connections>::isConnectedTo(size_t otherId) const {
			return connections[otherId];
		}

		template <typename Connections>
		std::vector<size_t> GenericNode<Connections>::getArcs() const {
			std::vector<size_t> res;
			for(size_t i = 0; i < connections.size(); ++i) {
				if(connections[i]) {
					res.push_back(i);
				}
			}
			return res;
		}

		template <typename Connections>
		size_t GenericNode<Connections>::getId() const {
			return id;
		}

		template <typename Connections>
		std::vector<bool> GenericNode<Connections>::getConnections() const {
			return connections;
		}
	}
}
