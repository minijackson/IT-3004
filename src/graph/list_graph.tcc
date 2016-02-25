#ifndef _GRAPH_LIB_LIST_GRAPH_TCC_
#define _GRAPH_LIB_LIST_GRAPH_TCC_

#include "list_graph.hpp"

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
}

#endif
