#ifndef _GRAPH_LIB_LIST_GRAPH_TCC_
#define _GRAPH_LIB_LIST_GRAPH_TCC_

#include "list_graph.hpp"

#include <functional>
#include <type_traits>

namespace list {

	template <typename... Arcs>
	Graph::Graph(size_t nbVertices, Arcs... arcs)
	      : connections(nbVertices) {
		addArcs(arcs...);
	}

	template <typename Candidate>
	using is_convertible_to_arc = std::is_convertible<std::pair<size_t, size_t>, Candidate>;

	template <typename... Arcs>
	inline void Graph::addArcs(std::pair<size_t, size_t> const& arc, Arcs... arcs) {
		static_assert(check_all_v<is_convertible_to_arc, Arcs...>,
		              "The arguments of addArcs must be convertible to std::pair<int, int>");
		connections[arc.first].push_back(arc.second);
		addArcs(arcs...);
	}

	template <typename Functor>
	void Graph::eachVertices(Functor&& functor) const {
		static_assert(std::is_convertible<Functor, std::function<void(ConstNode)>>::value,
		              "The function must be convertible to a function of type void(ConstNode)");
		for(size_t i = 0; i < connections.size(); ++i) {
			functor((*this)[i]);
		}
	}

	template <typename Functor>
	void Graph::eachEdges(Functor&& functor) const {
		static_assert(
		        std::is_convertible<Functor, std::function<void(ConstNode, ConstNode)>>::value,
		        "The function must be convertible to a function of type void(ConstNode, "
		        "ConstNode)");
		for(size_t i = 0; i < connections.size(); ++i) {
			for(auto j : connections[i]) {
				functor((*this)[i], (*this)[j]);
			}
		}
	}

	template <typename Functor>
	void Graph::eachAdjacents(ConstNode vertex, Functor&& functor) const {
		static_assert(std::is_convertible<Functor, std::function<void(ConstNode)>>::value,
		              "The function must be convertible to a function of type void(ConstNode)");
		for(auto i : connections[vertex.getId()]) {
			functor((*this)[i]);
		}
	}
}

#endif
