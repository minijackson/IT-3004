#pragma once

#include "list_node.hpp"

#include <algorithm>

namespace graph {
	namespace list {

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
		      , name(name)
		      , property(property) {}

		template <typename NodePropertyVector, typename Connections>
		template <typename OtherNodePropertyVector, typename OtherConnections>
		bool GenericNode<NodePropertyVector, Connections>::operator==(
		        GenericNode<OtherNodePropertyVector, OtherConnections> other) const {
			return name == other.name;
		}

		template <typename NodePropertyVector, typename Connections>
		template <typename OtherNodePropertyVector, typename OtherConnections>
		bool GenericNode<NodePropertyVector, Connections>::operator<(
		        GenericNode<OtherNodePropertyVector, OtherConnections> other) const {
			return id < other.id;
		}

		template <typename NodePropertyVector, typename Connections>
		bool GenericNode<NodePropertyVector, Connections>::isConnectedTo(
		        GenericNode<NodePropertyVector, Connections> const& other) const {
			return std::find(connections.begin(), connections.end(), other.getId()) !=
			       connections.end();
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
		std::list<size_t> GenericNode<NodePropertyVector, Connections>::getConnections() const {
			return connections;
		}

		template <typename NodePropertyVector, typename Connections>
		auto GenericNode<NodePropertyVector, Connections>::getProperty()
		        const -> NodeProperty {
			return property[id];
		}

		template <typename NodeProperty>
		Node<NodeProperty>::Node(size_t id,
		                         std::list<size_t>& connections,
		                         std::string name,
		                         std::vector<NodeProperty>& property)
		      : ParentClass(id, connections, name, property) {}

		template <typename NodeProperty>
		NodeProperty& Node<NodeProperty>::getProperty() {
			return this->property[this->id];
		}

		template <typename NodeProperty>
		void Node<NodeProperty>::setProperty(NodeProperty property) {
			this->property[this->id] = property;
		}

		template <typename NodeProperty>
		ConstNode<NodeProperty>::ConstNode(size_t id,
		                                   std::list<size_t> const& connections,
		                                   std::string name,
		                                   std::vector<NodeProperty> const& property)
		      : ParentClass(id, connections, name, property) {}

		template <typename NodePropertyVector>
		ConstNode<NodePropertyVector>::ConstNode(Node<NodePropertyVector> other)
		      : ParentClass(other.id, other.connections, other.name, other.property) {}
	}
}
