#include "matrix_graph.hpp"

namespace graph {
	namespace matrix {

		template class Graph<NoProperty, NoProperty>;
		template class Graph<NoProperty, WeightedProperty>;
		template class Graph<AstarNodeProperty, WeightedProperty>;

	}
}
