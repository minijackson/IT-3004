#include "list_node.hpp"
#include "properties.hpp"

namespace graph {
	namespace list {

		template class Node<NoProperty>;
		template class Node<AstarNodeProperty>;
		template class ConstNode<NoProperty>;
		template class ConstNode<AstarNodeProperty>;

	}
}
