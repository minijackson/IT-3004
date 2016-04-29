#include <graph.hpp>
#include <algorithms.hpp>

#include <iostream>

int main() {
	using Graph = graph::list::Graph<graph::NoProperty, graph::WeightedProperty>;

	Graph myGraph{{"0", "1", graph::WeightedProperty{1}},
	              {"0", "5", graph::WeightedProperty{2}},
	              {"1", "2", graph::WeightedProperty{3}},
	              {"2", "5", graph::WeightedProperty{4}},
	              {"3", "1", graph::WeightedProperty{5}},
	              {"3", "2", graph::WeightedProperty{4}},
	              {"4", "2", graph::WeightedProperty{3}},
	              {"5", "1", graph::WeightedProperty{2}},
	              {"5", "3", graph::WeightedProperty{1}},
	              {"5", "4", graph::WeightedProperty{0}}};

	std::cout << makeDigraph("myGraph", myGraph);
}
