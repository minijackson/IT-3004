#pragma once

#include "matrix_node.hpp"

namespace graph {
	namespace matrix {

		template <typename NodePropertyVector, typename Connections>
		template <typename OtherNodePropertyVector, typename OtherConnections>
		GenericNode<NodePropertyVector, Connections>::GenericNode(
		        GenericNode<OtherNodePropertyVector, OtherConnections>& other)
		      : id(other.id)
		      , connections(other.connections)
		      , name(other.name)
		      , property(other.property) {}

		template <typename NodePropertyVector, typename Connections>
		template <typename OtherNodePropertyVector, typename OtherConnections>
		GenericNode<NodePropertyVector, Connections>::GenericNode(
		        GenericNode<OtherNodePropertyVector, OtherConnections>&& other)
		      : id(other.id)
		      , connections(other.connections)
		      , name(std::move(other.name))
		      , property(std::move(other.property)) {}

		template <typename NodePropertyVector, typename Connections>
		template <typename ParentConnections>
		GenericNode<NodePropertyVector, Connections>::GenericNode(size_t id,
		                                                    ParentConnections& connections,
		                                                    std::string name,
		                                                    NodePropertyVector& property)
		      : id(id)
		      , connections(connections)
		      , name(std::move(name))
		      , property(property) {}

		template <typename NodePropertyVector, typename Connections>
		template <typename OtherNodePropertyVector, typename OtherConnections>
		bool GenericNode<NodePropertyVector, Connections>::operator==(
		        GenericNode<OtherNodePropertyVector, OtherConnections> const& other) const {
			return name == other.name;
		}

		template <typename NodePropertyVector, typename Connections>
		template <typename OtherNodePropertyVector, typename OtherConnections>
		bool GenericNode<NodePropertyVector, Connections>::operator<(
		        GenericNode<OtherNodePropertyVector, OtherConnections> const& other) const {
			return id < other.id;
		}

		template <typename NodePropertyVector, typename Connections>
		bool GenericNode<NodePropertyVector, Connections>::isConnectedTo(
		        GenericNode<NodePropertyVector, Connections> const& other) const {
			return connections[other.getId()];
		}

		template <typename NodePropertyVector, typename Connections>
		size_t GenericNode<NodePropertyVector, Connections>::getId() const {
			return id;
		}

		template <typename NodePropertyVector, typename Connections>
		std::string GenericNode<NodePropertyVector, Connections>::getName() const {
			return name;
		}

		template <typename NodePropertyVector, typename Connections>
		std::vector<bool> GenericNode<NodePropertyVector, Connections>::getConnections() const {
			return connections;
		}

		template <typename NodePropertyVector, typename Connections>
		NodePropertyVector GenericNode<NodePropertyVector, Connections>::getProperty() const {
			return property;
		}

		template <typename NodeProperty>
		Node<NodeProperty>::Node(size_t id,
		                         std::vector<bool>& connections,
		                         std::string name,
		                         std::vector<NodeProperty>& property)
		      : ParentClass(id, connections, std::move(name), property) {}

		template <typename NodeProperty>
		NodeProperty& Node<NodeProperty>::getProperty() {
			return this->property[this->id];
		}

		template <typename NodeProperty>
		void Node<NodeProperty>::setProperty(NodeProperty property) {
			this->property[this->id] = std::move(property);
		}

		template <typename NodeProperty>
		ConstNode<NodeProperty>::ConstNode(size_t id,
		                                   std::vector<bool> const& connections,
		                                   std::string name,
		                                   std::vector<NodeProperty> const& property)
		      : ParentClass(id, connections, std::move(name), property) {}

		template <typename NodePropertyVector>
		ConstNode<NodePropertyVector>::ConstNode(Node<NodePropertyVector> const& other)
		      : ParentClass(other.id, other.connections, other.name, other.property) {}
	}
}
