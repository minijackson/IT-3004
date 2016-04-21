#include <graph.hpp>

#include <iostream>

int main() {
	using Graph = graph::list::Graph<graph::NoProperty, graph::NoProperty>;

	Graph myGraph{{"0", "1"},
	              {"0", "5"},
	              {"1", "2"},
	              {"2", "5"},
	              {"3", "1"},
	              {"3", "2"},
	              {"4", "2"},
	              {"5", "1"},
	              {"5", "3"},
	              {"5", "4"}};

	std::cout << makeDigraph("myGraph", myGraph);
}
