#include "list_node.hpp"
#include "properties.hpp"

namespace graph {
	namespace list {

		template class Node<NoProperty>;
		template class Node<AstarNodeProperty<void>>;
		template class Node<AstarNodeProperty<std::string>>;
		template class ConstNode<NoProperty>;
		template class ConstNode<AstarNodeProperty<void>>;
		template class ConstNode<AstarNodeProperty<std::string>>;

	}
}
