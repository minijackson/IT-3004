#include "list_graph.hpp"

namespace graph {
	namespace list {

		template class Graph<NoProperty, NoProperty>;
		template class Graph<NoProperty, WeightedProperty>;
		template class Graph<AstarNodeProperty, WeightedProperty>;

	}
}
