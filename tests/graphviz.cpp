#include <graph.hpp>
#include <algorithms.hpp>

#include <iostream>

int main() {
	using Graph = graph::list::Graph<graph::NoProperty, graph::WeightedProperty>;

	Graph myGraph{std::make_tuple("0", "1", graph::WeightedProperty{1}),
	              std::make_tuple("0", "5", graph::WeightedProperty{2}),
	              std::make_tuple("1", "2", graph::WeightedProperty{3}),
	              std::make_tuple("2", "5", graph::WeightedProperty{4}),
	              std::make_tuple("3", "1", graph::WeightedProperty{5}),
	              std::make_tuple("3", "2", graph::WeightedProperty{4}),
	              std::make_tuple("4", "2", graph::WeightedProperty{3}),
	              std::make_tuple("5", "1", graph::WeightedProperty{2}),
	              std::make_tuple("5", "3", graph::WeightedProperty{1}),
	              std::make_tuple("5", "4", graph::WeightedProperty{0})};

	std::cout << makeDigraph("myGraph", myGraph);
}
